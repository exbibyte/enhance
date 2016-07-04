#ifndef NET0_H
#define NET0_H

#include "INet.hpp"

class Net0 : public INet {
public:
    char const * get_id(){ return "net0"; }
};

#endif
