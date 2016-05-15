#ifndef EN_RENDER_PASS_H
#define EN_RENDER_PASS_H

#include "RenderBackend.h"
#include "PassType_ShadowMap_OpGL.h"
#include "RenderPass.h"
#include "enComponentType.h"

class enRenderPass_ShadowMap_OpGL : public RenderPass< RenderBackEndOpenGL, PassType_ShadowMap_OpGL > {
public:
    static enComponentType _component_type;
};

enComponentType enRenderPass_ShadowMap_OpGL::_component_type = enComponentType::RENDER;

#endif
