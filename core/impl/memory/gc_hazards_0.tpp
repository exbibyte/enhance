//local structures for garbage collection
template< typename T >
thread_local ::e2::mt::lock_rw_sync_impl gc_hazards_0_impl< T >::_hazards_modify;

template< typename T >
thread_local ::e2::mt::lock_rw_sync_impl gc_hazards_0_impl< T >::_garbage_modify;

template< typename T >
thread_local std::list< T * > gc_hazards_0_impl< T >::_hazards;

template< typename T >
thread_local std::list< T * > gc_hazards_0_impl< T >::_garbage;

template< typename T >
thread_local size_t gc_hazards_0_impl< T >::_count_garbage = 0;

//global stuctures for garbage collection
template< typename T >
::e2::mt::lock_rw_sync_impl gc_hazards_0_impl< T >::_global_hazards_modify;

template< typename T >
::e2::mt::lock_rw_sync_impl gc_hazards_0_impl< T >::_global_garbage_modify;

template< typename T >
std::list< T * > gc_hazards_0_impl< T >::_global_hazards;

template< typename T >
std::list< T * > gc_hazards_0_impl< T >::_global_garbage;

template< typename T >
bool gc_hazards_0_impl< T >::gc_h_stat_recycled(){
#ifdef DEBUG_INFO
    std::cout<<"thread local number of garbage recycled: " << _count_garbage << std::endl;
#endif
    return true;
}
template< typename T >
bool gc_hazards_0_impl< T >::gc_h_garbage_clear(){
    std::list< T * > temp_list {};
#ifdef DEBUG_INFO
    std::cout<<"num garbage: " << _garbage.size() << std::endl;
#endif
    _garbage_modify.lock( ::e2::interface::lock_access_type::WRITE );
    for( auto & i : _garbage ){
	if( i ){
	    if( gc_h_hazard_find( i ) ){
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
    temp_list.clear();
    return true;
}
template< typename T >
bool gc_hazards_0_impl< T >::gc_h_garbage_clear_force(){
#ifdef DEBUG_INFO
    std::cout<<"num garbage force clear: " << _garbage.size() << std::endl;
#endif
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
bool gc_hazards_0_impl< T >::gc_h_hazard_add( T * n ){
    _hazards_modify.lock( ::e2::interface::lock_access_type::WRITE );
    _hazards.push_back( n );
    _hazards_modify.unlock( ::e2::interface::lock_access_type::WRITE );
    return true;
}

template< typename T >
bool gc_hazards_0_impl< T >::gc_h_hazard_remove( T * n ){
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
bool gc_hazards_0_impl< T >::gc_h_hazard_find( T * n ){
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
bool gc_hazards_0_impl< T >::gc_h_dump_global_garbage(){
    _garbage_modify.lock( ::e2::interface::lock_access_type::WRITE );
    _global_garbage_modify.lock( ::e2::interface::lock_access_type::WRITE );
    _garbage.splice( _garbage.end(), _global_garbage );
    _global_garbage_modify.unlock( ::e2::interface::lock_access_type::WRITE );
    _garbage_modify.unlock( ::e2::interface::lock_access_type::WRITE );
    return true;
}

template< typename T >
bool gc_hazards_0_impl< T >::gc_h_dump_global_hazards(){
    _hazards_modify.lock( ::e2::interface::lock_access_type::WRITE );
    _global_hazards_modify.lock( ::e2::interface::lock_access_type::WRITE );
    _hazards.splice( _hazards.end(), _global_hazards );
    _global_hazards_modify.unlock( ::e2::interface::lock_access_type::WRITE );
    _hazards_modify.unlock( ::e2::interface::lock_access_type::WRITE );
    return true;
}

template< typename T >
bool gc_hazards_0_impl< T >::gc_h_garbage_publish(){
    //thread publish its local garbage, transfer ownership
    _garbage_modify.lock( ::e2::interface::lock_access_type::WRITE );
    _global_garbage_modify.lock( ::e2::interface::lock_access_type::WRITE );
    _global_garbage.splice( _global_garbage.end(), _garbage );
    _global_garbage_modify.unlock( ::e2::interface::lock_access_type::WRITE );
    _garbage_modify.unlock( ::e2::interface::lock_access_type::WRITE );
    return true;
}

template< typename T >
bool gc_hazards_0_impl< T >::gc_h_hazards_publish(){
    //thread publish its local hazard pointers, copy items
    _hazards_modify.lock( ::e2::interface::lock_access_type::WRITE );
    _global_hazards_modify.lock( ::e2::interface::lock_access_type::WRITE );
    for( auto & i : _hazards ){
	_global_hazards.push_back( i );
    }
    _global_hazards_modify.unlock( ::e2::interface::lock_access_type::WRITE );
    _hazards_modify.unlock( ::e2::interface::lock_access_type::WRITE );
    return true;
}
