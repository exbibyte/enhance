#ifndef EN_COMPONENT_RENDERCOMPUTE_H
#define EN_COMPONENT_RENDERCOMPUTE_H

#include "enComponentSpecialize.hpp"
#include "IRendercompute.hpp"
#include "Rendercompute0.hpp"

class enComponentRendercompute0 : public enComponentSpecialize<IRendercompute, Rendercompute0 > {
public:
    enComponentRendercompute0( Rendercompute0 * instance ) : enComponentSpecialize<IRendercompute, Rendercompute0 >( instance, enComponentType::RENDERCOMPUTE ){}
};

#endif
