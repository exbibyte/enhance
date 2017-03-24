#include "PassType_NoShadow_OpGL.h"
#include "GLSLProgram.h"
#include "RenderContext.h"
#include "RenderEntity.h"
#include "RenderLight.h"
#include "RenderCamera.h"
#include "RenderPoly.h"
#include "RenderVertex.h"
#include "RenderMaterial.h"

#include "GLIncludes.hpp"

#include "Mat.hpp"
#include "MatrixMath.h"

#include <vector>
#include <cassert>
#include <string>
#include <map>
#include <algorithm>

using namespace std;

bool PassType_NoShadow_OpGL::Process( GLSLProgram * glsl_program, list< RenderEntity *> entities, RenderLight * light, RenderCamera * camera, RenderContext * context ){
    bool bRet = true;
    if( !glsl_program ){
	assert( 0 && "GLSLProgram invalid in GLRenderPassNoShadow::Process()");
	return false;
    }
    if( !light ||
    	!camera ||
    	!context ){
    	return false;
    }

    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

    //retrieve render context data
    RenderContextData::WindowSize::Type data_windowsize;
    RenderContextData::TextureSizeShadowMap::Type data_texturesize_shadowmap;
    RenderContextData::Title::Type data_title;
    if( !context->Compute( data_windowsize, data_texturesize_shadowmap, data_title ) ){
	assert( 0 && "RenderContext Compute() failed." );
	return false;
    }

    //set render window with rendering context data: view port size
    int width = data_windowsize[0];
    int height = data_windowsize[1];
    glViewport( 0, 0, width, height );

    glCullFace(GL_BACK);
    bRet = ProcessPassCommon( glsl_program, entities, light, camera, context );
    if( !bRet ){
	assert( 0 && "GLRenderPassNoShadow::ProcessPassCommon() for Normal Render failed");
    }

    return true;
}

bool PassType_NoShadow_OpGL::ProcessPassCommon( GLSLProgram * glsl_program, list< RenderEntity * > entities, RenderLight * light, RenderCamera * camera, RenderContext * context ){
    bool bRet = true;

    if( !glsl_program ){
	assert( 0 && "GLSLProgram invalid in GLRenderPassNoShadow::ProcessPassDepth()");
	return false;
    }

    /// light --------------------------------------------------------------
    //retrieve the above attributes
    RenderLightData::Ambient::Type light_data_ambient;
    RenderLightData::Diffuse::Type light_data_diffuse;
    RenderLightData::Specular::Type light_data_specular;
    RenderLightData::Coordinate::Type light_data_coordinate;
    Mat light_perspective_frust_matrix;
    Mat light_view_matrix;
    if( !light->Compute( light_data_ambient, light_data_diffuse, light_data_specular, light_data_coordinate,
			 light_perspective_frust_matrix,
			 light_view_matrix ) ){
	assert( 0 && "RenderLight Compute() failed." );
	return false;
    }
    //retrieve light data
    Vec Light_Position;
    Light_Position.SetFromArray( 4, &light_data_coordinate[0] ); //4x1
    Vec Light_Ambient;
    Light_Ambient.SetFromArray( 3, &light_data_ambient[0] ); //3x1
    Vec Light_Diffuse;
    Light_Diffuse.SetFromArray( 3, &light_data_diffuse[0] ); //3x1
    Vec Light_Specular;
    Light_Specular.SetFromArray( 3, &light_data_specular[0] ); //3x1
    //set light data
    bRet = glsl_program->SetUniformVec4( "Light.Position", Light_Position );
    bRet = glsl_program->SetUniformVec3( "Light.La", Light_Ambient );
    bRet = glsl_program->SetUniformVec3( "Light.Ld", Light_Diffuse );
    bRet = glsl_program->SetUniformVec3( "Light.Ls", Light_Specular );

    /// camera -------------------------------------------------------------
    RenderCameraData::Ambient::Type camera_data_ambient;
    RenderCameraData::Diffuse::Type camera_data_diffuse;
    RenderCameraData::Specular::Type camera_data_specular;
    RenderCameraData::Coordinate::Type camera_data_coordinate;
    Mat camera_perspective_frust_matrix;
    Mat camera_view_matrix;
    if( !camera->Compute( camera_data_ambient, camera_data_diffuse,
			  camera_data_specular, camera_data_coordinate,
			  camera_perspective_frust_matrix,
			  camera_view_matrix ) ){
	assert( 0 && "RenderCamera Compute() failed." );
	return false;
    }

    /// per entity ---------------------------------------------------------
    for( auto * entity : entities ){
	if( !entity ){
	    assert( 0 && "entity data invalid." );
	    return false;
	}
	GLAttribData<float> * attrib_PositionData;
	GLAttribData<float> * attrib_NormalData;
	if( !glsl_program->GetMapAttrib( "VertexPosition", attrib_PositionData ) ){
	    assert( 0 && "VertexPosition attribute not found in PassType_NoShadow_OpGL::ProcessNow" );
	    return false;
	}
	if( !glsl_program->GetMapAttrib( "VertexNormal", attrib_NormalData ) ){
	    assert( 0 && "VertexNormal attribute not found in PassType_NoShadow_OpGL::ProcessNow" );
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
	Mat entity_orientation_matrix;
	if( !entity->Compute( RenderPolyData(), entity_coordinate, entity_rotation_axis, entity_rotation_angle, entity_orientation_matrix ) ){
	    assert( 0 && "RenderPoly Compute() failed." );
	    return false;
	}

	//retrieve matrices needed for rendering, TODO
	float bias[16] = { 0.5, 0.0, 0.0, 0.0,
			   0.0, 0.5, 0.0, 0.0,
			   0.0, 0.0, 0.5, 0.0,
			   0.5, 0.5, 0.5, 1.0 };	   
	Mat Bias;
	size_t col = 4, row = 4;
	Bias.SetFromArray( bias, col, row );

	Mat view_matrix_light_or_camera;
	Mat perspective_matrix_light_or_camera;

	view_matrix_light_or_camera = camera_view_matrix;
	perspective_matrix_light_or_camera = camera_perspective_frust_matrix;

	Mat attrib_model_view_matrix = view_matrix_light_or_camera * entity_orientation_matrix;
	Mat attrib_model_view_perspective_matrix = perspective_matrix_light_or_camera * view_matrix_light_or_camera * entity_orientation_matrix; //4x4
	// MVPB matrix's view and perspective matrices are from light's POV
	Mat attrib_model_view_perspective_bias_matrix = Bias * light_perspective_frust_matrix * light_view_matrix * entity_orientation_matrix; //4x4

        // attrib_normal_matrix <- inverse( transpose( mat3( attrib_model_view_matrix ) ) )//3x3
	Mat attrib_model_view_matrix_3x3;
	assert( attrib_model_view_matrix.GetSubMat( attrib_model_view_matrix_3x3, 0, 0, 3, 3 ) && "obtaining submatrix failed." );
	Mat attrib_model_view_matrix_3x3_transpose = attrib_model_view_matrix_3x3.Transpose();
	size_t actual_num;
	float attrib_model_view_matrix_3x3_transpose_raw[9];
	assert( attrib_model_view_matrix_3x3_transpose.GetArray( attrib_model_view_matrix_3x3_transpose_raw, 9, actual_num ) && "get array failed." );
	float attrib_model_view_matrix_3x3_transpose_inverse_raw[9];
	assert( MatrixMath::InvertMatrix3x3( attrib_model_view_matrix_3x3_transpose_raw, attrib_model_view_matrix_3x3_transpose_inverse_raw ) && "inversing matrix failed." );
	Mat attrib_normal_matrix; //3x3
	attrib_normal_matrix.SetFromArray( attrib_model_view_matrix_3x3_transpose_inverse_raw, 3, 3 );

	/// retrieve material data ------------------------------------------
	RenderMaterialData::Ambient::Type data_ambient;
	RenderMaterialData::Diffuse::Type data_diffuse;
	RenderMaterialData::Specular::Type data_specular;
	RenderMaterialData::Shininess::Type data_shininess;
	if( !entity->Compute( RenderMaterialData(), data_ambient, data_diffuse, data_specular, data_shininess ) ){
	    assert( 0 && "RenderVertex Compute() failed." );
	    return false;
	}
	Vec Material_Ambient;
	Material_Ambient.SetFromArray( 3, &data_ambient[0] ); //3x1
	Vec Material_Diffuse;
	Material_Diffuse.SetFromArray( 3, &data_diffuse[0] ); //3x1
	Vec Material_Specular;
	Material_Specular.SetFromArray( 3, &data_specular[0] ); //3x1
	float Material_Shininess = data_shininess[0];
	//set material data
	bRet = glsl_program->SetUniformVec3( "Material.Ka", Material_Ambient );
	bRet = glsl_program->SetUniformVec3( "Material.Kd", Material_Diffuse );
	bRet = glsl_program->SetUniformVec3( "Material.Ks", Material_Specular );
	bRet = glsl_program->SetUniform( "Material.Shininess", Material_Shininess );

	bRet = glsl_program->SetUniformMat4( "MVP", attrib_model_view_perspective_matrix );
	bRet = glsl_program->SetUniformMat4( "ModelViewMatrix", attrib_model_view_matrix );
	bRet = glsl_program->SetUniformMat3( "NormalMatrix", attrib_normal_matrix );

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
