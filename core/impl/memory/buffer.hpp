#ifndef E2_BUFFER_HPP
#define E2_BUFFER_HPP

#include <vector>

namespace e2 { namespace memory {

class buffer {
public:
    buffer( size_t s ){
	_buf.resize( s );
    }
    bool buffer_get( size_t const offset, void ** data ){
	if( offset >= _buf.size() ||
	    nullptr == data ){
	    return false;
	}
	*data = _buf[ offset ];
	return true;
    }
    bool buffer_set( size_t const offset, void const * data, size_t const len ){
	if( offset >= _buf.size() ||
	    offset + len >= buf.size() ||
	    nullptr == data ){
	    return false;
	}
	for( size_t i = 0; i < len; ++i ){
	    _buf[ offset + i ] = data[i];
	}
	return true;
    }
    bool buffer_resize( size_t s ){
	_buf.resize( s );
	return true;
    }
private:
    std::vector< void * > _buf;
};

} }

#endif
