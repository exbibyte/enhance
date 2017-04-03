#ifndef INSTANCE_HPP
#define INSTANCE_HPP

#include <list>
#include <cstdint>

#include "orientprop.hpp"

class instance_info {
    uint64_t  _id;
};

class instances {
public:
    std::list< std::pair< instance_info, orientprop > > _instances;
    uint64_t _type;
};

#endif
