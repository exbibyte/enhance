#ifndef INITGL_H
#define INITGL_H

#include "IInit.hpp"

class InitGL : public IInit {
public:
    char const * get_id(){ return "initGL"; }
    bool init();
};

#endif
