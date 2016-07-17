#ifndef RENDERPASS_H
#define RENDERPASS_H

#include "RenderBackend.h"

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/transform2.hpp>
using glm::mat3;
using glm::mat4;
using glm::vec3;

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
