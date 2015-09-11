#include "GLRenderPassShadowMap.h"
#include "RenderMeshOrientation.h"
#include <GLFW/glfw3.h>
#include "GLSLProgram.h"

#include <cassert>
#include <list>
#include <string>
#include <utility>
using namespace std;

bool GLRenderPassShadowMap::ProcessPassDepth( GLSLProgram * glsl_program, RenderMeshOrientation & render_mesh_orientation, list<string> & buffer_obj_name ){
    bool bRet = true;
    if( !glsl_program ){
	assert( 0 && "GLSLProgram invalid in GLRenderPassShadowMap::ProcessPassDepth()");
	return false;
    }
    bRet = glsl_program->SetUniform( "bShadeShadow", false );
    glCullFace(GL_BACK);
    bRet = ProcessPassAux( glsl_program, render_mesh_orientation, buffer_obj_name );
    if( !bRet ){
	assert( 0 && "GLRenderPassShadowMap::ProcessPassDepth() failed");
    }

    return bRet;
}

bool GLRenderPassShadowMap::ProcessPassNormal( GLSLProgram * glsl_program, RenderMeshOrientation & render_mesh_orientation, list<string> & buffer_obj_name ){
    bool bRet = true;
    if( !glsl_program ){
	assert( 0 && "GLSLProgram invalid in GLRenderPassShadowMap::ProcessPassNormal()");
	return false;
    }
    bRet = glsl_program->SetUniform( "bShadeShadow", true );
    glCullFace(GL_BACK);
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

    bRet = glsl_program->SetUniform( "ShadowMatrix", (mat4 const) MVPB );
    bRet = glsl_program->SetUniform( "MVP", (mat4 const) MVP );
    bRet = glsl_program->SetUniform( "ModelViewMatrix", (mat4 const) ModelViewMatrix );
    bRet = glsl_program->SetUniform( "NormalMatrix", (mat3 const) NormalMatrix );
    mat4 LightViewMatrix = render_mesh_orientation._MatView;
    bRet = glsl_program->SetUniform( "LightViewMatrix", (mat4 const) LightViewMatrix );
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

    return bRet;
}

bool GLRenderPassShadowMap::ProcessPass( std::string strPassType, GLSLProgram * glsl_program ){
    bool bRet = true;

    if( "DEPTH" == strPassType ){
	for( auto & i : list_pass_depth ){
	    RenderMeshOrientation mesh_orientation = i.first;
	    list<string> list_buffer_obj_name = i.second;
	    bRet = ProcessPassDepth( glsl_program, mesh_orientation, list_buffer_obj_name );
	    if( !bRet ){
		return false;
	    }
	}
        list_pass_depth.clear();
    }else{
	for( auto & i : list_pass_normal ){
	    RenderMeshOrientation mesh_orientation = i.first;
	    list<string> list_buffer_obj_name = i.second;
	    bRet = ProcessPassNormal( glsl_program, mesh_orientation, list_buffer_obj_name );
	    if( !bRet ){
		return false;
	    }
	}
	list_pass_normal.clear();
    }
    return bRet;    
}

bool GLRenderPassShadowMap::AddPath( std::string strPassType, RenderMeshOrientation & mesh_orientation, std::list<std::string> & buffer_obj_name ){
    bool bRet = true;
    
    if( "DEPTH" == strPassType ){
	list_pass_depth.push_back( std::make_pair( mesh_orientation, buffer_obj_name ) );
    }else{
        list_pass_normal.push_back( std::make_pair( mesh_orientation, buffer_obj_name ) );	
    }

    return bRet;
}
