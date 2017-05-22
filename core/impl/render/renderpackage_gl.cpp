#include <cstdint>
#include <cassert>
#include <map>

#include "i_renderpayload.hpp"
#include "i_renderpackage.hpp"
#include "renderpayload_gl.hpp"
#include "renderpackage_gl.hpp"
#include "buffer.hpp"

namespace e2 { namespace render {

bool renderpackage_gl::pack( ::e2::memory::buffer * b, ::e2::interface::i_renderpackage ** package, uint64_t * renderresource_key, void ** renderresource_data, size_t count ){
    assert( b );
    assert( package );
    size_t offset;
    ::e2::interface::i_renderpackage * pkg;
    ::e2::interface::i_renderpayload * p;
    uint64_t * k;
    void ** d;
    //allocate from memory buffer
    assert( b->buffer_get_next_available( &offset, &pkg, 1 ) );
    if( 0 < count ){
	assert( renderresource_key );
	assert( renderresource_data );
	assert( b->buffer_get_next_available( &offset, &p, 1 ) );
	assert( b->buffer_get_next_available( &offset, &k, count ) );
	assert( b->buffer_get_next_available( &offset, &d, count ) );
	pkg->_payload = p;
	p->_count = count;
	p->_key = k;
	p->_data = d;
	for( size_t i = 0; i < count; ++i ){
	    *k++ = *renderresource_key++;
	    int * test = (int*)*renderresource_data;
	    *d++ = *renderresource_data++;
	}
    }else{
	pkg->_payload = nullptr;
    }
    *package = pkg;

    return true;
}
bool renderpackage_gl::unpack( ::e2::interface::i_renderpackage * pkg, ::e2::interface::i_package_filter * filter ){
    assert( pkg );
    assert( filter );
    //loop for every resource
    if( nullptr != pkg->_payload ){
	uint64_t * key = pkg->_payload->_key;
	void ** data = pkg->_payload->_data;
	for( size_t i = 0; i < pkg->_payload->_count; ++i ){
	    assert( key );
	    assert( data );
	    //store found data
	    auto it = filter->_lookup.find( *key );
	    if( filter->_lookup.end() != it ){
		it->second.first = true;
		it->second.second = *data;
	    }
	    ++key;
	    ++data;
	}
    }
    for( auto it = filter->_lookup.begin(); it != filter->_lookup.end(); ++it ){
	if( false == it->second.first ){
	    return false;
	}
    }
    return true;
}

} }
