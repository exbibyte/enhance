#ifndef RENDERPASS_H
#define RENDERPASS_H

#include "RenderBackend.h"

#include <string>
#include <list>

template< class RenderBackEnd, class PassType > 
class RenderPass {};

template< class PassType > 
class RenderPass< RenderBackEndOpenGL, PassType > : public PassType {
public:
    bool Process( GLSLProgram * glsl_program, std::list< RenderEntity * > entities, RenderLight * light, RenderCamera * camera, RenderContext * context ){
	bool bRet = PassType::Process( glsl_program, entities, light, camera, context );
	return bRet;
    }
};

template< class PassType > 
class RenderPass< RenderBackEndDirectX, PassType > {
public:
    bool Process( GLSLProgram * glsl_program, std::list< RenderEntity * > * entities, RenderLight * light, RenderCamera * camera, RenderContext * context ){
	return false;
    }
};

#endif
