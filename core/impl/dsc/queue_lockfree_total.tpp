//local structures for garbage collection
template< typename T >
thread_local ::e2::mt::lock_rw_sync_impl queue_lockfree_total_impl<T>::_hazards_modify;

template< typename T >
thread_local ::e2::mt::lock_rw_sync_impl queue_lockfree_total_impl<T>::_garbage_modify;

template< typename T >
thread_local std::list< typename queue_lockfree_total_impl<T>::Node * > queue_lockfree_total_impl<T>::_hazards;

template< typename T >
thread_local std::list< typename queue_lockfree_total_impl<T>::Node * > queue_lockfree_total_impl<T>::_garbage;

template< typename T >
thread_local size_t queue_lockfree_total_impl<T>::_count_garbage = 0;

//global stuctures for garbage collection
template< typename T >
::e2::mt::lock_rw_sync_impl queue_lockfree_total_impl<T>::_global_hazards_modify;

template< typename T >
::e2::mt::lock_rw_sync_impl queue_lockfree_total_impl<T>::_global_garbage_modify;

template< typename T >
std::list< typename queue_lockfree_total_impl<T>::Node * > queue_lockfree_total_impl<T>::_global_hazards;

template< typename T >
std::list< typename queue_lockfree_total_impl<T>::Node * > queue_lockfree_total_impl<T>::_global_garbage;

template< typename T >
std::mutex queue_lockfree_total_impl<T>::_mtx;

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
    _garbage_modify.lock( ::e2::interface::lock_access_type::WRITE );
    std::cout<<"sch destructor local garbage clear: " << _garbage.size() << std::endl;
    for( auto & i : _garbage ){
	if( i ) delete i;
    }
    _garbage_modify.unlock( ::e2::interface::lock_access_type::WRITE );

    //global garbage
    {
	std::lock_guard<std::mutex> lock( _mtx );
	// _global_garbage_modify.lock( ::e2::interface::lock_access_type::WRITE );
	std::cout<<"sch destructor global garbage clear: " << _global_garbage.size() << std::endl;
	for( auto & i : _global_garbage ){
	    if( i ) delete i;
	}
	_global_garbage.clear();
	// _global_garbage_modify.unlock( ::e2::interface::lock_access_type::WRITE );
    }

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
	hazard_add( tail );
	Node * tail_next = tail->_next.load( std::memory_order_acquire );
	hazard_remove( tail );

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
	hazard_add( head );
	Node * head_next = head->_next.load( std::memory_order_acquire );
	hazard_remove( head );
	
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
	        hazard_add( head_next );
		*val = head_next->_val;
		hazard_remove( head_next );
		Node * to_delete = head;
		if( _head.compare_exchange_strong( head, head_next, std::memory_order_acq_rel ) ){ //try unlink an item

		    // //per-thread garbage list
		    _garbage_modify.lock( ::e2::interface::lock_access_type::WRITE );
		    _garbage.push_back( to_delete );
		    _garbage_modify.unlock( ::e2::interface::lock_access_type::WRITE );
		    // delete to_delete;
		    return true;
		}
	    }
	}
	std::this_thread::yield();
    }
}
template< typename T >
bool queue_lockfree_total_impl<T>::garbage_publish(){
    std::lock_guard<std::mutex> lock( _mtx );
    //thread publish its local garbage, transfer ownership
    // _garbage_modify.lock( ::e2::interface::lock_access_type::WRITE );
    // _global_garbage_modify.lock( ::e2::interface::lock_access_type::WRITE );
    _global_garbage.splice( _global_garbage.end(), _garbage );
    // _global_garbage_modify.unlock( ::e2::interface::lock_access_type::WRITE );
    // _garbage_modify.unlock( ::e2::interface::lock_access_type::WRITE );
    return true;
}
template< typename T >
bool queue_lockfree_total_impl<T>::hazards_publish(){
    std::lock_guard<std::mutex> lock( _mtx );
    //thread publish its local hazard pointers, copy items
    // _hazards_modify.lock( ::e2::interface::lock_access_type::WRITE );
    // _global_hazards_modify.lock( ::e2::interface::lock_access_type::WRITE );
    for( auto & i : _hazards ){
	_global_hazards.push_back( i );
    }
    // _global_hazards_modify.unlock( ::e2::interface::lock_access_type::WRITE );
    // _hazards_modify.unlock( ::e2::interface::lock_access_type::WRITE );
    return true;
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
bool queue_lockfree_total_impl<T>::garbage_stat_thread_local(){
    std::cout<<"thread local number of garbage recycled: " << _count_garbage << std::endl;
    return true;
}
template< typename T >
bool queue_lockfree_total_impl<T>::garbage_clear(){
    std::list< Node * > temp_list {};
    // std::cout<<"num garbage: " << _garbage.size() << std::endl;
    _garbage_modify.lock( ::e2::interface::lock_access_type::WRITE );
    for( auto & i : _garbage ){
	if( i ){
	    if( hazard_find( i ) ){
		temp_list.push_back( i );
	    }else{
		delete i;
		i = nullptr;
		++_count_garbage;
	    }
	}
    }
    _hazards_modify.lock( ::e2::interface::lock_access_type::WRITE );
    _hazards.clear();
    _hazards_modify.unlock( ::e2::interface::lock_access_type::WRITE );
    _garbage.swap( temp_list );
    _garbage_modify.unlock( ::e2::interface::lock_access_type::WRITE );
    return true;
}
template< typename T >
bool queue_lockfree_total_impl<T>::garbage_clear_force(){
    std::cout<<"num garbage force clear: " << _garbage.size() << std::endl;
    for( auto & i : _garbage ){
	if( i ){
	    delete i;
	    i = nullptr;
	}
    }
    _garbage.clear();
    _hazards.clear();
    return true;
}
template< typename T >
bool queue_lockfree_total_impl<T>::hazard_add( Node * n ){
    _hazards_modify.lock( ::e2::interface::lock_access_type::WRITE );
    _hazards.push_back( n );
    _hazards_modify.unlock( ::e2::interface::lock_access_type::WRITE );
    return true;
}

template< typename T >
bool queue_lockfree_total_impl<T>::hazard_remove( Node * n ){
    _hazards_modify.lock( ::e2::interface::lock_access_type::WRITE );
    for( auto it = _hazards.begin(), it_e = _hazards.end(); it != it_e; ++it ){
        if( *it == n ){
	    _hazards.erase(it);
	    break;
	}
    }
    _hazards_modify.unlock( ::e2::interface::lock_access_type::WRITE );
    return true;
}

template< typename T >
bool queue_lockfree_total_impl<T>::hazard_find( Node * n ){
    _hazards_modify.lock( ::e2::interface::lock_access_type::READ );
    for( auto it = _hazards.begin(), it_e = _hazards.end(); it != it_e; ++it ){
        if( *it == n ){
	    return true;
	}
    }
    _hazards_modify.unlock( ::e2::interface::lock_access_type::READ );
    return false;
}

template< typename T >
bool queue_lockfree_total_impl<T>::dump_global_garbage(){
    std::lock_guard<std::mutex> lock( _mtx );
    // _garbage_modify.lock( ::e2::interface::lock_access_type::WRITE );
    // _global_garbage_modify.lock( ::e2::interface::lock_access_type::WRITE );
    _garbage.splice( _garbage.end(), _global_garbage );
    // _global_garbage_modify.unlock( ::e2::interface::lock_access_type::WRITE );
    // _garbage_modify.unlock( ::e2::interface::lock_access_type::WRITE );
    return true;
}

template< typename T >
bool queue_lockfree_total_impl<T>::dump_global_hazards(){
    std::lock_guard<std::mutex> lock( _mtx );
    // _hazards_modify.lock( ::e2::interface::lock_access_type::WRITE );
    // _global_hazards_modify.lock( ::e2::interface::lock_access_type::WRITE );
    _hazards.splice( _hazards.end(), _global_hazards );
    // _global_hazards_modify.unlock( ::e2::interface::lock_access_type::WRITE );
    // _hazards_modify.unlock( ::e2::interface::lock_access_type::WRITE );
    return true;
}
