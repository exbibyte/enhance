#include "Renderdraw0.hpp"
#include "IRenderdraw.hpp"

#include "RenderData.h"
#include "RenderPass.h"

bool Renderdraw0::render( RenderData * renderdata ){
    RenderPass< RenderBackEndOpenGL, PassType_ShadowMap_OpGL > renderpass;
    return renderpass.Process( renderdata->_glslprogram, renderdata->_entity, renderdata->_light, renderdata->_camera, renderdata->_context );
}
