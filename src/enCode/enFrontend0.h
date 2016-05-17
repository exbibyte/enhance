#ifndef EN_FRONTEND0_H
#define EN_FRONTEND0_H

#include "enComponentType.h"

class enFrontend0 {
public:
    static enComponentType _component_type;
};

enComponentType enFrontend0::_component_type = enComponentType::FRONTEND;

#endif
