#include "Renderdraw0.hpp"
#include "IRenderdraw.hpp"

#include "RenderData.hpp"
#include "RenderPass.h"
#include "PassType_ShadowMap_OpGL.h"

#include <iostream>
#include <memory>
#include <list>

bool Renderdraw0::render( RenderData renderdata ){

    std::cout << "Renderdraw0::render invoked." << std::endl;
    
    RenderPass< RenderBackEndOpenGL, PassType_ShadowMap_OpGL > renderpass;
    std::list<RenderEntity *> entities;
    for( auto & i : renderdata._entities ){
	entities.push_back( i.get() );
    }
    return renderpass.Process( renderdata._glslprogram, entities, renderdata._light.get(), renderdata._camera.get(), renderdata._context.get() );
}
