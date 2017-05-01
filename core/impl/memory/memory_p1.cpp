#include <list>
#include <cstdint>
#include <utility>
#include <cstring>
#include <cassert>
#include <cstdlib>
#include <vector>

#include "memory_p1.hpp"

namespace e2 { namespace memory {

memory_p1_impl::~memory_p1_impl(){
    //block wait until lent block have returned
    internal_free();
    while ( _mem_lent.size() > 0 || nullptr != _mem_start ){
	internal_free();
    }
}

size_t memory_p1_impl::stat_size_total(){
    return _mem_len;
}

size_t memory_p1_impl::stat_free_size_total(){
    size_t total = 0;
    for( auto it = _mem_blocks.begin(), it_e = _mem_blocks.end(); it!=it_e; ++it ){
	total += it->_len;
    }
    return total;
}

double memory_p1_impl::stat_free_size_mean(){
    size_t n = stat_free_count_blocks();
    if( n == 0 )
	return 0;
    return (double) stat_free_size_total() / stat_free_count_blocks();
}

size_t memory_p1_impl::stat_free_count_blocks(){
    size_t s = _mem_blocks.size();
    return s;
}

size_t memory_p1_impl::stat_lent_size_total(){
    size_t total = 0;
    for( auto it = _mem_lent.begin(), it_e = _mem_lent.end(); it!=it_e; ++it ){
	total += it->_len;
    }
    return total;
}

double memory_p1_impl::stat_lent_size_mean(){
    size_t n = _mem_lent.size();
    if( n == 0 ){
	return 0;
    }
    return (double) stat_lent_size_total() / n;
}

size_t memory_p1_impl::stat_lent_count_blocks(){
    return _mem_lent.size();
}

double memory_p1_impl::stat_free_fraction(){
    if( _mem_len == 0 )
	return 0.0;
    return (double) stat_free_size_total() / _mem_len;
}

bool memory_p1_impl::internal_resize( size_t len_resize, std::vector<void*> * lookup_ptr ){
    if( len_resize < _mem_len ){
	// assert( 0 && "resize target block too small." );
	return false;
    }
    try{
	if( nullptr == _mem_start )
	{
	    _mem_start = malloc( len_resize );
	    if( nullptr == _mem_start ){
		assert( 0 && "internal allocating from system unsuccessful.");
	    }
	    _mem_len = len_resize;
	    _mem_blocks.clear();
	    _mem_blocks.push_back( { 0, len_resize } );
	}else{
	    void * mem_resize = malloc( len_resize );
	    char * p_dst = static_cast<char*>( mem_resize );

	    //compaction into newly allocating memory region
	    size_t new_offset = 0;
	    for( auto & i : _mem_lent ){ //per lent object
		char * p_src = (char *)( (char *) _mem_start + i._offset );
		i._offset = new_offset; //save new offset
		*(i._lookup_ptr) = (void*)p_dst; //save new ptr lookup
		for( size_t j = 0; j < i._len; ++j ){ //copy
		    *p_dst++ = *p_src++;
		}
		new_offset += i._len;
	    }
	    free( _mem_start );
	    _mem_len = len_resize;
	    _mem_start = mem_resize;
	    _mem_blocks.clear();
	    _mem_blocks.push_back( { new_offset, len_resize - new_offset } );
	}
    }catch(...){
	assert( "resizing memory unsuccessful." );
	return false;
    }
    return true;
}

bool memory_p1_impl::internal_free(){
    if( _mem_lent.size() > 0 ){
	//some block(s) still lent out
	return false;
    }
    try{
	if( nullptr != _mem_start ){
	    free( _mem_start );
	    _mem_start = nullptr;
	}
	_mem_len = 0;
	_mem_blocks.clear();
	_mem_lent.clear();
    }catch(...){
	assert( 0 && "clear_internal unsucessful." );
	return false;
    }
    return true;
}

} }
