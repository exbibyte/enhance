#include "Renderdraw1.hpp"
#include "IRenderdraw.hpp"

#include "RenderData.hpp"
#include "RenderPass.h"
#include "PassType_NoShadow_OpGL.h"

#include <iostream>
#include <memory>
#include <list>

bool Renderdraw1::render( RenderData renderdata ){
    RenderPass< RenderBackEndOpenGL, PassType_NoShadow_OpGL > renderpass;
    std::list<RenderEntity *> entities;
    for( auto & i : renderdata._entities ){
	entities.push_back( i.get() );
    }
    return renderpass.Process( renderdata._glslprogram, entities, renderdata._light.get(), renderdata._camera.get(), renderdata._context.get() );
}
