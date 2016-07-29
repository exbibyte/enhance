#ifndef ALLOC_POOL_H
#define ALLOC_POOL_H

#include "alloc.hpp"

class allocpool : public alloc<allocpool> {
public:
    void reserve( size_t obj_size, size_t n );
};

#endif
