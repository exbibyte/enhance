#include <cstring>
#include <utility>
#include <functional>
#include <list>
#include <cassert>

#include "alloc_first_fit.hpp"
#include "memory_common.hpp"

namespace e2 { namespace memory {

bool alloc_first_fit_impl::allocating( void * p_mem_start, size_t p_mem_len, std::list< memory_block_info > * mem_blocks, std::list< memory_block_info > * mem_lent, void ** p, size_t size ){
    if( size > p_mem_len ){
	// // assert( 0 && "allocating size too big" );
	// std::bad_alloc bad_alloc;
	// throw bad_alloc;
	return false;
    }
    for( auto & i : *mem_blocks ){
	size_t mem_len = i._len;
	if( size <= mem_len ){

	    memory_block_info block_info;
	    block_info._offset = i._offset;
	    block_info._len = size;
	    block_info._lookup_ptr = p;

	    mem_lent->push_back( block_info );
	    
	    i._offset += size;
	    i._len -= size;

	    *p = (void*)((char*)p_mem_start + block_info._offset);
	    return true;
	}
    }
    //no suitable free block found
    return false;
}

} }
