#ifndef EN_BACKEND0_H
#define EN_BACKEND0_H

#include "enComponentType.h"

class enBackend0 {
public:
    static enComponentType _component_type;
};

enComponentType enBackend0::_component_type = enComponentType::BACKEND;

#endif
