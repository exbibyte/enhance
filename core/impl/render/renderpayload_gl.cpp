#include <cstdint>
#include <cassert>

#include "i_renderpayload.hpp"
#include "renderpayload_gl.hpp"
#include "buffer.hpp"

namespace e2 { namespace render {

bool renderpayload_gl::unpack( size_t count_unpack, ::e2::interface::i_renderpayload * p, void ** p_data ){
    void ** data = p_data;
    for( size_t count = 0; count < count_unpack; ++count ){
	assert( p );
	switch( p->_payload_type ){
	case ::e2::interface::na:
	{
	}
	break;
	case ::e2::interface::float_n:
	{
	    uint64_t ** n = ( uint64_t ** ) data;
	    if( false == p->_buf->buffer_get( p->_offset, n, 1 ) ){
		assert( "unpack float n count failed." );
		return false;
	    }
	    void ** d = ( void ** ) data;
	    ++d;
	    float ** d_n = ( float ** ) d;
	    if( false == p->_buf->buffer_get( p->_offset, d_n, **n ) ){
		assert( "unpack float n failed." );
		return false;
	    }
	}
	break;
	case ::e2::interface::mat_3:
	{
	}
	break;
	case ::e2::interface::mat_4:
	{
	}
	break;
	case ::e2::interface::quat:
	{
	}
	break;
	case ::e2::interface::int_n:
	{
	    uint64_t ** n = ( uint64_t ** ) data;
	    if( false == p->_buf->buffer_get( p->_offset, n, 1 ) ){
		assert( "unpack uint64_t n count failed." );
		return false;
	    }
	    void ** d = ( void ** ) data;
	    ++d;
	    int64_t ** d_n = ( int64_t ** ) d;
	    if( false == p->_buf->buffer_get( p->_offset, d_n, **n ) ){
		assert( "unpack uint64_t n failed." );
		return false;
	    }
	}
	break;
	case ::e2::interface::uint_n:
	{
	    uint64_t ** n = ( uint64_t ** ) data;
	    if( false == p->_buf->buffer_get( p->_offset, n, 1 ) ){
		assert( "unpack uint64_t n count failed." );
		return false;
	    }
	    void ** d = ( void ** ) data;
	    ++d;
	    uint64_t ** d_n = ( uint64_t ** ) d;
	    if( false == p->_buf->buffer_get( p->_offset, d_n, **n ) ){
		assert( "unpack uuint64_t n failed." );
		return false;
	    }
	}
	break;
	case ::e2::interface::text:
	{
	}
	break;
	}
	++p;
	++data;
    }
    return true;
}

} }
