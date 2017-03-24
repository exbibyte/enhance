#ifndef PASSTYPE_NOSHADOW_OPGL
#define PASSTYPE_NOSHADOW_OPGL

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

class PassType_NoShadow_OpGL {
public:
    bool Process( GLSLProgram *, list< RenderEntity * > entities, RenderLight * light, RenderCamera * camera, RenderContext * context );
private:
    bool ProcessPassCommon( GLSLProgram *, list< RenderEntity * > entity, RenderLight * light, RenderCamera * camera, RenderContext * context );
};

#endif
