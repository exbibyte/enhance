#ifndef STAT0_H
#define STAT0_H

#include "IStat.hpp"

class Stat0 : public IStat {
public:
    char const * get_id(){ return "stat0"; }
    std::string const getstat();
};

#endif
