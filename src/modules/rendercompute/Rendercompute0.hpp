#ifndef RENDERCOMPUTE0_H
#define RENDERCOMPUTE0_H

#include "IRendercompute.hpp"

class Rendercompute0 : public IRendercompute {
public:
    char const * get_id(){ return "rendercompute0"; }
};

#endif
