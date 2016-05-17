#ifndef EN_CORE0_H
#define EN_CORE0_H

#include "enEngineCoreAbstract.h"
#include "enComponentType.h"

class enCore0 : public enEngineCoreAbstract {
public:
    static enComponentType _component_type;
};

enComponentType enCore0::_component_type = enComponentType::CORE;

#endif
