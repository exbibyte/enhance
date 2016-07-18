#ifndef IINIT_H
#define IINIT_H

#include <string>

class IInit {
public:
    virtual ~IInit(){}
    virtual bool init(){ return false; }
};

#endif
