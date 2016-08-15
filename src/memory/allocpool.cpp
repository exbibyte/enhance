#include "allocpool.hpp"

#include <unordered_map>
#include <iostream>
#include <cstdlib>

//static member
std::unordered_map<void*, allocpool*> allocpool::_map_allocpools{};

void * allocpool::alloc( size_t n ){
#ifdef DEBUG_PRINT
    std::cout << "custom allocation( " << n << " bytes)." << std::endl;
#endif
    size_t num_bytes = n/sizeof(char);
    if( _occupied + num_bytes > _reserve_size ){
	std::cout << "custom allocation out of memory." << std::endl;
        std::bad_alloc bad_alloc;
	throw bad_alloc;
	return nullptr;
    }

    block b;
    b._size = num_bytes;
    b._data = _next_block;
    _blocks.push_back(b);

    void * placement = _next_block;
    _data_to_block[(void*)_next_block] = &_blocks.back();
    _map_allocpools[(void*)_next_block] = this;
    _next_block += n;
    _occupied += num_bytes;
    return placement;
}

void allocpool::free( void * p ){
    auto it = _data_to_block.find(p);
    if( it == _data_to_block.end() ){
#ifdef DEBUG_PRINT
	std::cout << "custom deallocation error( cannot map pointer )." << std::endl;
#endif
    }else{
	block * b = it->second;
	size_t n = b->_size;
	b->_free = true;
	_occupied -= n;
#ifdef DEBUG_PRINT
	std::cout << "custom deallocation( " << n << " bytes)." << std::endl;
#endif
    }
}

void allocpool::reserve( size_t n ){
    if( !_reserve ){
	// _reserve = new char[n];
	_reserve = (char*)malloc(n);
	std::cout << "reserved: " << n << " bytes." << std::endl;
	_next_block = _reserve;
	_reserve_size = n;
    }
}

void allocpool::dereserve(){
    //return memory to system
    if( _reserve ){
	// delete [] _reserve;
        free( _reserve );
	_reserve = nullptr;
	_next_block = nullptr;
    }
}

double allocpool::get_loading(){
    return _occupied / (double) _reserve_size;
}

void * operator new( size_t n, allocpool * pool ){
    return pool->alloc(n);
}

void operator delete( void * p, allocpool * pool ){
    return pool->free(p);
}
