#ifndef ISTAT_H
#define ISTAT_H

#include <string>

class IStat {
public:
    virtual ~IStat(){}
    virtual std::string const getstat(){ return ""; }
};

#endif
