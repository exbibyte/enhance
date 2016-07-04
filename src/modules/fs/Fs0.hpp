#ifndef FS0_H
#define FS0_H

#include "IFs.hpp"

class Fs0 : public IFs {
public:
    char const * get_id(){ return "fs0"; }
    Fs0();
    ~Fs0();
};

#endif
