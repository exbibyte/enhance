#ifndef EN_COMPONENT_ORIENTATIONMANIP_H
#define EN_COMPONENT_ORIENTATIONMANIP_H

#include "enComponentSpecialize.hpp"
#include "IOrientationmanip.hpp"
#include "Orientationmanip0.hpp"
#include "Quat.h"

class enComponentOrientationmanip0 : public enComponentSpecialize<IOrientationmanip<Quat,drag_coordinate>, Orientationmanip0> {
public:
    enComponentOrientationmanip0( Orientationmanip0 * instance ) : enComponentSpecialize<IOrientationmanip<Quat,drag_coordinate>, Orientationmanip0>( instance, enComponentType::ORIENTATIONMANIP ){}
};

#endif
