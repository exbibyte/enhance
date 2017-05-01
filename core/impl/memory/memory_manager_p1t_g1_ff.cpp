#include <vector>

#include "memory_manager_p1t_g1_ff.hpp"
#include "memory_common.hpp"

namespace e2 { namespace memory {

memory_manager_p1t_g1_ff_impl::memory_manager_p1t_g1_ff_impl(){
    size_t s = (size_t) 1 << 16;
    _lookup_ptr.resize( ((size_t) 1 << 20)*sizeof(void*), nullptr ); //about 1E6 of trackable pointers
    _lookup_index = 0;
    _partition.internal_resize( s, &_lookup_ptr ); //65kB
}
memory_manager_p1t_g1_ff_impl::~memory_manager_p1t_g1_ff_impl(){
}

bool memory_manager_p1t_g1_ff_impl::find_next_lookup_slot( void *** p ){
    if( nullptr == _lookup_ptr[ _lookup_index ] ){
	*p = &_lookup_ptr[ _lookup_index ];
	return true;
    }
    size_t index_prev = _lookup_index;
    ++_lookup_index;
    while( _lookup_index != index_prev ){
	if( nullptr == _lookup_ptr[ _lookup_index ] ){
	    *p = &_lookup_ptr[ _lookup_index ];
	    return true;
	}
	++_lookup_index;
	if( _lookup_index >= _lookup_ptr.size() )
	    _lookup_index = 0;
    }
    return false;
}

} }
