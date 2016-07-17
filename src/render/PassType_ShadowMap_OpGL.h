#ifndef PASSTYPE_SHADOWMAP_OPGL
#define PASSTYPE_SHADOWMAP_OPGL

#include "RenderType.h"
#include "GLAttribData.h"

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/transform2.hpp>
using glm::mat3;
using glm::mat4;
using glm::vec3;

#include <vector>
#include <map>
#include <type_traits>
#include <list>

#include "RenderEntity.h"
#include "RenderLight.h"
#include "RenderCamera.h"
#include "RenderContext.h"

class GLSLProgram;

class PassType_ShadowMap_OpGL {
public:
    enum class PassType {
	NORMAL,
	DEPTH,
    };
    // bool Process( GLSLProgram *, PassType pass_type, list< RenderEntity * > entity, RenderLight * light, RenderCamera * camera, RenderContext * context );
    bool Process( GLSLProgram *, list< RenderEntity * > entities, RenderLight * light, RenderCamera * camera, RenderContext * context );
private:
    bool ProcessPassCommon( PassType, GLSLProgram *, list< RenderEntity * > entity, RenderLight * light, RenderCamera * camera, RenderContext * context );
};

#endif
