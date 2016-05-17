#ifndef EN_LOG0_H
#define EN_LOG0_H

#include "enComponentType.h"

class enLog0{
public:
    static enComponentType _component_type;
};

enComponentType enLog0::_component_type = enComponentType::LOG;

#endif
