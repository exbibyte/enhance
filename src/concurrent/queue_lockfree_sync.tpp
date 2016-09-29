template< typename T >
queue_lockfree_sync_impl<T>::queue_lockfree_sync_impl(){
    Node * sentinel = new Node();
    _head.store( sentinel );
    _tail.store( sentinel );
}
template< typename T >
queue_lockfree_sync_impl<T>::~queue_lockfree_sync_impl(){
    clear();
    Node * node = _head.load();
    while( node ){
	Node * next = node->_next.load();
	delete node;
	node = next;
    }    
}
template< typename T >
bool queue_lockfree_sync_impl<T>::push_back( T & val ){ //push an item to the tail
    Node * new_node = new Node( val ); //type is ITEM if value argument is present
    while( true ){
	Node * tail = _tail.load( std::memory_order_relaxed );
	Node * head = _head.load( std::memory_order_relaxed );
	if( tail == head || _t_node_type::ITEM == tail->_type ){ //try enque an item by putting an ITEM object in queue
	    Node * tail_next = tail->_next.load( std::memory_order_relaxed );
	    if( tail == _tail ){
		if( nullptr != tail_next ){ //tail is invalidated
		    _tail.compare_exchange_weak( tail, tail_next ); //update tail before retry, enque lineariation point 2 if successful
		}else if( tail->_next.compare_exchange_weak( tail_next, new_node ) ){ //try commit item as the next node, enque linearization point 1 if successful
		    _tail.compare_exchange_weak( tail, new_node ); //try update tail after commit, enque linearization point 2 if successful
                    //wait for synchronization with dequing thread for the signal that transaction is complete
		    T * value_ptr;
		    bool value_is_changing;
		    do{
			value_ptr = new_node->_val_guard.load( std::memory_order_acquire );
		        value_is_changing = new_node->_val_change.load( std::memory_order_acquire );
		    }while( nullptr != value_ptr && false != value_is_changing ); //value_ptr should be nullptr if it breaks out of this loop

		    //performance optimization start
		    head = _head.load( std::memory_order_relaxed );
		    if( new_node == head->_next.load( std::memory_order_relaxed ) ){
			Node * node_recycle = head;
			if( _head.compare_exchange_weak( head, new_node ) ){ //update head, linearization point 2 if successful
			    delete node_recycle; //delete synchronized node
			    node_recycle = nullptr;
			}
		    }
		    //performance optimization end
		    return true;
		}
	    }
	}else{ //try fulfill a waiting dequing thread in queue
	    Node * n = head->_next.load( std::memory_order_relaxed );
	    if( tail != _tail.load( std::memory_order_relaxed ) || head != _head.load( std::memory_order_relaxed ) || nullptr == n ){
		//nodes are invalidated, retry
		continue;
	    }
	    bool success = false;
	    T * value_ptr = n->_val_guard.load( std::memory_order_acquire );
	    if( nullptr == value_ptr ){
		bool b_expected = false;
		bool b_new = true;
		if( n->_val_change.compare_exchange_weak( b_expected, b_new, std::memory_order_acquire ) ){ //claim ownership of value_ptr
		    success = true;
		    //set n->_val_ptr to valid pointer
		    value_ptr = &n->_val;
		    //copy value
		    *value_ptr = val;
		    n->_val_guard.store( &n->_val, std::memory_order_release ); //deque linearization point 1 if successful
		    n->_val_change.store( false, std::memory_order_release );//release ownership of value_ptr
		}
	    }
	    Node * node_recycle = head;
	    if( _head.compare_exchange_weak( head, n ) ){ //update head, deque linearization point 2 if successful
		delete node_recycle; //delete synchronized node
		node_recycle = nullptr;
	    }
	    if( success ){ //return for succeeding enquing thread
		delete new_node; //delete unused node after succesful enquing thread synchronizes with an existing dequing RESERVATION node
		new_node = nullptr;
		return true;
	    }
	}
    }
}
template< typename T >
bool queue_lockfree_sync_impl<T>::pop_front( T & val ){ //pop an item from the head
    //mirror implementation to that of push_back
    Node * new_node = new Node(); // default type is RESERVATION is no argument is presented
    while( true ){
	Node * tail = _tail.load( std::memory_order_relaxed );
	Node * head = _head.load( std::memory_order_relaxed );
	if( tail == head || _t_node_type::RESERVATION == tail->_type ){ //try deque an item by putting a RESERVATION object in queue
	    Node * tail_next = tail->_next.load( std::memory_order_relaxed );
	    if( tail == _tail ){
		if( nullptr != tail_next ){ //tail is invalidated
		    _tail.compare_exchange_weak( tail, tail_next ); //update tail before retry, enque lineariation point 2 if successful
		}else if( tail->_next.compare_exchange_weak( tail_next, new_node ) ){ //try commit item as the next node, enque linearization point 1 if successful
		    _tail.compare_exchange_weak( tail, new_node ); //try update tail after commit, enque linearization point 2 if successful
                    //wait for synchronization with enquing thread for the signal that transaction is complete
		    T * value_ptr;
		    bool value_is_changing;
		    do{
			value_ptr = new_node->_val_guard.load( std::memory_order_acquire );
		        value_is_changing = new_node->_val_change.load( std::memory_order_acquire );
		    }while( nullptr == value_ptr && false != value_is_changing ); //value_ptr should be valid if it breaks out of this loop

		    //performance optimization start
		    head = _head.load( std::memory_order_relaxed );
		    if( new_node == head->_next.load( std::memory_order_relaxed ) ){
			Node * node_recycle = head;
			if( _head.compare_exchange_weak( head, new_node ) ){ //update head, linearization point 2 if successful
			    delete node_recycle; //delete synchronized node
			    node_recycle = nullptr;
			}
		    }
		    //performance optimization end
		    return true;
		}
	    }
	}else{ //try fulfill a waiting enquing thread in queue
	    Node * n = head->_next.load( std::memory_order_relaxed );
	    if( tail != _tail.load( std::memory_order_relaxed ) || head != _head.load( std::memory_order_relaxed ) || nullptr == n ){
		//nodes are invalidated, retry
		continue;
	    }
	    bool success = false;
	    T * value_ptr = n->_val_guard.load( std::memory_order_acquire );
	    if( nullptr != value_ptr ){ //value_ptr should be set as valid pointer by enquing thread
		bool b_expected = false;
		bool b_new = true;
		if( n->_val_change.compare_exchange_weak( b_expected, b_new, std::memory_order_acquire ) ){ //claim ownership of value_ptr
		    success = true;
		    //copy value
		    val = *value_ptr;
		    //set n->_val_ptr to nullptr
		    value_ptr = nullptr;
		    n->_val_guard.store( value_ptr, std::memory_order_release ); //linearization point 1 if successful
		    n->_val_change.store( false, std::memory_order_release );//release ownership of value_ptr
		}
	    }
	    Node * node_recycle = head;
	    if( _head.compare_exchange_weak( head, n ) ){ //update head, linearization point 2 if successful
		delete node_recycle; //delete synchronized node
		node_recycle = nullptr;
	    }
	    if( success ){ //return for succeeding dequing thread
		delete new_node; //delete unused node after succesful dequing thread synchronizes with an existing enquing ITEM node
		new_node = nullptr;
		return true;
	    }
	}
    }
}
template< typename T >
size_t queue_lockfree_sync_impl<T>::size(){
    size_t count = 0;
    Node * node = _head.load();
    while( node ){
	Node * next = node->_next.load();
	node = next;
	++count;
    }
    return count - 1; //discount for sentinel node
}
template< typename T >
bool queue_lockfree_sync_impl<T>::empty(){
    return size() == 0 ;
}
template< typename T >
bool queue_lockfree_sync_impl<T>::clear(){
    while( !empty() ){
	T t;
	pop_front( t );
    }
    return true;
}

