#ifndef E2_BUFFER_HPP
#define E2_BUFFER_HPP

#include <list>
#include <vector>
#include <cstring>

namespace e2 { namespace memory {

class buffer {
public:
    buffer( size_t s = 1024 ){
	std::vector< char > block( s );
	_buf.push_back( std::move( block ) );
    	// _buf.resize( s );
	_next = 0;
    }
    template< class TypeData >
    bool buffer_get( size_t const offset, TypeData ** data, size_t const num_data ){
	size_t len_bytes = sizeof(TypeData) * num_data;
    	if( _buf.empty() || offset >= _buf.back().size() ||
	    offset + len_bytes >= _buf.back().size() ||
    	    nullptr == data ){
    	    return false;
    	}
    	*data = (TypeData*)(&_buf.back()[ offset ]);
    	return true;
    }
    template< class TypeData >
    bool buffer_get_next_available( size_t * offset, TypeData ** data, size_t const num_data ){
	size_t len_bytes = sizeof(TypeData) * num_data;
    	if( _buf.empty() || _next >= _buf.back().size() ||
	    _next + len_bytes >= _buf.back().size() ||
    	    nullptr == data ||
	    nullptr == offset ){
	    //allocate new block
	    std::vector< char > block( len_bytes );
	    _buf.push_back( std::move( block ) );
	    _next = 0;
	    *data = (TypeData*)(&_buf.back()[ _next ]);
    	    return true;
    	}
    	*data = (TypeData*)(&_buf.back()[ _next ]);
	*offset = _next;
	_next += len_bytes;
    	return true;
    }
    bool buffer_resize( size_t s ){
	if( _buf.empty() )
	    return false;
    	_buf.back().resize( s );
	_next = 0;
    	return true;
    }
    bool buffer_stat_fraction_free( double * v ){
	if( nullptr == v ) return false;
	if( _buf.empty() )
	    return false;
	if( 0 == _buf.back().size() ){
	    *v = 0;
	}else{
	    *v = 1.0 - ((double) _next / _buf.back().size());
	}
    	return true;
    }
    bool buffer_stat_total_bytes( size_t * v ){
	if( nullptr == v ) return false;
	if( _buf.empty() ) return false;
	*v = _buf.back().size();
    	return true;
    }
    bool buffer_stat_all_total_bytes( size_t * v ){
	if( nullptr == v ) return false;
	if( _buf.empty() ) return false;
	*v = 0;
	for( auto & i : _buf ){
	    *v += i.size();
	}
    	return true;
    }
    bool buffer_clear_all(){
	_buf.clear();
    	return true;
    }
private:
    std::list< std::vector< char > > _buf;
    size_t _next;
};

} }

#endif
