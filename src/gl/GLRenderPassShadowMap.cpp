#include "GLRenderPassShadowMap.h"
#include "RenderMeshOrientation.h"
#include <GLFW/glfw3.h>
#include "GLSLProgram.h"

#include <cassert>
#include <list>
#include <string>
using namespace std;

bool GLRenderPassShadowMap::ProcessPassDepth( GLSLProgram * glsl_program, RenderMeshOrientation & render_mesh_orientation, list<string> & buffer_obj_name ){
    bool bRet = true;
    if( !glsl_program ){
	assert( 0 && "GLSLProgram invalid in GLRenderPassShadowMap::ProcessPassDepth()");
	return false;
    }
    // if( 0 != IsShadeShadow ){
    bRet = glsl_program->SetUniform( "bShadeShadow", false );
    // 	IsShadeShadow = 0;
    // }
    // glCullFace(GL_BACK);
    bRet = ProcessPassAux( glsl_program, render_mesh_orientation, buffer_obj_name );
    if( !bRet ){
	assert( 0 && "GLRenderPassShadowMap::ProcessPassDepth() failed");
    }
    // glCullFace(GL_BACK);
    return bRet;
}

bool GLRenderPassShadowMap::ProcessPassNormal( GLSLProgram * glsl_program, RenderMeshOrientation & render_mesh_orientation, list<string> & buffer_obj_name ){
    bool bRet = true;
    if( !glsl_program ){
	assert( 0 && "GLSLProgram invalid in GLRenderPassShadowMap::ProcessPassNormal()");
	return false;
    }
    // if( 1 != IsShadeShadow ){
    bRet = glsl_program->SetUniform( "bShadeShadow", true );
    // 	IsShadeShadow = 1;
    // }
    // glCullFace(GL_FRONT);
    bRet = ProcessPassAux( glsl_program, render_mesh_orientation, buffer_obj_name );
    if( !bRet ){
	assert( 0 && "GLRenderPassShadowMap::ProcessPassNormal() failed");
    }
    return bRet;
}

bool GLRenderPassShadowMap::ProcessPassAux( GLSLProgram * glsl_program, RenderMeshOrientation & render_mesh_orientation, list<string> & buffer_obj_name ){
    bool bRet = true;

    if( !glsl_program ){
	assert( 0 && "GLSLProgram invalid in GLRenderPassShadowMap::ProcessPassDepth()");
	return false;
    }

    mat4 ModelViewMatrix;
    mat4 MVP;
    mat4 MVPB;
    mat3 NormalMatrix;
    if ( !render_mesh_orientation.GetCompositeMats( ModelViewMatrix, MVP, MVPB, NormalMatrix ) ){
	assert( 0 && "GetCompositeMats() failed in GLRenderPassShadowMap::ProcessPassDepth()");
	return false;
    }

    // glCullFace(GL_BACK);
    bRet = glsl_program->SetUniform( "ShadowMatrix", (mat4 const) MVPB );
    bRet = glsl_program->SetUniform( "MVP", (mat4 const) MVP );
    bRet = glsl_program->SetUniform( "ModelViewMatrix", (mat4 const) ModelViewMatrix );
    bRet = glsl_program->SetUniform( "NormalMatrix", (mat3 const) NormalMatrix );
    mat4 LightViewMatrix = render_mesh_orientation._MatView;
    // bRet = glsl_program->SetUniform( "LightViewMatrix", (mat4 const) LightViewMatrix );
    glsl_program->BindVertexArray();
    for( auto & i : buffer_obj_name ){
	if( !glsl_program->SetCurrentBufferInfo( i ) ){
	    assert( 0 && "GLSLProgram::DrawCurrentBufferSegment() failed in GLRenderPassShadowMap::ProcessPassDepth()" );
	    return false;
	}
	if( !glsl_program->DrawCurrentBufferSegment() ){
	    assert( 0 && "GLSLProgram::DrawCurrentBufferSegment() failed in GLRenderPassShadowMap::ProcessPassDepth()" );
	    return false;
	}
    }
    glsl_program->UnBindVertexArray();
    // glCullFace(GL_BACK);

    return bRet;
}
