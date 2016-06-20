#ifndef EN_COMPONENT_WORKERPOOL_H
#define EN_COMPONENT_WORKERPOOL_H

#include "enComponentSpecialize.hpp"
#include "IWorkerpool.hpp"
#include "Workerpool0.hpp"
#include "Funwrap3.hpp"

class enComponentWorkerpool0 : public enComponentSpecialize<IWorkerpool<Funwrap3>, Workerpool0> {
public:
    enComponentWorkerpool0( Workerpool0 * instance ) : enComponentSpecialize<IWorkerpool<Funwrap3>, Workerpool0>( instance, enComponentType::WORKERPOOL ){}
};

#endif
