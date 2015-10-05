#ifndef GL_RENDER_PASS_SHADOW_MAP_H
#define GL_RENDER_PASS_SHADOW_MAP_H

#include "RenderMeshOrientation.h"

#include <list>
#include <string>
#include <utility>

class GLSLProgram;

class GLRenderPassShadowMap {
public:
    GLRenderPassShadowMap(){}
    bool ProcessPass( std::string strPassType, GLSLProgram * glsl_program );
    bool ProcessPassDepth( GLSLProgram * GLSLProgram, RenderMeshOrientation &, std::list<std::string> & buffer_obj_name );
    bool ProcessPassNormal( GLSLProgram * GLSLProgram, RenderMeshOrientation &, std::list<std::string> & buffer_obj_name );
    bool ProcessPassAux( GLSLProgram * GLSLProgram, RenderMeshOrientation &, std::list<std::string> & buffer_obj_name );
    bool AddPath( std::string strPassType, RenderMeshOrientation & mesh_orientation, std::list<std::string> & buffer_obj_name );
private:
    std::list< std::pair< RenderMeshOrientation, std::list<std::string> > > list_pass_depth;
    std::list< std::pair< RenderMeshOrientation, std::list<std::string> > > list_pass_normal;
};

#endif
