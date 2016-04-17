#include "PassType_ShadowMap_OpGL.h"
#include "GLSLProgram.h"
#include "RenderContext.h"
#include "RenderEntity.h"
#include "RenderLight.h"
#include "RenderCamera.h"
#include "RenderPoly.h"
#include "RenderVertex.h"
#include "RenderMaterial.h"

#include <GLFW/glfw3.h>

#include <vector>
#include <cassert>
#include <string>
#include <map>
#include <algorithm>

using namespace std;

bool PassType_ShadowMap_OpGL::Process( GLSLProgram * glsl_program, list< RenderEntity *> * entities, RenderLight * light, RenderCamera * camera, RenderContext * context ){
    bool bRet = true;
    if( !glsl_program ){
	assert( 0 && "GLSLProgram invalid in GLRenderPassShadowMap::Process()");
	return false;
    }
    if( !entities ||
	!light ||
    	!camera ||
    	!context ){
    	return false;
    }

    //retrieve render context data
    RenderContextData::WindowSize::Type data_windowsize;
    RenderContextData::TextureSizeShadowMap::Type data_texturesize_shadowmap;
    RenderContextData::Title::Type data_title;
    if( !context->Compute( data_windowsize, data_texturesize_shadowmap, data_title ) ){
	assert( 0 && "RenderContext Compute() failed." );
	return false;
    }
    // switch( pass_type ){
    // case PassType::DEPTH:
    // {
        //first pass render for light POV
	//bind shadow map texture prior to rendering with light's POV
	GLTexture * ShadowTexture;
	if( glsl_program->GetMapTexture("ShadowTexture", ShadowTexture ) ) {
	    ShadowTexture->BindFbo();
	    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
	    int iActiveTexture;
	    ShadowTexture->GetActiveTexture( iActiveTexture );
	    bRet = glsl_program->SetUniform( "ShadowMap", iActiveTexture );
	} else {
	    assert( 0 && "Can't find ShadowTexture." );
	    return false;
	}

	//set render window with rendering context data: view port size
	int width = data_texturesize_shadowmap[0];
	int height = data_texturesize_shadowmap[1];
	glViewport( 0, 0, width, height );

	bRet = glsl_program->SetUniform( "bShadeShadow", false );
	glCullFace(GL_BACK);

	bRet = ProcessPassCommon( PassType::DEPTH, glsl_program, entities, light, camera, context );
	if( !bRet ){
	    assert( 0 && "GLRenderPassShadowMap::ProcessPassCommon() for Depth Render failed");
	}
    // }
    // 	break;
    // case PassType::NORMAL:
    // {
	//2nd pass render
	//unbind shadow map texture prior to rendering from light's POV
	// GLTexture * ShadowTexture;
	if( glsl_program->GetMapTexture("ShadowTexture", ShadowTexture ) ) {
	    ShadowTexture->UnbindFbo();
	} else {
	    assert( 0 && "Can't find ShadowTexture." );
	    return false;
	}
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
	
	//set render window with rendering context data: view port size
	width = data_windowsize[0];
	height = data_windowsize[1];
	glViewport( 0, 0, width, height );
		
	bRet = glsl_program->SetUniform( "bShadeShadow", true );
	glCullFace(GL_BACK);
	bRet = ProcessPassCommon( PassType::NORMAL, glsl_program, entities, light, camera, context );
	if( !bRet ){
	    assert( 0 && "GLRenderPassShadowMap::ProcessPassCommon() for Normal Render failed");
	}
    // }
    // 	break;
    // default:
    // 	return false;
    // }
    return true;
}

bool PassType_ShadowMap_OpGL::ProcessPassCommon( PassType pass_type, GLSLProgram * glsl_program, list< RenderEntity * > * entities, RenderLight * light, RenderCamera * camera, RenderContext * context ){
    bool bRet = true;

    if( !glsl_program ){
	assert( 0 && "GLSLProgram invalid in GLRenderPassShadowMap::ProcessPassDepth()");
	return false;
    }

    /// light --------------------------------------------------------------
    //retrieve the above attributes
    RenderLightData::Ambient::Type light_data_ambient;
    RenderLightData::Diffuse::Type light_data_diffuse;
    RenderLightData::Specular::Type light_data_specular;
    RenderLightData::Coordinate::Type light_data_coordinate;
    mat4 light_perspective_frust_matrix;
    mat4 light_view_matrix;
    if( !light->Compute( light_data_ambient, light_data_diffuse, light_data_specular, light_data_coordinate,
			 light_perspective_frust_matrix,
			 light_view_matrix ) ){
	assert( 0 && "RenderLight Compute() failed." );
	return false;
    }
    //retrieve light data
    vec4 Light_Position = vec4( light_data_coordinate[0], light_data_coordinate[1], light_data_coordinate[2], 1 );
    vec3 Light_Ambient = vec3( light_data_ambient[0], light_data_ambient[1], light_data_ambient[2] );
    vec3 Light_Diffuse = vec3( light_data_diffuse[0], light_data_diffuse[1], light_data_diffuse[2] );
    vec3 Light_Specular = vec3( light_data_specular[0], light_data_specular[1], light_data_specular[2] );
    //set light data
    bRet = glsl_program->SetUniform( "Light.Position", Light_Position );
    bRet = glsl_program->SetUniform( "Light.La", Light_Ambient );
    bRet = glsl_program->SetUniform( "Light.Ld", Light_Diffuse );
    bRet = glsl_program->SetUniform( "Light.Ls", Light_Specular );

    /// camera -------------------------------------------------------------
    vec3 Camera_Position_3;
    vec4 Camera_Position;
    vec3 Camera_Ambient;
    vec3 Camera_Diffuse;
    vec3 Camera_Specular;
    RenderCameraData::Ambient::Type camera_data_ambient;
    RenderCameraData::Diffuse::Type camera_data_diffuse;
    RenderCameraData::Specular::Type camera_data_specular;
    RenderCameraData::Coordinate::Type camera_data_coordinate;
    mat4 camera_perspective_frust_matrix;
    mat4 camera_view_matrix;
    if( !camera->Compute( camera_data_ambient, camera_data_diffuse,
			  camera_data_specular, camera_data_coordinate,
			  camera_perspective_frust_matrix,
			  camera_view_matrix ) ){
	assert( 0 && "RenderCamera Compute() failed." );
	return false;
    }

    /// per entity ---------------------------------------------------------
    for( auto * entity : *entities ){
	// if( !entity ){
	//     assert( 0 && "entity data invalid." );
	//     return false;
	// }
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
	
	/// retrieve polygon vertex and normal data --------------------------
	RenderVertexData::Vertices::Type vertices;
	RenderVertexData::Normals::Type normals;
	if( !entity->Compute( RenderVertexData(), vertices, normals ) ){
	    assert( 0 && "RenderVertex Compute() failed." );
	    return false;
	}

	int iSizeVertexData = vertices.size();
	float * vertex_coord = new float[ iSizeVertexData ];
	float * vertex_normal = new float[ iSizeVertexData ];
	float * vertex_coord_iter = vertex_coord;
	float * vertex_normal_iter = vertex_normal;
	auto it_vertices = vertices.begin();
	auto it_normals = normals.begin();
	for( int i = 0; i < iSizeVertexData; ++i ){
	    *vertex_coord_iter = (float)*it_vertices;
	    *vertex_normal_iter = (float)*it_normals;
	    ++vertex_coord_iter;
	    ++vertex_normal_iter;
	    ++it_vertices;
	    ++it_normals;
	}
	//generate VBO, populate and bind data to vertex attribute arrays
	attrib_PositionData->SetData( vertex_coord, 3, iSizeVertexData );
	attrib_NormalData->SetData( vertex_normal, 3, iSizeVertexData );

	delete [] vertex_coord;
	delete [] vertex_normal;

	glsl_program->BindVertexArray();
	
	/// retrive entity orientation data ----------------------------------
	RenderPolyData::Coordinate::Type entity_coordinate;
	RenderPolyData::RotationAxis::Type entity_rotation_axis;
	RenderPolyData::RotationAngle::Type entity_rotation_angle;
	mat4 entity_orientation_matrix;
	if( !entity->Compute( RenderPolyData(), entity_coordinate, entity_rotation_axis, entity_rotation_angle, entity_orientation_matrix ) ){
	    assert( 0 && "RenderPoly Compute() failed." );
	    return false;
	}

	//retrieve matrices needed for rendering, TODO
	mat4 Bias(
	    0.5, 0.0, 0.0, 0.0,
	    0.0, 0.5, 0.0, 0.0,
	    0.0, 0.0, 0.5, 0.0,
	    0.5, 0.5, 0.5, 1.0
	    );

	mat4 view_matrix_light_or_camera;
	mat4 perspective_matrix_light_or_camera;
	//use light or camera view depending on pass_type
	//use light or camera perspective depending on pass_type
	switch( pass_type ){
	case PassType::DEPTH:
	{
	    view_matrix_light_or_camera = light_view_matrix;
	    perspective_matrix_light_or_camera = light_perspective_frust_matrix;
	}
	    break;
	case PassType::NORMAL:
	{
	    view_matrix_light_or_camera = camera_view_matrix;
	    perspective_matrix_light_or_camera = camera_perspective_frust_matrix;
	}
	    break;
	default:
	    assert( 0 && "PassType invalid." );
	    return false;
	}

	mat4 attrib_model_view_matrix = view_matrix_light_or_camera * entity_orientation_matrix;
	mat4 attrib_model_view_perspective_matrix = perspective_matrix_light_or_camera * view_matrix_light_or_camera * entity_orientation_matrix;
	// MVPB matrix's view and perspective matrices are from light's POV
	mat4 attrib_model_view_perspective_bias_matrix = Bias * light_perspective_frust_matrix * light_view_matrix * entity_orientation_matrix;
	mat3 attrib_normal_matrix = glm::inverse( glm::transpose( glm::mat3( attrib_model_view_matrix ) ) );
	mat4 attrib_light_view_matrix = light_view_matrix;

	/// retrieve material data ------------------------------------------
	RenderMaterialData::Ambient::Type data_ambient;
	RenderMaterialData::Diffuse::Type data_diffuse;
	RenderMaterialData::Specular::Type data_specular;
	RenderMaterialData::Shininess::Type data_shininess;
	if( !entity->Compute( RenderMaterialData(), data_ambient, data_diffuse, data_specular, data_shininess ) ){
	    assert( 0 && "RenderVertex Compute() failed." );
	    return false;
	}
	vec3 Material_Ambient = vec3( data_ambient[0], data_ambient[1], data_ambient[2] );
	vec3 Material_Diffuse = vec3( data_diffuse[0], data_diffuse[1], data_diffuse[2] );
	vec3 Material_Specular = vec3( data_specular[0], data_specular[1], data_specular[2] );
	float Material_Shininess = data_shininess[0];
	//set material data
	bRet = glsl_program->SetUniform( "Material.Ka", Material_Ambient );
	bRet = glsl_program->SetUniform( "Material.Kd", Material_Diffuse );
	bRet = glsl_program->SetUniform( "Material.Ks", Material_Specular );
	bRet = glsl_program->SetUniform( "Material.Shininess", Material_Shininess );

	bRet = glsl_program->SetUniform( "ShadowMatrix", (mat4 const) attrib_model_view_perspective_bias_matrix );
	bRet = glsl_program->SetUniform( "MVP", (mat4 const) attrib_model_view_perspective_matrix );
	bRet = glsl_program->SetUniform( "ModelViewMatrix", (mat4 const) attrib_model_view_matrix );
	bRet = glsl_program->SetUniform( "NormalMatrix", (mat3 const) attrib_normal_matrix );
	bRet = glsl_program->SetUniform( "LightViewMatrix", (mat4 const) attrib_light_view_matrix );

	// glsl_program->BindVertexArray();

	//render everything in the buffer for now	
	GLBufferInfo render_buffer_info;
	render_buffer_info._Name = "Default_Render_Buffer";
	render_buffer_info._Offset = 0;
	render_buffer_info._Length = iSizeVertexData;
	glsl_program->SetBufferInfo( &render_buffer_info );
	glsl_program->SetCurrentBufferInfo( "Default_Render_Buffer" );
	glsl_program->DrawCurrentBufferSegment();

	glsl_program->UnBindVertexArray();
    }
    return bRet;
}
