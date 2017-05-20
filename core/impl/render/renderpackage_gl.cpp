#include <cstdint>
#include <cassert>

#include "i_renderpayload.hpp"
#include "i_renderpackage.hpp"
#include "renderpayload_gl.hpp"
#include "renderpackage_gl.hpp"
#include "buffer.hpp"

namespace e2 { namespace render {

bool renderpackage_gl::unpack( ::e2::interface::i_renderpackage * pkg, void ** p_data ){
    if( nullptr == pkg ){
	assert( false && "resource package invalid." );
	return false;
    }
    void ** data = p_data;

    //loop for every resource    
    for( size_t count_resource = 0; count_resource < pkg->_num_resource; ++count_resource ){
	::e2::interface::i_renderresource * resource = pkg->_resource + count_resource;
	if( nullptr == resource ){
	    assert( false && "resource invalid" );
	    return false;
	}
	if( nullptr == data ){
	    assert( false && "data invalid" );
	    return false;
	}
	if( true == resource->_empty_payload ){
	    continue;
	}
	::e2::interface::i_renderpayload * payload = pkg->_payload + resource->_offset_payload;
	if( nullptr == payload ){
	    assert( false && "resource payload invalid" );
	    return false;
	}
	//loop for every payload for current resource
	for( size_t count_payload = 0; count_payload < pkg->_num_payload; ++count_payload ){
	    if( nullptr == payload ){
		assert( false && "resource payload invalid" );
		return false;
	    }
	    switch( payload->_payload_type ){
	    case ::e2::interface::na:
	    {
	    }
	    break;
	    case ::e2::interface::float_n:
	    {
		uint64_t ** n = ( uint64_t ** ) data;
		if( false == payload->_buf->buffer_get( payload->_offset, n, 1 ) ){
		    assert( "unpack float n count failed." );
		    return false;
		}
		void ** d = ( void ** ) data;
		++d;
		float ** d_n = ( float ** ) d;
		if( false == payload->_buf->buffer_get( payload->_offset + sizeof(void*), d_n, **n ) ){
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
		if( false == payload->_buf->buffer_get( payload->_offset, n, 1 ) ){
		    assert( "unpack uint64_t n count failed." );
		    return false;
		}
		void ** d = ( void ** ) data;
		++d;
		int64_t ** d_n = ( int64_t ** ) d;
		if( false == payload->_buf->buffer_get( payload->_offset + sizeof(void*), d_n, **n ) ){
		    assert( "unpack uint64_t n failed." );
		    return false;
		}
	    }
	    break;
	    case ::e2::interface::uint_n:
	    {
		uint64_t ** n = ( uint64_t ** ) data;
		if( false == payload->_buf->buffer_get( payload->_offset, n, 1 ) ){
		    assert( "unpack uint64_t n count failed." );
		    return false;
		}
		void ** d = ( void ** ) data;
		++d;
		uint64_t ** d_n = ( uint64_t ** ) d;
		if( false == payload->_buf->buffer_get( payload->_offset + sizeof(void*), d_n, **n ) ){
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
	    ++payload;
	    ++data;	    
	}
    }
    return true;
}

} }
