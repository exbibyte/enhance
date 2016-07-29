#ifndef ALLOC_STACK_H
#define ALLOC_STACK_H

#include "alloc.hpp"

class allocstack : public alloc<allocstack> {
public:
    void reserve( size_t totalsize );
};

#endif
