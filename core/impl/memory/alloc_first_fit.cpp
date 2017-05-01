#include <cstring>
#include <utility>
#include <functional>
#include <list>
#include <cassert>

#include "alloc_first_fit.hpp"

namespace e2 { namespace memory {

bool alloc_first_fit_impl::allocating( void * p_mem_start, size_t p_mem_len, std::list<std::pair<size_t, size_t> > * mem_blocks, std::list<std::pair<size_t, size_t> > * mem_lent, void ** p, size_t size ){
    if( size > p_mem_len ){
	// // assert( 0 && "allocating size too big" );
	// std::bad_alloc bad_alloc;
	// throw bad_alloc;
	return false;
    }
    for( auto & i : *mem_blocks ){
	size_t mem_len = i.second;
	if( size <= mem_len ){
	    mem_lent->push_back( std::pair<size_t,size_t>( i.first, size ) );
	    i.first += size;
	    i.second -= size;
	    *p = (void*)((char*)p_mem_start + i.first);
	    return true;
	}
    }
    //no suitable free block found
    return false;
}

} }
