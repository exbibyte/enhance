#ifndef EN_COMPONENT_CAM_HPP
#define EN_COMPONENT_CAM_HPP

#include "enComponentSpecialize.hpp"
#include "ICam.hpp"
#include "cam0.hpp"

class enComponentCam0 : public enComponentSpecialize<ICam, cam0> {
public:
    enComponentCam0( cam0 * instance ) : enComponentSpecialize<ICam, cam0>( instance, enComponentType::CAM ){}
};

#endif
