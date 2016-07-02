#ifndef ISTAT_H
#define ISTAT_H

#include <string>

class IStat {
public:
    virtual ~IStat(){}
    virtual char const * getstat(){ return nullptr; }
};

#endif
