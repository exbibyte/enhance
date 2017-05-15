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
    _gc._garbage_modify.lock( ::e2::interface::lock_access_type::WRITE );
#ifdef DEBUG_INFO
    std::cout<<"pool destructor local garbage clear: " << _gc._garbage.size() << std::endl;
#endif
    for( auto & i : _gc._garbage ){
	if( i ) delete i;
    }
    _gc._garbage_modify.unlock( ::e2::interface::lock_access_type::WRITE );

    _gc._global_garbage_modify.lock( ::e2::interface::lock_access_type::WRITE );
#ifdef DEBUG_INFO
    std::cout<<"pool destructor global garbage clear: " << _gc._global_garbage.size() << std::endl;
#endif
    for( auto & i : _gc._global_garbage ){
	if( i ) delete i;
    }
    _gc._global_garbage.clear();
    _gc._global_garbage_modify.unlock( ::e2::interface::lock_access_type::WRITE );

}
template< typename T >
bool queue_lockfree_total_impl<T>::push_back( T const * val ){ //push item to the tail
    Node * new_node = new Node( val );
    while( true ){
	Node * tail = _tail.load( std::memory_order_relaxed );
	if( nullptr == tail ){ //TODO: stricter check if _head/_tail is deallocated during destruction
	    continue; //this should not happen unless during destructor call
	}

	//hazard check start for tail
	_gc.gc_h_hazard_add( tail );
	Node * tail_next = tail->_next.load( std::memory_order_acquire );
	_gc.gc_h_hazard_remove( tail );

	if( nullptr == tail_next ){  //determine if thread has reached tail	  
	    if( tail->_next.compare_exchange_weak( tail_next, new_node, std::memory_order_acq_rel ) ){ //add new node
		//thread succeeds, thus try setting new tail, if it fails it means another thread has succeeded
		Node * tail_old = tail;
		while( false == _tail.compare_exchange_strong( tail_old, new_node, std::memory_order_acq_rel ) ){
		    tail_old = tail;
		}
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
	Node * head = _head.load( std::memory_order_relaxed );
	Node * tail = _tail.load( std::memory_order_relaxed );
	if( nullptr == head ){ //TODO: stricter check if _head/_tail is deallocated during destruction
	    return false; //shoud not happen unless during destructor call
	}

	//hazard check start for head
	_gc.gc_h_hazard_add( head );
	Node * head_next = head->_next.load( std::memory_order_acquire );
	_gc.gc_h_hazard_remove( head );
	
	if( head == _head.load( std::memory_order_acquire ) ){
	    if( head == tail ){ 
	        if( nullptr == head_next ){ //empty
		    return false;
		}else{
		// possible optimization here
		// _tail.compare_exchange_weak( tail, head_next, std::memory_order_relaxed ); //other thread updated head/tail, so retry
	    	}
	    }else{
	        //hazard check start for head_next
	        _gc.gc_h_hazard_add( head_next );
		*val = head_next->_val;
		_gc.gc_h_hazard_remove( head_next );
		Node * to_delete = head;
		if( _head.compare_exchange_strong( head, head_next, std::memory_order_acq_rel ) ){ //try unlink an item

		    // //per-thread garbage list
		    _gc._garbage_modify.lock( ::e2::interface::lock_access_type::WRITE );
		    _gc._garbage.push_back( to_delete );
		    _gc._garbage_modify.unlock( ::e2::interface::lock_access_type::WRITE );
		    // delete to_delete;
		    return true;
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
    _gc.gc_h_garbage_clear();
    return true;
}
