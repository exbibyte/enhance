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
               allocpool();
               ~allocpool(){}
    template< typename T, typename ... Args >
            T *  pool_new( Args && ... args );
    template< typename T >
    static void pool_delete( T * p );
         void * pool_alloc( size_t n );
           void pool_free( void * );
           void reserve( size_t chunk_size, size_t num_chunks );
           void dereserve();
         double get_loading();
private:
    using type_map_datablock = std::unordered_map<void*, std::list<block*>::iterator >;
    using type_map_allocpool = std::unordered_map<void*, allocpool*>;
    std::vector<block> _blocks;
     std::list<block*> _blocks_nonfree;
     std::list<block*> _blocks_free; //free list
                char * _reserve;
                size_t _occupied;
    type_map_datablock _data_to_block;
                char * _next_block;
                size_t _reserve_size;
                size_t _chunk_size;
    static type_map_allocpool _map_allocpools;
};

void * operator new( size_t n, allocpool * pool );
void operator delete( void * p, allocpool * pool );

#include "allocpool.tpp"

#endif
