#ifndef PASSTYPE_SHADOWMAP_OPGL
#define PASSTYPE_SHADOWMAP_OPGL

#include "RenderType.h"
#include "GLAttribData.h"

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
    bool Process( GLSLProgram *, list< RenderEntity * > entities, RenderLight * light, RenderCamera * camera, RenderContext * context );
private:
    bool ProcessPassCommon( PassType, GLSLProgram *, list< RenderEntity * > entity, RenderLight * light, RenderCamera * camera, RenderContext * context );
};

#endif
