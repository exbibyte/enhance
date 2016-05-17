#ifndef EN_MAIN0_H
#define EN_MAIN0_H

#include "enComponentType.h"

class enMain0{
public:
    static enComponentType _component_type;
};

enComponentType enMain0::_component_type = enComponentType::MAIN;

#endif
