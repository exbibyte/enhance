template< typename T >
::e2::mt::lock_rw_sync_impl queue_lockfree_total_impl<T>::_hazard_modify;
template< typename T >
std::list< queue_lockfree_total_impl<T> * > queue_lockfree_total_impl<T>::_common_group;
template< typename T >
std::list< typename queue_lockfree_total_impl<T>::Node * > queue_lockfree_total_impl<T>::_hazards;

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

	//hazard check start
	_hazard_modify.lock( ::e2::interface::lock_access_type::READ );
	bool contains_hazard = false;
	for( auto & i : _hazards ){
	    if( i == tail ){
		contains_hazard = true;
		break;
	    }	
	}	    
	_hazard_modify.unlock( ::e2::interface::lock_access_type::READ );
	if( contains_hazard )
	    continue;
	//hazard check end

	Node * tail_next = tail->_next.load( std::memory_order_acquire );
	if( nullptr == tail_next ){  //determine if thread has reached tail
	    if( tail->_next.compare_exchange_weak( tail_next, new_node, std::memory_order_acq_rel ) ){ //add new node
		_tail.compare_exchange_strong( tail, new_node, std::memory_order_acq_rel ); //thread succeeds, thus try setting new tail, if it fails it means another thread has succeeded
		return true;
	    }
	}else{
	    // // possible optimization here
	    // _tail.compare_exchange_weak( tail, tail_next, std::memory_order_relaxed ); //update tail and retry
	    if( 1000000 < _hazards.size() ){
		_hazard_modify.lock( ::e2::interface::lock_access_type::WRITE ); //blocking call
		for( auto & i : _hazards ){
		    if( i )
			delete i;
		}
		_hazards.clear();
		_hazard_modify.unlock( ::e2::interface::lock_access_type::WRITE ); //blocking call
	    }
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

	//hazard check start
	_hazard_modify.lock( ::e2::interface::lock_access_type::READ );
	bool contains_hazard = false;
	for( auto & i : _hazards ){
	    if( i == head ){
		contains_hazard = true;
		break;
	    }	
	}	    
	_hazard_modify.unlock( ::e2::interface::lock_access_type::READ );
	if( contains_hazard )
	    continue;
	//hazard check end

	Node * head_next = head->_next.load( std::memory_order_acquire );
	if( head == tail ){
	    if( nullptr == head_next ){ //empty
		// if( !_hazards.empty() ){
		//     _hazard_modify.lock( ::e2::interface::lock_access_type::WRITE ); //blocking call
		//     for( auto & i : _hazards ){
		// 	if( i )
		// 	    delete i;
		//     }
		//     _hazards.clear();
		//     _hazard_modify.unlock( ::e2::interface::lock_access_type::WRITE ); //blocking call
		// }
		return false;
	    }else{
		// // possible optimization here
		// _tail.compare_exchange_weak( tail, head_next, std::memory_order_relaxed ); //other thread updated head/tail, so retry
	    }
	}else{
	    if( head_next ){
		if( _head.compare_exchange_strong( head, head_next, std::memory_order_acq_rel ) ){ //try unlink an item
		    //todo: per-thread free list
		    *val = head_next->_val; //thread suceeds and returns
		    _hazard_modify.lock( ::e2::interface::lock_access_type::WRITE ); //blocking call
		    _hazards.push_back( head );
		    _hazard_modify.unlock( ::e2::interface::lock_access_type::WRITE ); //blocking call 

		    return true;
		}
	    }else{

	    }
	}
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
    _hazard_modify.lock( ::e2::interface::lock_access_type::WRITE ); //blocking call 
    for( auto & i : _hazards )
    	delete i;
    _hazard_modify.unlock( ::e2::interface::lock_access_type::WRITE ); //blocking call 
    return true;
}
