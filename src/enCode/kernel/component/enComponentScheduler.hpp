#ifndef EN_COMPONENT_SCHEDULER_H
#define EN_COMPONENT_SCHEDULER_H

#include <chrono>

#include "enComponentSpecialize.hpp"
#include "IScheduler.hpp"
#include "Scheduler0.hpp"
#include "Funwrap3.hpp"

class enComponentScheduler0 : public enComponentSpecialize<IScheduler<Funwrap3>, Scheduler0> {
public:
    enComponentScheduler0( Scheduler0 * instance ) : enComponentSpecialize<IScheduler<Funwrap3>, Scheduler0>( instance, enComponentType::SCHEDULER ){}
};

#endif
