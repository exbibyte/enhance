#ifndef EN_COMPONENT_RENDERCOMPUTE_H
#define EN_COMPONENT_RENDERCOMPUTE_H

#include "enComponentSpecialize.hpp"
#include "IRenderCompute.hpp"
#include "RenderComputeShadowmap.hpp"

class enComponentRenderComputeShadowmap : public enComponentSpecialize<IRenderCompute, RenderComputeShadowmap > {
public:
    enComponentRenderComputeShadowmap( RenderComputeShadowmap * instance ) : enComponentSpecialize<IRenderCompute, RenderComputeShadowmap >( instance, enComponentType::RENDERCOMPUTE ){}
};

#endif
