#ifndef ALLOCPOOL_H
#define ALLOCPOOL_H

#define DEBUG_PRINT

#include "alloc.hpp"

#include <cstring>
#include <vector>
#include <iostream>
#include <unordered_map>
#include <new>
#include <list>

class allocpool : public alloc<allocpool> {
public:
    struct block{
	size_t _size = 0;
	void * _data = 0;
	bool _free = false;
    };
    allocpool() : _reserve(nullptr), _occupied(0), _next_block(nullptr), _reserve_size(0), _chunk_size(0) {
	reserve( 250000000, 4 );
    }
    ~allocpool(){
    }
    template< typename T, typename ... Args >
    T * pool_new( Args && ... args );
    template< typename T >
    static void pool_delete( T * p );
    void * pool_alloc( size_t n );
    void pool_free( void * );
    void reserve( size_t chunk_size, size_t num_chunks );
    void dereserve();
    double get_loading();
private:
    std::vector<block> _blocks;
    std::list<block*> _blocks_nonfree;
    std::list<block*> _blocks_free; //free list
    char * _reserve;
    size_t _occupied;
    std::unordered_map<void*, std::list<block*>::iterator > _data_to_block;
    char * _next_block;
    size_t _reserve_size;
    size_t _chunk_size;
    static std::unordered_map<void*, allocpool*> _map_allocpools;
};

void * operator new( size_t n, allocpool * pool );
void operator delete( void * p, allocpool * pool );

#include "allocpool.tpp"

#endif
