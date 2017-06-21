template< class mem_manager >
renderbackend0_impl< mem_manager >::renderbackend0_impl(){
}
template< class mem_manager >
bool renderbackend0_impl< mem_manager >::renderbackend_get_window( void ** win ){
    assert( win );
    *win = (void*) _rd._window;
    return true;
}
template< class mem_manager >
bool renderbackend0_impl< mem_manager >::renderbackend_process_rendernodes( std::list< ::e2::interface::i_rendernode * > * nodes  ){
    std::list< ::e2::interface::i_rendernode * > nodes_scheduled;
    assert( _sch.schedule_graph( nodes, &_scheduled ) );
    return true;
}
template< class mem_manager >
bool renderbackend0_impl< mem_manager >::renderbackend_process_commit(){
    _committed.push_back( std::move( _scheduled ) );
    _scheduled = {};
    return true;
}
template< class mem_manager >
bool renderbackend0_impl< mem_manager >::renderbackend_process_batches( int count ){
    if( count < 0 ){
	for( auto & i : _committed ){
	    for( auto & j : i ){
		j->process_renderdevice( &_rd );
	    }
	}
	_committed.clear();
    }else{
	for( int i = 0; i < count; ++i ){
	    if( true == _committed.empty() )
		break;
	    auto & l = _committed.front();
	    for( auto & j : l ){
		j->process_renderdevice( &_rd );
	    }
	    _committed.pop_front();
	}
    }
    return true;
}
