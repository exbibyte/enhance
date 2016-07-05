#ifndef CRYPT0_H
#define CRYPT0_H

#include "ICrypt.hpp"

class Crypt0 : public ICrypt {
public:
    char const * get_id(){ return "fs0"; }
    Crypt0();
    ~Crypt0();
};

#endif
