#ifndef EN_COMPONENT_RENDER_H
#define EN_COMPONENT_RENDER_H

#include "enComponentSpecialize.hpp"
#include "IRender.hpp"
#include "RenderGl.hpp"

class enComponentRenderGl : public enComponentSpecialize<IRender, RenderGl > {
public:
    enComponentRenderGl( RenderGl * instance ) : enComponentSpecialize<IRender, RenderGl >( instance, enComponentType::RENDER ){}
};

#endif
