#ifndef EN_RENDER_PASS_H
#define EN_RENDER_PASS_H

#include "RenderBackend.h"
#include "PassType_ShadowMap_OpGL.h"
#include "RenderPass.h"

class enRenderPass_ShadowMap_OpGL : public RenderPass< RenderBackEndOpenGL, PassType_ShadowMap_OpGL > {};

#endif
