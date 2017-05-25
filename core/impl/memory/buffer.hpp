#ifndef E2_BUFFER_HPP
#define E2_BUFFER_HPP

#include <vector>
#include <cstring>

namespace e2 { namespace memory {

class buffer {
public:
    buffer( size_t s = 1024 ){
    	_buf.resize( s );
	_next = 0;
	_buf_next = nullptr;
	_buf_prev = nullptr;
    }
    template< class TypeData >
    bool buffer_get( size_t const offset, TypeData ** data, size_t const num_data ){
	size_t len_bytes = sizeof(TypeData) * num_data;
    	if( offset >= _buf.size() ||
	    offset + len_bytes >= _buf.size() ||
    	    nullptr == data ){
    	    return false;
    	}
    	*data = (TypeData*)(&_buf[ offset ]);
    	return true;
    }
    template< class TypeData >
    bool buffer_get_next_available( size_t * offset, TypeData ** data, size_t const num_data ){
	size_t len_bytes = sizeof(TypeData) * num_data;
    	if( _next >= _buf.size() ||
	    _next + len_bytes >= _buf.size() ||
    	    nullptr == data ||
	    nullptr == offset ){
    	    return false;
    	}
    	*data = (TypeData*)(&_buf[ _next ]);
	*offset = _next;
	_next += len_bytes;
    	return true;
    }
    bool buffer_resize( size_t s ){
    	_buf.resize( s );
	_next = 0;
    	return true;
    }
    bool buffer_stat_fraction_free( double * v ){
	if( nullptr == v ) return false;
	if( 0 == _buf.size() ){
	    *v = 0;
	}else{
	    *v = 1.0 - ((double) _next / _buf.size());
	}
    	return true;
    }
    bool buffer_stat_total_bytes( size_t * v ){
	if( nullptr == v ) return false;
	*v = _buf.size();
    	return true;
    }
private:
    std::vector< char > _buf;
    size_t _next;
    buffer * _buf_next;
    buffer * _buf_prev;
};

} }

#endif
