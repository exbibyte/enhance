template< typename T >
std::list< typename queue_lockfree_total_impl<T>::Node * > queue_lockfree_total_impl<T>::_hazards;
template< typename T >
thread_local std::list< typename queue_lockfree_total_impl<T>::Node * > queue_lockfree_total_impl<T>::_garbage;

template< typename T >
queue_lockfree_total_impl<T>::queue_lockfree_total_impl(){
    Node * sentinel = new Node();
    _head.store( sentinel );
    _tail.store( sentinel );
}
template< typename T >
queue_lockfree_total_impl<T>::~queue_lockfree_total_impl(){
    clear();
    if( _head ){
	Node * n = _head.load();
	if( _head.compare_exchange_strong( n, nullptr, std::memory_order_relaxed ) ){
	    if( n ){
		delete n;
		_head.store(nullptr);
		_tail.store(nullptr);
	    }
	}
    }
}
template< typename T >
bool queue_lockfree_total_impl<T>::push_back( T const * val ){ //push item to the tail
    Node * new_node = new Node( val );
    while( true ){
	Node * tail = _tail.load( std::memory_order_acquire );
	if( nullptr == tail ){ //TODO: stricter check if _head/_tail is deallocated during destruction
	    continue; //this should not happen unless during destructor call
	}

	//hazard check start for tail
	hazard_add( tail );
	Node * tail_next = tail->_next.load( std::memory_order_acquire );
	hazard_remove( tail );

	if( nullptr == tail_next ){  //determine if thread has reached tail	  
	    if( tail->_next.compare_exchange_weak( tail_next, new_node, std::memory_order_acq_rel ) ){ //add new node
		_tail.compare_exchange_strong( tail, new_node, std::memory_order_acq_rel ); //thread succeeds, thus try setting new tail, if it fails it means another thread has succeeded
		return true;
	    }
	}else{
	    // // possible optimization here
	    // _tail.compare_exchange_weak( tail, tail_next, std::memory_order_relaxed ); //update tail and retry
	}
	std::this_thread::yield();
    }
}
template< typename T >
bool queue_lockfree_total_impl<T>::pop_front( T * val ){ //obtain item from the head
    while( true ){
	Node * head = _head.load( std::memory_order_acquire );
	Node * tail = _tail.load( std::memory_order_acquire );
	if( nullptr == head ){ //TODO: stricter check if _head/_tail is deallocated during destruction
	    return false; //shoud not happen unless during destructor call
	}

	//hazard check start for head
	hazard_add( head );
	Node * head_next = head->_next.load( std::memory_order_acquire );
	hazard_remove( head );
	
	if( head == _head.load( std::memory_order_acquire ) ){
	    if( head == tail ){ 
	        if( nullptr == head_next ){ //empty
		    return false;
		}else{
		// // possible optimization here
		// _tail.compare_exchange_weak( tail, head_next, std::memory_order_relaxed ); //other thread updated head/tail, so retry
	    	}
	    }else{
	        //hazard check start for head_next
	        hazard_add( head_next );
	        *val = head_next->_val;
		hazard_remove( head_next );
		    
		if( _head.compare_exchange_strong( head, head_next, std::memory_order_acq_rel ) ){ //try unlink an item
		    //per-thread garbage list
		    std::list< Node * > temp_list;
		    _garbage.push_back( head );
		    if( _garbage.size() > 10000 ){
		        // std::cout << "gc cycle" << std::endl;
		        for( auto & i : _garbage ){
			    if( hazard_find( i ) ){
			        temp_list.push_back( i );
			    }else{
			      if(i) delete i;
			    }
			}
			_garbage.swap( temp_list );
		    }
		    return true;
		}else{
		  
		}
	    }
	}
	std::this_thread::yield();
    }
}
template< typename T >
size_t queue_lockfree_total_impl<T>::size(){
    size_t count = 0;
    Node * node = _head.load();
    if( nullptr == node ){
	return 0;
    }
    while( node ){
	Node * next = node->_next.load();
	node = next;
	++count;
    }
    return count - 1; //discount for sentinel node
}
template< typename T >
bool queue_lockfree_total_impl<T>::empty(){
    return size() == 0;
}
template< typename T >
bool queue_lockfree_total_impl<T>::clear(){
    while( !empty() ){
	T t;
	pop_front( &t );
    }
    garbage_clear();
    return true;
}
template< typename T >
bool queue_lockfree_total_impl<T>::garbage_clear(){
    std::cout<<"num garbage: " << _garbage.size() << std::endl;
    for( auto & i : _garbage ){
      if( i ) delete i;
    }
    _garbage.clear();
    return true;
}
template< typename T >
bool queue_lockfree_total_impl<T>::hazard_add( Node * n ){
    _hazard_modify.lock( ::e2::interface::lock_access_type::WRITE );
    _hazards.push_back( n );
    _hazard_modify.unlock( ::e2::interface::lock_access_type::WRITE );
    return true;
}

template< typename T >
bool queue_lockfree_total_impl<T>::hazard_remove( Node * n ){
    _hazard_modify.lock( ::e2::interface::lock_access_type::WRITE );
    for( auto it = _hazards.begin(), it_e = _hazards.end(); it != it_e; ++it ){
        if( *it == n ){
	    _hazards.erase(it);
	    break;
	}
    }
    _hazard_modify.unlock( ::e2::interface::lock_access_type::WRITE );
    return true;
}

template< typename T >
bool queue_lockfree_total_impl<T>::hazard_find( Node * n ){
    _hazard_modify.lock( ::e2::interface::lock_access_type::READ );
    for( auto it = _hazards.begin(), it_e = _hazards.end(); it != it_e; ++it ){
        if( *it == n ){
	    return true;
	}
    }
    _hazard_modify.unlock( ::e2::interface::lock_access_type::READ );
    return false;
}
