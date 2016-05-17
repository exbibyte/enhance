#ifndef EN_UI0_H
#define EN_UI0_H

#include "enComponentType.h"

class enUI0 {
public:
    static enComponentType _component_type;
};

enComponentType enUI0::_component_type = enComponentType::UI;

#endif
