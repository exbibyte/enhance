#ifndef EN_COMPONENT_THREAD_H
#define EN_COMPONENT_THREAD_H

#include "enComponentSpecialize.hpp"
#include "IThread.hpp"
#include "Thread0.hpp"

class enComponentThread0 : public enComponentSpecialize<IThread, Thread0> {
public:
    enComponentThread0( Thread0 * instance ) : enComponentSpecialize<IThread, Thread0>( instance, enComponentType::THREAD ){}
};

#endif
