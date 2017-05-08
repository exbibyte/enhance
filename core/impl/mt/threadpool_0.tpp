template< class ThreadType >
threadpool_0_impl< ThreadType >::threadpool_0_impl( size_t n ){
    for( size_t i = 0; i < n; ++i ){
	ThreadType * t = new ThreadType;
	bool available = true;
	_threads.push_back( { t, available } );
    }
}
template< class ThreadType >
threadpool_0_impl< ThreadType >::~threadpool_0_impl(){
    //blocking
    while( true ){
	std::list< std::pair< ThreadType *, bool > > temp {};
	for( auto & i : _threads ){
	    if( nullptr == i.first ){
		continue;
	    }
	    ::e2::interface::e_thread_state s;
	    i.first->get_thread_state( & s );
	    if( false == i.second || ::e2::interface::e_thread_state::STOPPED != s ){
		temp.push_back( std::move( i ) );
	    }
	}
	_threads.swap( temp );
	if( _threads.empty() )
	    break;
    }
}
template< class ThreadType >
bool threadpool_0_impl< ThreadType >::threadpool_process( ::e2::interface::e_threadpool_action a, void * v ){
    switch( a ){
    case ::e2::interface::e_threadpool_action::spawn:
    {
	ThreadType * t = new ThreadType;
	bool available = true;
	_threads.push_back( { t, available } );
	return true;
    }
    break;
    case ::e2::interface::e_threadpool_action::terminate:
    {
	ThreadType * t = ( ThreadType * ) v;
	for( auto it = _threads.begin(), it_e = _threads.end(); it != it_e; ++it ){
	    if( (*it).first == t ){
		if( t ){
		    ::e2::interface::e_thread_state s;
		    t->get_thread_state( & s );
		    if( ::e2::interface::e_thread_state::STOPPED != s )
			return false;
		    if( t ) delete t;
		}
		_threads.erase( it );
		return true;
	    }
	}
	return false;
    }
    break;
    case ::e2::interface::e_threadpool_action::withdraw:
    {
	ThreadType ** t = ( ThreadType ** ) v;
	if( nullptr == t ) return false;
	for( auto it = _threads.begin(), it_e = _threads.end(); it != it_e; ++it ){
	    bool & available = it->second;
	    if( available ){
		available = false;
		*t = it->first;
		return true;
	    }
	}
	return false;
    }
    break;
    case ::e2::interface::e_threadpool_action::deposit:
    {
	ThreadType * t = ( ThreadType * ) v;
	for( auto it = _threads.begin(), it_e = _threads.end(); it != it_e; ++it ){
	    if( t == it->first ){
		bool & available = it->second;
		if( false == available ){
		    available = true;
		    return true;
		}else{
		    return false;
		}
	    }
	}
	return false;
    }
    break;
    case ::e2::interface::e_threadpool_action::query_num_total:
    {
	size_t * n = ( size_t * ) v;
	if( nullptr == n ) return false;
	*n = _threads.size();
	return true;
    }
    break;
    case ::e2::interface::e_threadpool_action::query_num_available:
    {
	size_t * n = ( size_t * ) v;
	if( nullptr == n ) return false;
	*n = 0;
	for( auto it = _threads.begin(), it_e = _threads.end(); it != it_e; ++it ){
	    if( true == it->second )
		++(*n);
	}
	return true;
    }
    break;
    case ::e2::interface::e_threadpool_action::query_num_unavailable:
    {
	size_t * n = ( size_t * ) v;
	if( nullptr == n ) return false;
	*n = 0;
	for( auto it = _threads.begin(), it_e = _threads.end(); it != it_e; ++it ){
	    if( false == it->second )
		++(*n);
	}
	return true;
    }
    break;
    default:
	return false;
    }
    return false;
}
