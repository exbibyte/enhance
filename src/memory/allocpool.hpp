#ifndef ALLOCPOOL_H
#define ALLOCPOOL_H

#define DEBUG_PRINT

#include <cstring>
#include <unordered_map>
#include <vector>

#include "alloc.hpp"

class allocpool : public alloc<allocpool> {
public:
    struct block{
	size_t _size = 0;
	void * _data = 0;
	bool _free = false;
    };
    allocpool() : _reserve(nullptr), _occupied(0), _next_block(nullptr), _reserve_size(0) {
	reserve( 4000000 );
    }
    ~allocpool(){
    }
    void * alloc( size_t n );
    void free( void * );
    void reserve( size_t n );
    void dereserve();
private:
    std::vector<block> _blocks;
    std::vector<block> _blocks_free; //TODO: free-list for memory recycling
    char * _reserve;
    size_t _occupied;
    std::unordered_map<void*, block *> _data_to_block;
    char * _next_block;
    size_t _reserve_size;
};

void * operator new( size_t n, allocpool * pool );
void operator delete( void * p, allocpool * pool );

#endif
