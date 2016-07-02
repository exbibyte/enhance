#ifndef EN_COMPONENT_CLOCK_H
#define EN_COMPONENT_CLOCK_H

#include <chrono>

#include "enComponentSpecialize.hpp"
#include "IClock.hpp"
#include "Clock0.hpp"

class enComponentClock0 : public enComponentSpecialize<IClock, Clock0> {
public:
    enComponentClock0( Clock0 * instance ) : enComponentSpecialize<IClock, Clock0>( instance, enComponentType::CLOCK ){}
};

#endif
