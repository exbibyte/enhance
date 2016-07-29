#ifndef ALLOC_STACK_H
#define ALLOC_STACK_H

#include <cstring>

#include "alloc.hpp"

class allocstack : public alloc<allocstack> {
public:
    void reserve( size_t totalsize, size_t alignment );
    void * malloc( size_t size );
};

#endif
