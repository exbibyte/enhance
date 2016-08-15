#ifndef ALLOCPOOL_H
#define ALLOCPOOL_H

#define DEBUG_PRINT

#include <cstring>
#include <vector>
#include <iostream>
#include <unordered_map>
#include <new>

#include "alloc.hpp"

class allocpool : public alloc<allocpool> {
public:
    struct block{
	size_t _size = 0;
	void * _data = 0;
	bool _free = false;
    };
    allocpool() : _reserve(nullptr), _occupied(0), _next_block(nullptr), _reserve_size(0) {
	reserve( 1'000'000'000 );
    }
    ~allocpool(){
    }
    template< typename T, typename ... Args >
    T * pool_new( Args && ... args );
    template< typename T >
    static void pool_delete( T * p );
    void * alloc( size_t n );
    void free( void * );
    void reserve( size_t n );
    void dereserve();
    double get_loading();
private:
    std::vector<block> _blocks;
    std::vector<block> _blocks_free; //TODO: free-list for memory recycling
    char * _reserve;
    size_t _occupied;
    std::unordered_map<void*, block *> _data_to_block;
    char * _next_block;
    size_t _reserve_size;
    static std::unordered_map<void*, allocpool*> _map_allocpools;
};

void * operator new( size_t n, allocpool * pool );
void operator delete( void * p, allocpool * pool );

#include "allocpool.tpp"

#endif
