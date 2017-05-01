#include <list>
#include <cstdint>
#include <functional>
#include <utility>
#include <cassert>

#include "free_first_fit.hpp"
#include "memory_common.hpp"

namespace e2 { namespace memory {

bool free_first_fit_impl::freeing( void * p_mem_start, size_t p_mem_len, std::list< memory_block_info > * mem_blocks, std::list< memory_block_info > * mem_lent, void * p ){
    if( (char*)p < (char*)p_mem_start || (char*)p >= ((char*)p_mem_start + p_mem_len ) ){
	assert( 0 && "reclaiming pointer is invalid" );
	return false;
    }

    for( auto it = mem_lent->begin(), it_e = mem_lent->end(); it!=it_e; ++it ){
	void * a = (void *)( (char *)p_mem_start + it->_offset );
	if( a == p ){ //found candidate ptr to recycle
	    for( auto it_2 = mem_blocks->begin(), it_2_e = mem_blocks->end(); it_2!=it_2_e; ++it_2 ){
		if( it_2->_offset > it->_offset ){
		    mem_blocks->splice( it_2, *mem_lent, it );
		    //try coalesce adjacent block after
		    if( it->_offset + it->_len == it_2->_offset ){
			it->_len += it_2->_len;
			mem_blocks->erase( it_2 );
		    }
		    //try coalesce adjacent block before
		    if( it != mem_blocks->begin() ){
			auto it_before = it;
			it_before--;
			if( it_before->_offset + it_before->_len == it->_offset ){
			    it_before->_len += it->_len;
			    mem_blocks->erase( it );
			}
		    }
		    //reset lookup ptr
		    *(it->_lookup_ptr) = nullptr;
		    return true;
		}
	    }
	    //insert at the end
	    mem_blocks->splice( mem_blocks->end(), *mem_lent, it );
	    //try coalesce adjacent block before
	    if( it != mem_blocks->begin() ){
		auto it_before = it;
		it_before--;
		if( it_before->_offset + it_before->_len == it->_offset ){
		    it_before->_len += it->_len;
		    mem_blocks->erase( it );
		}
	    }
	    //reset lookup ptr
	    *(it->_lookup_ptr) = nullptr;
	    return true;
	}
    }
    return false;
}

} }
