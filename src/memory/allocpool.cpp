#include "allocpool.hpp"

#include <cstring>
#include <iostream>
#include <unordered_map>
#include <vector>
#include <new>

using namespace std;

void * allocpool::alloc( size_t n ){
#ifdef DEBUG_PRINT
    cout << "custom allocation( " << n << " bytes)." << endl;
#endif
    size_t num_bytes = n/sizeof(char);
    if( _occupied + num_bytes > _reserve_size ){
	cout << "custom allocation out of memory." << endl;
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
    _next_block += n;
    _occupied += num_bytes;
    return placement;
}

void allocpool::free( void * p ){
    auto it = _data_to_block.find(p);
    if( it == _data_to_block.end() ){
#ifdef DEBUG_PRINT
	cout << "custom deallocation error( cannot map pointer )." << endl;
#endif
    }else{
	block * b = it->second;
	size_t n = b->_size;
	b->_free = true;
#ifdef DEBUG_PRINT
	cout << "custom deallocation( " << n << " bytes)." << endl;
#endif
    }
}

void allocpool::reserve( size_t n ){
    _reserve = new char[n];
    _next_block = _reserve;
    _reserve_size = n;
}

void allocpool::dereserve(){
    if( _reserve ){
	delete [] _reserve;
	_reserve = nullptr;
	_next_block = nullptr;
    }
}

void * operator new( size_t n, allocpool * pool ){
    return pool->alloc(n);
}

void operator delete( void * p, allocpool * pool ){
    return pool->free(p);
}
