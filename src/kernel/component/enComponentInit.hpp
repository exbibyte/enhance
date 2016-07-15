#ifndef EN_COMPONENT_INIT_H
#define EN_COMPONENT_INIT_H

#include <chrono>

#include "enComponentSpecialize.hpp"
#include "IInit.hpp"
#include "InitGL.hpp"

class enComponentInitGL : public enComponentSpecialize<IInit, InitGL> {
public:
    enComponentInitGL( InitGL * instance ) : enComponentSpecialize<IInit, InitGL>( instance, enComponentType::INIT ){}
};

#endif
