#include <cstring>
#include <utility>
#include <functional>
#include <list>
#include <cassert>

#include "alloc_single_thread_first_fit.hpp"

alloc_single_thread_first_fit_impl::alloc_single_thread_first_fit_impl( size_t block_size ){
    _p = malloc( block_size );
    _size = block_size;
    _alloc_info.push_back( std::pair<size_t,size_t>(0,_size) );
}

alloc_single_thread_first_fit_impl::alloc_single_thread_first_fit_impl( void * p, size_t size ){
    _p = p;
    _size = size;
}
alloc_single_thread_first_fit_impl::~alloc_single_thread_first_fit_impl(){
    clear_internal();
}
bool alloc_single_thread_first_fit_impl::allocating( void ** p, size_t size, bool zeroed ){
    if( size > _size ){
	assert( 0 && "allocating size too big" );
	return false;
    }
    for( auto & i : _alloc_info ){
	size_t remain_size = i.second;
	if( size <= remain_size ){
	    _lent_info.push_back( std::pair<size_t,size_t>( i.first, size ) );
	    i.first += size;
	    i.second -= size;
	    *p = (void*)((char*)_p + i.first);
	    if( zeroed )
		memset( *p, 0, size );
	    return true;
	}
    }
    //no suitable free block found
    return false;
}
bool alloc_single_thread_first_fit_impl::freeing( void * p ){
    if( (char*)p < (char*)_p || (char*)p >= ((char*)_p + _size) ){
	assert( 0 && "reclaiming pointer is invalid" );
	return false;
    }
    size_t offset = (char*)p - (char*)_p;    
    for( auto it = _lent_info.begin(), it_e = _lent_info.end(); it!=it_e; ++it ){
	for( auto it_2 = _alloc_info.begin(), it_2_e = _alloc_info.end(); it_2!=it_2_e; ++it_2 ){
	    if( it_2->first > it->first ){
		_alloc_info.splice( it_2, _lent_info, it );
		//try coalesce adjacent blocks in free list
		if( it->first + it->second == it_2->first ){
		    it->second += it_2->second;
		    _alloc_info.erase( it_2 );
		}
		return true;
	    }
	}
    }
    return false;
}
size_t alloc_single_thread_first_fit_impl::stat_free_size_total(){
    size_t total = 0;
    for( auto it_2 = _alloc_info.begin(), it_2_e = _alloc_info.end(); it_2!=it_2_e; ++it_2 ){
	total += it_2->second;
    }
    return total;
}
size_t alloc_single_thread_first_fit_impl::stat_free_size_largest(){
    size_t largest = 0;
    for( auto it_2 = _alloc_info.begin(), it_2_e = _alloc_info.end(); it_2!=it_2_e; ++it_2 ){
	if( largest < it_2->second ){
	    largest = it_2->second;
	}
    }
    return largest;
}
double alloc_single_thread_first_fit_impl::stat_free_size_mean(){
    return (double) stat_free_size_total() / stat_free_count_blocks();
}
size_t alloc_single_thread_first_fit_impl::stat_free_count_blocks(){
    size_t s = _alloc_info.size();
    assert( s > 0 && "free list size invalid." );
    return s;
}
bool alloc_single_thread_first_fit_impl::resize_internal( void * p, size_t size ){
    if( size < _size ){
	assert( 0 && "resize target block too small." );
	return false;
    }
    if( !p ){
	assert( 0 && "target pointer invalid." );
	return false;
    }
    char * p_char = (char*)p;
    char * _p_char = (char*)_p;
    try{
	for( size_t i = 0; i < _size; ++i ){
	    *p_char++ = *_p_char++;
	}
	for( size_t i = 0, remain = size - _size; i < remain; ++i ){
	    *p_char++ = 0;
	}
	free( _p );
	_size = size;
    }catch(...){
	assert( 0 && "resize_internal failed." );
	return false;
    }
    return true;
}
bool alloc_single_thread_first_fit_impl::resize_internal( size_t size ){
    if( size < _size ){
	assert( 0 && "resize target block too small." );
    }
    try{
	void * p = malloc( size );
	if( !p ){
	    assert( 0 && "malloc failed in resize_internal" );
	    return false;
	}
	return resize_internal( p, size );
    }catch(...){
	assert( 0 && "resize_internal failed." );
	return false;
    }
    return true;
}
bool alloc_single_thread_first_fit_impl::clear_internal(){
    if( _lent_info.size() > 0 ){
	//some block(s) still lent out
	return false;
    }
    try{
	if( _p ){
	    free( _p );
	    _p = 0;
	}
	_size = 0;
    }catch(...){
	assert( 0 && "clear_internal failed." );
	return false;
    }
    return true;
}
