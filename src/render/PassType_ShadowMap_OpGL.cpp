#include "PassType_ShadowMap_OpGL.h"
#include "GLSLProgram.h"
#include "RenderType.h"

#include <GLFW/glfw3.h>

#include <vector>
#include <cassert>
#include <string>
#include <map>

using namespace std;

bool PassType_ShadowMap_OpGL::AddToProcess( eRenderType render_type, vector< double > render_data ){
    // add data pertaining to different render types
    auto it_find = _map_render_double.find( render_type );
    if( _map_render_double.end() == it_find ){
	_map_render_double[ render_type ] = render_data;
    }else{
	it_find->second.insert( it_find->second.end(), render_data.begin(), render_data.end() );
    }
    return true;
}

bool PassType_ShadowMap_OpGL::AddToProcess( eRenderType render_type, mat4 render_data ){
    // add data pertaining to different render types
    auto it_find = _map_render_mat4.find( render_type );
    if( _map_render_mat4.end() == it_find ){
	vector< mat4 > new_item { render_data };
	_map_render_mat4[ render_type ] = new_item;
    }else{
	it_find->second.push_back( render_data );
    }
    return true;
}

bool PassType_ShadowMap_OpGL::AddToProcess( eRenderType render_type, mat3 render_data ){
    // add data pertaining to different render types
    auto it_find = _map_render_mat3.find( render_type );
    if( _map_render_mat3.end() == it_find ){
	vector< mat3 > new_item { render_data };
	_map_render_mat3[ render_type ] = new_item;
    }else{
	it_find->second.push_back( render_data );
    }
    return true;
}

bool PassType_ShadowMap_OpGL::ProcessNow( GLSLProgram * glsl_program, string strPassType ){
    bool bRet = true;

    //convert data to array
    float * vert_pos = nullptr;
    float * vert_norm = nullptr;
    int iNumDataVertex;
    int iNumDataNormal;
    //retrieve GL attribute handles
    GLAttribData<float> * attrib_PositionData;
    GLAttribData<float> * attrib_NormalData;
    if( !glsl_program->GetMapAttrib( "VertexPosition", attrib_PositionData ) ){
	assert( 0 && "VertexPosition attribute not found in PassType_ShadowMap_OpGL::ProcessNow" );
	return false;
    }
    if( !glsl_program->GetMapAttrib( "VertexNormal", attrib_NormalData ) ){
	assert( 0 && "VertexNormal attribute not found in PassType_ShadowMap_OpGL::ProcessNow" );
	return false;
    }
    {
	auto it_find = _map_render_double.find( eRenderType::POLY_VERT );
	if( _map_render_double.end() == it_find ){
	    assert( 0 && "Cannot find POLY_VERT in PassType_ShadowMap_OpGL" );
	    return false;
	}
	iNumDataVertex = it_find->second.size();
	vert_pos = new float[ iNumDataVertex ];
	float * p_data = vert_pos;
	for( auto & i : it_find->second ){
	    *p_data++ = (float)i;
	}
    }
    {
	auto it_find = _map_render_double.find( eRenderType::POLY_NORM );
	if( _map_render_double.end() == it_find ){
	    assert( 0 && "Cannot find POLY_NORM in PassType_ShadowMap_OpGL" );
	    return false;
	}
	iNumDataNormal = it_find->second.size();
	vert_norm = new float[ iNumDataNormal ];
	float * p_data = vert_norm;
	for( auto & i : it_find->second ){
	    *p_data++ = (float)i;
	}
    }

    if( iNumDataVertex != iNumDataNormal ){
	assert( 0 && "Vertex and Normal count not equal in PassType_ShadowMap_OpGL" );
	delete vert_pos;
	delete vert_norm;
	return false;
    }else{
	_CountRenderVerts = iNumDataVertex;
    }
    //generate VBO, populate and bind data to vertex attribute arrays
    attrib_PositionData->SetData( vert_pos, 3, iNumDataVertex );
    attrib_NormalData->SetData( vert_norm, 3, iNumDataNormal );

    delete [] vert_pos;
    delete [] vert_norm;

    if( "DEPTH" == strPassType ){
        //first pass render for light POV    
        glViewport( 0, 0, 2500, 2500 );
	bRet = ProcessPassDepth( glsl_program );
	if( !bRet ){
	    return false;
	}
    }else{
	//2nd pass render
        glViewport( 0, 0, 500, 500 );
	bRet = ProcessPassNormal( glsl_program );
	if( !bRet ){
	    return false;
	}
    }
    return bRet;
}

bool PassType_ShadowMap_OpGL::ProcessPassDepth( GLSLProgram * glsl_program ){
    bool bRet = true;
    if( !glsl_program ){
	assert( 0 && "GLSLProgram invalid in GLRenderPassShadowMap::ProcessPassDepth()");
	return false;
    }
    bRet = glsl_program->SetUniform( "bShadeShadow", false );
    glCullFace(GL_BACK);
    bRet = ProcessPassCommon( glsl_program );
    if( !bRet ){
	assert( 0 && "GLRenderPassShadowMap::ProcessPassDepth() failed");
    }

    return bRet;
}

bool PassType_ShadowMap_OpGL::ProcessPassNormal( GLSLProgram * glsl_program ){
    bool bRet = true;
    if( !glsl_program ){
	assert( 0 && "GLSLProgram invalid in GLRenderPassShadowMap::ProcessPassNormal()");
	return false;
    }
    bRet = glsl_program->SetUniform( "bShadeShadow", true );
    glCullFace(GL_BACK);
    bRet = ProcessPassCommon( glsl_program );
    if( !bRet ){
	assert( 0 && "GLRenderPassShadowMap::ProcessPassNormal() failed");
    }
    return bRet;
}

bool PassType_ShadowMap_OpGL::ProcessPassCommon( GLSLProgram * glsl_program ){
    bool bRet = true;

    if( !glsl_program ){
	assert( 0 && "GLSLProgram invalid in GLRenderPassShadowMap::ProcessPassDepth()");
	return false;
    }

    //retrieve matrices needed for rendering, TODO
    vector< mat4 > attrib_model_view_matrix;
    vector< mat4 > attrib_model_view_perspective_matrix;
    vector< mat4 > attrib_model_view_perspective_bias_matrix;
    vector< mat3 > attrib_normal_matrix;
    vector< mat4 > attrib_light_view_matrix;
    
    if( !GetAttribute( eRenderType::MODEL_VIEW_MATRIX, attrib_model_view_matrix ) ){
	assert( 0 && "Cannot find Model View Matrix attribute in PassType_ShadowMap_OpGL::ProcessPassCommon" );	
    }
    if( !GetAttribute( eRenderType::MODEL_VIEW_PERSPECTIVE_MATRIX, attrib_model_view_perspective_matrix ) ){
	assert( 0 && "Cannot find Model View Perspective Matrix attribute in PassType_ShadowMap_OpGL::ProcessPassCommon" );	
    }
    if( !GetAttribute( eRenderType::MODEL_VIEW_PERSPECTIVE_BIAS_MATRIX, attrib_model_view_perspective_bias_matrix ) ){
	assert( 0 && "Cannot find Model View Perspective Bias Matrix attribute in PassType_ShadowMap_OpGL::ProcessPassCommon" );	
    }
    if( !GetAttribute( eRenderType::NORMAL_MATRIX, attrib_normal_matrix ) ){
	assert( 0 && "Cannot find Normal Matrix attribute in PassType_ShadowMap_OpGL::ProcessPassCommon" );	
    }
    if( !GetAttribute( eRenderType::LIGHT_VIEW_MATRIX, attrib_light_view_matrix ) ){
	assert( 0 && "Cannot find Light View Matrix attribute in PassType_ShadowMap_OpGL::ProcessPassCommon" );	
    }

    //use the first item from the vector for now
    mat4 ModelViewMatrix = attrib_model_view_matrix[0];
    mat4 MVP = attrib_model_view_perspective_matrix[0];
    mat4 MVPB = attrib_model_view_perspective_bias_matrix[0];
    mat3 NormalMatrix = attrib_normal_matrix[0];
    mat4 LightViewMatrix = attrib_light_view_matrix[0];

    bRet = glsl_program->SetUniform( "ShadowMatrix", (mat4 const) MVPB );
    bRet = glsl_program->SetUniform( "MVP", (mat4 const) MVP );
    bRet = glsl_program->SetUniform( "ModelViewMatrix", (mat4 const) ModelViewMatrix );
    bRet = glsl_program->SetUniform( "NormalMatrix", (mat3 const) NormalMatrix );
    bRet = glsl_program->SetUniform( "LightViewMatrix", (mat4 const) LightViewMatrix );
    
    glsl_program->BindVertexArray();

    //render everything in the buffer for now	
    GLBufferInfo render_buffer_info;
    render_buffer_info._Name = "Default_Render_Buffer";
    render_buffer_info._Offset = 0;
    render_buffer_info._Length = _CountRenderVerts;
    glsl_program->SetBufferInfo( &render_buffer_info );
    glsl_program->SetCurrentBufferInfo( "Default_Render_Buffer" );
    glsl_program->DrawCurrentBufferSegment();

    glsl_program->UnBindVertexArray();

    return bRet;
}

bool PassType_ShadowMap_OpGL::GetAttribute( eRenderType render_type, vector< mat4 > & attrib ){
    auto it = _map_render_mat4.find( render_type );
    if( _map_render_mat4.end() == it ){
	return false;
    }
    if( it->second.empty() ){
	return false;
    }
    attrib = it->second;
    return true;
}

bool PassType_ShadowMap_OpGL::GetAttribute( eRenderType render_type, vector< mat3 > & attrib ){
    auto it = _map_render_mat3.find( render_type );
    if( _map_render_mat3.end() == it ){
	return false;
    }
    if( it->second.empty() ){
	return false;
    }
    attrib = it->second;
    return true;
}

bool PassType_ShadowMap_OpGL::Clear(){
//    _map_render_double.clear(); TODO: vertex data to be flushed
    _map_render_mat4.clear();
    return true;
}
