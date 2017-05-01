#include <list>
#include <cstdint>
#include <functional>
#include <utility>
#include <cassert>

#include "free_first_fit.hpp"

namespace e2 { namespace memory {

bool free_first_fit_impl::freeing( void * p_mem_start, size_t p_mem_len, std::list<std::pair<size_t, size_t> > * mem_blocks, std::list<std::pair<size_t, size_t> > * mem_lent, void * p ){
    if( (char*)p < (char*)p_mem_start || (char*)p >= ((char*)p_mem_start + p_mem_len ) ){
	assert( 0 && "reclaiming pointer is invalid" );
	return false;
    }
    for( auto it = mem_lent->begin(), it_e = mem_lent->end(); it!=it_e; ++it ){
	for( auto it_2 = mem_blocks->begin(), it_2_e = mem_blocks->end(); it_2!=it_2_e; ++it_2 ){
	    if( it_2->first > it->first ){
		mem_blocks->splice( it_2, *mem_lent, it );
		//try coalesce adjacent block after
		if( it->first + it->second == it_2->first ){
		    it->second += it_2->second;
		    mem_blocks->erase( it_2 );
		}
		//try coalesce adjacent block before
		if( it != mem_blocks->begin() ){
		    auto it_before = it;
		    it_before--;
		    if( it_before->first + it_before->second == it->first ){
			it_before->second += it->second;
			mem_blocks->erase( it );
		    }
		}
		return true;
	    }
	}
    }
    return false;
}

} }
