#ifndef EN_SCENE_SAMPLE_H
#define EN_SCENE_SAMPLE_H

//opengl includes
#include <GLFW/glfw3.h>

#include "GLSceneManager.h"
#include "GLSLProgram.h"
#include "WindowManagerGlfw.h"
#include "WingedEdge.h"
#include "enTPCommon.h"

#include "DataTransformDriver.h"
#include "PassParsePolyMesh.h"
#include "PassConvertPolyMeshDataStructToArray.h"
#include "DataTransformPass.h"
#include "DataTransformMetaInfo.h"
#include "Filter_ParsePolyMesh.h"
#include "PolyMesh_Data_Arrays.h"
#include "GLBufferInfo.h"
#include "Clock.h"
#include "GLRenderPassShadowMap.h"
#include "RenderMeshOrientation.h"

#include <functional>
#include <iostream>
#include <cassert>
#include <thread>
#include <chrono>

//math library
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/transform2.hpp>
using glm::mat4;
using glm::vec3;

using namespace std;
using namespace Winged_Edge;

class enSceneSample : public enScene {
public:
    bool SetShaders( GLSLProgram * _GLSLProgram ) {
        bool bRet;
        cout << "func init 01 - Set Shaders" << endl;
        _GLSLProgram->CompileShaderFromFile("./src/gl/shaders/Shadow.vert", GLSLShader::VERTEX );
        _GLSLProgram->CompileShaderFromFile("./src/gl/shaders/Shadow.frag", GLSLShader::FRAGMENT );
        _GLSLProgram->AttachShaders();

        //set buffer data
        GLAttribData<float> * pPositionData = new GLAttribData<float>;
        GLAttribData<float> * pNormalData = new GLAttribData<float>;

        //parse input polymesh file
        PassParsePolyMesh pass_parse_polymesh;
        PassParsePolyMesh * p_pass_parse_polymesh = & pass_parse_polymesh;

        PassConvertPolyMeshDataStructToArray pass_convert_polymesh_to_array;
        PassConvertPolyMeshDataStructToArray * p_pass_convert_polymesh_to_array = & pass_convert_polymesh_to_array;
    
        DataTransformDriver data_transform_driver;

        //set meta info for parsing polymesh
        DataTransformMetaInfo meta_info_parse_polymesh("parse_polymesh");
        string strFilePathPolyMesh = _strPathPolyMesh;
        meta_info_parse_polymesh.AddMetaInfo( "INPUT_DATATYPE", "FILE_POLYMESH" );
        meta_info_parse_polymesh.AddMetaInfo( "INPUT_DATAPATH", strFilePathPolyMesh );
        meta_info_parse_polymesh.AddMetaInfo( "OUTPUT_DATATYPE", "DATASTRUCT_POLYMESH");
        if( !pass_parse_polymesh.RegisterDataTransformMetaInfo( & meta_info_parse_polymesh ) ){
            assert( 0 && "PassParsePolyMesh::RegisterDataTransformMetaInfo failed" );
            return false;
        }

        //set meta info for converting polymesh to array
        DataTransformMetaInfo meta_info_convert_polymesh_to_array("convert_polymesh_to_array");
        meta_info_convert_polymesh_to_array.AddMetaInfo( "INPUT_DATATYPE", "DATASTRUCT_POLYMESH" );
        meta_info_convert_polymesh_to_array.AddMetaInfo( "OUTPUT_DATATYPE", "DATASTRUCT_POLYMESH_ARRAY");
        if( !pass_convert_polymesh_to_array.RegisterDataTransformMetaInfo( & meta_info_convert_polymesh_to_array ) ){
            assert( 0 && "PassParsePolyMesh::RegisterDataTransformMetaInfo failed" );
            return false;
        }    

        //register passes
        if( !data_transform_driver.RegisterPass( p_pass_parse_polymesh ) ){
            assert( 0 && "DataTransformDriver::RegisterPass failed.");
            return false;
        }
        if( !data_transform_driver.RegisterPass( p_pass_convert_polymesh_to_array ) ){
            assert( 0 && "DataTransformDriver::RegisterPass failed.");
            return false;
        }

        void * data_in;
        void * data_out;
        if( !data_transform_driver.ExecutePasses( data_in, data_out ) ){
            assert( 0 && "DataTransformDriver::ExecutePasses failed.");
            return false;
        }

        PolyMesh_Data_Arrays * polymesh_data_arrays = ( PolyMesh_Data_Arrays * ) data_out;

        float * data_vertex;
        float * data_normal;
        int iNumDataVertex;
        int iNumDataNormal;
        if( !polymesh_data_arrays->Get( PolyMesh_Data_Arrays_Type::VERTEX, data_vertex, iNumDataVertex ) ){
            assert( 0 && "PolyMesh_Data_Arrays::Get failed.");
            return false;
        }
        if( !polymesh_data_arrays->Get( PolyMesh_Data_Arrays_Type::NORMAL, data_normal, iNumDataNormal ) ){
            assert( 0 && "PolyMesh_Data_Arrays::Get failed.");
            return false;
        }

        map< string, GLBufferInfo * > map_buffer_info;
        polymesh_data_arrays->GetMapBufferInfo( map_buffer_info );
        map< string, GLBufferInfoSequence * > map_buffer_info_sequence;
        polymesh_data_arrays->GetMapBufferInfoSequence( map_buffer_info_sequence );
        for( auto i : map_buffer_info ){
            _GLSLProgram->SetBufferInfo( i.second );
        }
        for( auto i : map_buffer_info_sequence ){
            _GLSLProgram->SetBufferInfoSequence( i.second );
        }
        
        if( !data_transform_driver.CleanUpPasses() ){
            assert( 0 && "DataTransformDriver::CleanUpPasses failed.");
            return false;
        }
    
        float * current_vertex = data_vertex;
        float * current_normal = data_normal;

        cout << "vertex: " << endl;
        for( int i = 0; i < iNumDataVertex; i++ ){
            cout << *current_vertex++ << " ";
        }
        cout << endl;
        cout << "normal: " << endl;
        for( int i = 0; i < iNumDataNormal; i++ ){
            cout << *current_normal++ << " ";
        }
        cout << endl;
    
        //save mapping of data
        _GLSLProgram->AddMapAttrib( "VertexPosition", pPositionData );
        _GLSLProgram->AddMapAttrib( "VertexNormal", pNormalData );
        //bind attributes
        _GLSLProgram->BindMapAttrib();

        _GLSLProgram->BindFragDataLocation( 0, "FragColor" );

        _GLSLProgram->Link();

        _GLSLProgram->PrintActiveUniforms();

        _GLSLProgram->PrintActiveAttribs();

        //generate VBO, populate and bind data to vertex attribute arrays
        pPositionData->SetData( data_vertex, 3, iNumDataVertex );
        pNormalData->SetData( data_normal, 3, iNumDataNormal );

        _GLSLProgram->Use();

        _GLSLProgram->AddNewTexture("ShadowTexture", GLTexture::DEPTH, 2500, 2500, 0, 0 );

        //deallocate data
        delete [] data_vertex;
        data_vertex = nullptr;
        delete [] data_normal;
        data_normal = nullptr;
        
        return true;
    }

    bool Render( GLSLProgram * _GLSLProgram ) {
        bool bRet;
        _dAngle += 0.005;

        mat4 Model = mat4(1.0f);
        // mat4 ModelMatrix = glm::rotate( Model, -_dAngle, vec3( 0.0f, 0.2f, 0.7f ) );
        mat4 ModelMatrix = Model;
        
        //first pass render for light POV    
        //glViewport( 0, 0, 2500, 2500 );
        mat4 ViewMatrix = glm::lookAt( vec3(0.0,0.0,20.0), 
                                       vec3(0.0,0.0,0.0),
                                       vec3(0.0,1.0,0.0) );
        
        // mat4 ViewMatrix = glm::lookAt( vec3(_fLightPos_x,_fLightPos_y,15.0), 
        //                             vec3(0.0,0.0,0.0),
        //                             vec3(0.0,1.0,0.0) );
        mat4 ProjectionMatrixLight = glm::perspective( 60.0f, 1.0f, 0.1f, 1000.0f );

        GLTexture * ShadowTexture;
        if( _GLSLProgram->GetMapTexture("ShadowTexture", ShadowTexture ) ) {
            ShadowTexture->BindFbo();
            glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
            int iActiveTexture;
            ShadowTexture->GetActiveTexture( iActiveTexture );
            bRet = _GLSLProgram->SetUniform( "ShadowMap", iActiveTexture );
        }

        // GLuint RecordDepthIndex = glGetSubroutineIndex( _GLSLProgram->GetHandle(), GL_FRAGMENT_SHADER, "recordDepth" ); 
        // glUniformSubroutinesuiv( GL_FRAGMENT_SHADER, 1, &RecordDepthIndex);

        //draw on first pass
        //Multiply it by the bias matrix
        glm::mat4 Bias(
            0.5, 0.0, 0.0, 0.0,
            0.0, 0.5, 0.0, 0.0,
            0.0, 0.0, 0.5, 0.0,
            0.5, 0.5, 0.5, 1.0
            );
        
        mat4 ModelViewMatrix = ViewMatrix  * ModelMatrix;
        mat4 MVP = ProjectionMatrixLight * ViewMatrix  * ModelMatrix;
        mat4 MVPB = Bias * ProjectionMatrixLight * ViewMatrix * ModelMatrix;
        mat3 NormalMatrix = glm::inverse( glm::transpose( glm::mat3(ModelViewMatrix) ) );

        vec3 LightLa;
        vec3 LightLd;
        vec3 LightLs;
        LightLa = vec3( 0.05, 0.05, 0.05 );
        LightLd = vec3( 0.5, 0.5, 0.5 );
        LightLs = vec3( 0.45, 0.45, 0.45 );
        bRet = _GLSLProgram->SetUniform( "Light.La", LightLa );
        bRet = _GLSLProgram->SetUniform( "Light.Ld", LightLd );
        bRet = _GLSLProgram->SetUniform( "Light.Ls", LightLs );
        float fDeltaLight = -0.01;
        vec4 LightPosition( 0.0f, 0.0f, 20.0f, 1.0 );
        // vec4 LightPosition( _fLightPos_x, _fLightPos_y, 15.0f, 1.0 );
        _fLightPos_x += fDeltaLight;
        _fLightPos_y += fDeltaLight;
        bRet = _GLSLProgram->SetUniform( "Light.Position", LightPosition );

        mat4 LightViewMatrixOriginal = ViewMatrix;
        
        vec3 MaterialCoeffKa( 1.0f, 1.0f, 1.0f );
        vec3 MaterialCoeffKd( 1.0f, 1.0f, 1.0f );
        vec3 MaterialCoeffKs( 1.0f, 1.0f, 1.0f );
        bRet = _GLSLProgram->SetUniform( "Material.Ka", MaterialCoeffKa );
        bRet = _GLSLProgram->SetUniform( "Material.Kd", MaterialCoeffKd );
        bRet = _GLSLProgram->SetUniform( "Material.Ks", MaterialCoeffKs );
        bRet = _GLSLProgram->SetUniform( "Material.Shininess", 2.0f );

        ///start of draw 1st object
        RenderMeshOrientation render_mesh_orientation_firstpass;
        render_mesh_orientation_firstpass._MatOrientation = ModelMatrix;
        render_mesh_orientation_firstpass._MatView = ViewMatrix;
        render_mesh_orientation_firstpass._MatProjection = ProjectionMatrixLight;
        render_mesh_orientation_firstpass._MatLightProjection = ProjectionMatrixLight;
        render_mesh_orientation_firstpass._MatLightView = LightViewMatrixOriginal;
        render_mesh_orientation_firstpass.ComputeCompositeMats();

        GLRenderPassShadowMap render_pass_shadow_map;
        list<string> buffer_obj_name_pass_depth;
        buffer_obj_name_pass_depth.push_back( "square" );
        render_pass_shadow_map.AddPath( "DEPTH", render_mesh_orientation_firstpass, buffer_obj_name_pass_depth );
        ///end of draw 1st object

        //set orientation for the objects to render below
        mat4 ObjOrientationMatrix = glm::rotate( Model, -2*_dAngle, vec3( 0.0f, 0.5f, 0.7f ) );
        mat4 ModelOrientationViewMatrix = ViewMatrix * ObjOrientationMatrix * ModelMatrix;
        mat4 MOVP = ProjectionMatrixLight * ViewMatrix  * ObjOrientationMatrix * ModelMatrix;
        mat4 MOVPB = Bias * ProjectionMatrixLight * ViewMatrix * ObjOrientationMatrix * ModelMatrix;
        mat3 NormalMatrixOrientation = glm::inverse( glm::transpose( glm::mat3(ModelOrientationViewMatrix) ) );

        ///start of draw 2nd object
        render_mesh_orientation_firstpass._MatOrientation = ObjOrientationMatrix;
        render_mesh_orientation_firstpass._MatView = ViewMatrix;
        render_mesh_orientation_firstpass._MatProjection = ProjectionMatrixLight;
        render_mesh_orientation_firstpass._MatLightProjection = ProjectionMatrixLight;
        render_mesh_orientation_firstpass._MatLightView = LightViewMatrixOriginal;
        render_mesh_orientation_firstpass.ComputeCompositeMats();

        buffer_obj_name_pass_depth.clear();
        buffer_obj_name_pass_depth.push_back( "Wheel_4_Wheel" );
        render_pass_shadow_map.AddPath( "DEPTH", render_mesh_orientation_firstpass, buffer_obj_name_pass_depth );
        //end of draw 2nd object
        render_pass_shadow_map.ProcessPass( "DEPTH", _GLSLProgram ); //this needs to be done prior to changing glViewport dimension for second pass rendering
        
        //2nd pass render
        //glViewport( 0, 0, 500, 500 );
        ViewMatrix = glm::lookAt( vec3(-5.0,-5.0,8.0), 
                                  vec3(0.0,0.0,0.0),
                                  vec3(0.0,1.0,0.0) );
        
        mat4 ViewOrientationMatrix = glm::rotate( Model, -_dAngle, vec3( 0.0f, 0.2f, 0.7f ) );
        ViewMatrix = ViewMatrix * ViewOrientationMatrix;
                
        //draw on 2nd pass
        ModelViewMatrix = ViewMatrix * ModelMatrix;
        mat4 ProjectionMatrix = glm::perspective( 90.0f, 1.0f, 0.1f, 500.0f );
        MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
        NormalMatrix = glm::inverse( glm::transpose( glm::mat3(ModelViewMatrix) ) );
                
        ///draw 1st object
        RenderMeshOrientation render_mesh_orientation_secondpass;
        render_mesh_orientation_secondpass._MatOrientation = ModelMatrix;
        render_mesh_orientation_secondpass._MatView = ViewMatrix;
        render_mesh_orientation_secondpass._MatProjection = ProjectionMatrix;
        render_mesh_orientation_secondpass._MatLightProjection = ProjectionMatrixLight;
        render_mesh_orientation_secondpass._MatLightView = LightViewMatrixOriginal;
        render_mesh_orientation_secondpass.ComputeCompositeMats();

        buffer_obj_name_pass_depth.clear();
        buffer_obj_name_pass_depth.push_back( "square" );
        render_pass_shadow_map.AddPath( "NORMAL", render_mesh_orientation_secondpass, buffer_obj_name_pass_depth );
        ///end of draw 1st object

        ///draw 2nd object
        render_mesh_orientation_secondpass._MatOrientation = ObjOrientationMatrix;
        render_mesh_orientation_secondpass._MatView = ViewMatrix;
        render_mesh_orientation_secondpass._MatProjection = ProjectionMatrix;
        render_mesh_orientation_secondpass._MatLightProjection = ProjectionMatrixLight;
        render_mesh_orientation_secondpass._MatLightView = LightViewMatrixOriginal;
        render_mesh_orientation_secondpass.ComputeCompositeMats();

        buffer_obj_name_pass_depth.clear();
        buffer_obj_name_pass_depth.push_back( "Wheel_4_Wheel" );
        render_pass_shadow_map.AddPath( "NORMAL", render_mesh_orientation_secondpass, buffer_obj_name_pass_depth );
        ///end of draw 2nd object

        //render all objects for the normal pass
        if( _GLSLProgram->GetMapTexture("ShadowTexture", ShadowTexture ) ) {
            ShadowTexture->UnbindFbo();
        }
        glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
        bRet = _GLSLProgram->SetUniform( "Light.Position", LightPosition );
        render_pass_shadow_map.ProcessPass( "NORMAL", _GLSLProgram );

        return true;
    }  
};

#endif
