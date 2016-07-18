#ifndef ALLOCBULK0_H
#define ALLOCBULK0_H

#include "IAllocbulk.hpp"

class Allocbulk0 : public IAllocbulk {
public:
    char const * get_id(){ return "allocbulk0"; }
    Allocbulk0();
    ~Allocbulk0();
};

#endif
