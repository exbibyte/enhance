#include <list>
#include <cassert>
#include <iostream>

#include "i_rendernode.hpp"
#include "i_renderpackage.hpp"
#include "renderbackend0.hpp"

namespace e2 { namespace render {

renderbackend0_impl::renderbackend0_impl(){
    constexpr size_t buf_len_bytes = 1 << 20;
#ifdef DEBUG_INFO
    std::cout << "allocated buffer of length: " << buf_len_bytes << std::endl;
#endif
    assert( _buf.buffer_resize( buf_len_bytes ) );
}
bool renderbackend0_impl::renderbackend_get_window( void ** win ){
    assert( win );
    *win = (void*) _rd._window;
    return true;
}
bool renderbackend0_impl::renderbackend_process_rendernodes( std::list< ::e2::interface::i_rendernode * > * nodes  ){
    std::list< ::e2::interface::i_rendernode * > nodes_scheduled;
    assert( _sch.schedule_graph( nodes, &nodes_scheduled ) );
    assert( _converter.process( &_buf, &nodes_scheduled, &_renderpackages ) );
    return true;
}
bool renderbackend0_impl::renderbackend_process_commit(){
    _renderpackages_committed.push_back( std::move( _renderpackages ) );
    _renderpackages = {};
    return true;
}
bool renderbackend0_impl::renderbackend_process_renderpackages( int count ){
    if( count < 0 ){
	for( auto & i : _renderpackages_committed ){
	    assert( _executer.process( &_rd, &i ) );
	}
	_renderpackages_committed.clear();
    }else{
	for( int i = 0; i < count; ++i ){
	    if( true == _renderpackages_committed.empty() )
		break;
	    auto & l = _renderpackages_committed.front();
	    assert( _executer.process( &_rd, &l ) );
	    _renderpackages_committed.pop_front();
	}
    }
    return true;
}
	
} }
