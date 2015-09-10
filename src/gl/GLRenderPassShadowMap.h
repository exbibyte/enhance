#ifndef GL_RENDER_PASS_SHADOW_MAP_H
#define GL_RENDER_PASS_SHADOW_MAP_H

#include "RenderMeshOrientation.h"

#include <list>
#include <string>

class GLSLProgram;

class GLRenderPassShadowMap {
public:
    GLRenderPassShadowMap(){}
    bool ProcessPass( std::string strPassType, GLSLProgram * GLSLProgram, RenderMeshOrientation & ){ return false; }
    bool ProcessPassDepth( GLSLProgram * GLSLProgram, RenderMeshOrientation &, std::list<std::string> & buffer_obj_name );
    bool ProcessPassNormal( GLSLProgram * GLSLProgram, RenderMeshOrientation &, std::list<std::string> & buffer_obj_name );
    bool ProcessPassAux( GLSLProgram * GLSLProgram, RenderMeshOrientation &, std::list<std::string> & buffer_obj_name );

private:
    int IsShadeShadow = -1;
};

#endif
