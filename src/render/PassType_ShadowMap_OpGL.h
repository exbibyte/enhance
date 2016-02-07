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

class GLSLProgram;

class PassType_ShadowMap_OpGL {
public:
    bool AddToProcess( eRenderType render_type, std::vector< double > render_data );
    bool AddToProcess( eRenderType render_type, mat4 render_data );
    bool AddToProcess( eRenderType render_type, mat3 render_data );
    bool ProcessNow( GLSLProgram *, string strPassType );
    bool Clear();
private:
    bool ProcessPassDepth( GLSLProgram * );
    bool ProcessPassNormal( GLSLProgram * );
    bool ProcessPassCommon( GLSLProgram * );
    std::map< eRenderType, std::vector< double > > _map_render_double;
    std::map< eRenderType, std::vector< mat4 > > _map_render_mat4;
    std::map< eRenderType, std::vector< mat3 > > _map_render_mat3;

    bool GetAttribute( eRenderType render_type, std::vector< mat4 > & attrib );
    bool GetAttribute( eRenderType render_type, std::vector< mat3 > & attrib );
    bool GetAttribute( eRenderType render_type, vec3 & attrib );
    bool GetAttribute( eRenderType render_type, vec4 & attrib );
    bool GetAttribute( eRenderType render_type, float & attrib );
    unsigned int _CountRenderVerts;
};

#endif
