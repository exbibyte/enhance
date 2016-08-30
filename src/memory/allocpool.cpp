#include "allocpool.hpp"

#include <unordered_map>
#include <iostream>
#include <cstdlib>

//static member
std::unordered_map<void*, allocpool*> allocpool::_map_allocpools{};

allocpool::allocpool(){
    _reserve = nullptr;
    _occupied = 0;
    _next_block = nullptr;
    _reserve_size = 0;
    _chunk_size = 0;
    reserve( 250000000, 4 );
}

void * allocpool::pool_alloc( size_t n ){
#ifdef DEBUG_PRINT
    std::cout << "custom allocation( " << n << " bytes)." << std::endl;
#endif
    size_t num_bytes = n/sizeof(char);
    if( num_bytes > _chunk_size ){
	std::cout << "custom allocation chunk size exceeds maximuim of " << _chunk_size << "." << std::endl;
        std::bad_alloc bad_alloc;
	throw bad_alloc;
	return nullptr;
    }

    //try to obtain from free list
    auto it = _blocks_free.begin();
    if( it == _blocks_free.end() ){
	std::cout << "custom allocation out of memory." << _chunk_size << std::endl;
        std::bad_alloc bad_alloc;
	throw bad_alloc;
	return nullptr;
    }
    block * b = *it;
    void * placement = b->_data;
    b->_free = false;
    _occupied += b->_size;
    //save association of assigned memory address to this allocpool
    _map_allocpools[placement] = this;
    //transfer memory block from free to non-free list
    _blocks_nonfree.splice( _blocks_nonfree.begin(), _blocks_free, it );
    //save association of assigned memory address to memory block metainfo
    _data_to_block[placement] = it;
    return placement;
}

void allocpool::pool_free( void * p ){
    auto it_pool = _map_allocpools.find(p);
    if( it_pool == _map_allocpools.end() ){
#ifdef DEBUG_PRINT
	std::cout << "custom deallocation error( cannot map pointer to pool )." << std::endl;
#endif
    }else{
	allocpool * pool = it_pool->second;
        auto it_block = pool->_data_to_block.find(p);
	if( it_block == pool->_data_to_block.end() ){
#ifdef DEBUG_PRINT
	    std::cout << "custom deallocation error( cannot map pointer to memory block metainfo )." << std::endl;
#endif
	}
	block * b = *(it_block->second);
	size_t n = b->_size;
	_occupied -= n;
	b->_free = true;
	//transfer from nonfree list to free list
	_blocks_free.splice( _blocks_free.begin(), _blocks_nonfree, it_block->second );
	_map_allocpools.erase(p);
	_data_to_block.erase(p);
#ifdef DEBUG_PRINT
	std::cout << "custom deallocation( " << n << " bytes)." << std::endl;
#endif
    }
}

void allocpool::reserve( size_t size_chunk, size_t num_chunks ){
    if( !_reserve ){
	// _reserve = new char[ size_chunk * num_chunks ];
	_reserve_size = size_chunk * num_chunks;
	_reserve = (char*)malloc( _reserve_size );
	std::cout << "reserved: " << _reserve_size << " bytes." << std::endl;
	_chunk_size = size_chunk;
	//add to free list
	for( size_t i = 0; i < num_chunks; ++i ){
	    block b;
	    b._size = _chunk_size;
	    b._data = _reserve + (_chunk_size * i);
	    b._free = true;
	    _blocks.push_back( b );
	}
	for( auto & i : _blocks ){
	    _blocks_free.push_back( &i );
	}
    }
}

void allocpool::dereserve(){
    //return memory to system
    if( _reserve ){
	// delete [] _reserve;
        ::free( _reserve );
	_reserve = nullptr;
    }
}

double allocpool::get_loading(){
    return _occupied / (double) _reserve_size;
}

void * operator new( size_t n, allocpool * pool ){
    return pool->pool_alloc(n);
}

void operator delete( void * p, allocpool * pool ){
    return pool->pool_free(p);
}
