#include "Renderdraw0.hpp"
#include "IRenderdraw.hpp"

#include "RenderData.hpp"
#include "RenderPass.h"
#include "PassType_ShadowMap_OpGL.h"

#include <iostream>

bool Renderdraw0::render( RenderData renderdata ){

    std::cout << "Renderdraw0::render invoked." << std::endl;
    
    RenderPass< RenderBackEndOpenGL, PassType_ShadowMap_OpGL > renderpass;

    return renderpass.Process( renderdata._glslprogram, renderdata._entities, renderdata._light, renderdata._camera, renderdata._context );
    
    // return true;
}
