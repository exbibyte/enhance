#ifndef EN_GAME_MAIN_H
#define EN_GAME_MAIN_H

//opengl includes
#include <GLFW/glfw3.h>

#include "GLSceneManager.h"
#include "GLSLProgram.h"
#include "WindowManagerGlfw.h"
#include "WingedEdge.h"
#include "enTPCommon.h"
#include "enScene.h"
#include "enSceneSample.h"
#include "enGameData.h"
#include "enRenderPass.h"

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

bool bSignalExit = false;

static void error_callback(int error, const char* description)
{
    fputs(description, stderr);
}
bool key_callback_0()
{
    cout << "triggered key_callback_0" << endl;
    return true;
}
bool key_callback_1()
{
    cout << "triggered key_callback_1" << endl;
    bSignalExit = true;
    return true;
}

auto func_body_02 = []( GLSLProgram * glslprogram )->bool {
    cout << "func body 02" << endl;
    return true;
};
auto func_cleanup_01 = []( GLSLProgram * glslprogram )->bool {
    cout << "func cleanup 01" << endl;
    return true;
};

bool SceneInit( enGameData * game_data, enScene * scene_data, GLSLProgram * _GLSLProgram ){
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
    string strFilePathPolyMesh = scene_data->_strPathPolyMesh;
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

    // cout << "vertex: " << endl;
    // for( int i = 0; i < iNumDataVertex; i++ ){
    // 	cout << *current_vertex++ << " ";
    // }
    // cout << endl;
    // cout << "normal: " << endl;
    // for( int i = 0; i < iNumDataNormal; i++ ){
    // 	cout << *current_normal++ << " ";
    // }
    // cout << endl;
    
    //save mapping of data
    _GLSLProgram->AddMapAttrib( "VertexPosition", pPositionData );
    _GLSLProgram->AddMapAttrib( "VertexNormal", pNormalData );
    //bind the above attributes to vertex array object
    _GLSLProgram->BindMapAttrib();

    _GLSLProgram->BindFragDataLocation( 0, "FragColor" );

    _GLSLProgram->Link();

    _GLSLProgram->PrintActiveUniforms();

    _GLSLProgram->PrintActiveAttribs();

    _GLSLProgram->Use();
    
    _GLSLProgram->AddNewTexture("ShadowTexture", GLTexture::DEPTH, 2500, 2500, 0, 0 );

    cout << "After linking program" << endl;
    //generate VBO, populate and bind data to vertex attribute arrays
//    pPositionData->SetData( data_vertex, 3, iNumDataVertex );
//    pNormalData->SetData( data_normal, 3, iNumDataNormal );

    vector<double> vert_pos;
    vector<double> vert_norm;
    for( int i = 0; i < iNumDataVertex; ++i ){
	vert_pos.push_back( data_vertex[i] );
    }
    for( int i = 0; i < iNumDataNormal; ++i ){
	vert_norm.push_back( data_normal[i] );
    }
    cout << "Size of vert_pos: " << vert_pos.size() << endl;
    cout << "Size of vert_norm: " << vert_norm.size() << endl;

    game_data->_render_pass->AddToProcess( eRenderType::POLY_VERT, vert_pos );
    game_data->_render_pass->AddToProcess( eRenderType::POLY_NORM, vert_norm );
    
   //deallocate data
    delete [] data_vertex;
    data_vertex = nullptr;
    delete [] data_normal;
    data_normal = nullptr;

    cout << "End of Init Phase" << endl;

    return true;
}

bool SceneRender( enGameData * game_data, enScene * scene_data, GLSLProgram * _GLSLProgram ){
    bool bRet;
    scene_data->_dAngle += 0.005;

    enRenderPass_ShadowMap_OpGL * render = game_data->_render_pass;

    mat4 Model = mat4(1.0f);
    mat4 ModelMatrix = Model;
        
    //first pass render for light POV    
    glViewport( 0, 0, 2500, 2500 );
    mat4 ViewMatrix = glm::lookAt( vec3(0.0,0.0,20.0), 
				   vec3(0.0,0.0,0.0),
				   vec3(0.0,1.0,0.0) );

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
    scene_data->_fLightPos_x += fDeltaLight;
    scene_data->_fLightPos_y += fDeltaLight;
    bRet = _GLSLProgram->SetUniform( "Light.Position", LightPosition );

    mat4 LightViewMatrixOriginal = ViewMatrix;
        
    vec3 MaterialCoeffKa( 1.0f, 1.0f, 1.0f );
    vec3 MaterialCoeffKd( 1.0f, 1.0f, 1.0f );
    vec3 MaterialCoeffKs( 1.0f, 1.0f, 1.0f );
    bRet = _GLSLProgram->SetUniform( "Material.Ka", MaterialCoeffKa );
    bRet = _GLSLProgram->SetUniform( "Material.Kd", MaterialCoeffKd );
    bRet = _GLSLProgram->SetUniform( "Material.Ks", MaterialCoeffKs );
    bRet = _GLSLProgram->SetUniform( "Material.Shininess", 2.0f );

    //set render matrices for 1st pass
    RenderMeshOrientation render_mesh_orientation_firstpass;
    //set orientation for the objects to render below
    mat4 ObjOrientationMatrix = glm::rotate( Model, -2 * scene_data->_dAngle, vec3( 0.0f, 0.5f, 0.7f ) );
    render_mesh_orientation_firstpass._MatOrientation = ModelMatrix;
    render_mesh_orientation_firstpass._MatView = ViewMatrix;
    render_mesh_orientation_firstpass._MatProjection = ProjectionMatrixLight;
    render_mesh_orientation_firstpass._MatLightProjection = ProjectionMatrixLight;
    render_mesh_orientation_firstpass._MatLightView = LightViewMatrixOriginal;
    render_mesh_orientation_firstpass.ComputeCompositeMats();

    //get computed matrices for rendering
    mat4 model_view_matrix;
    mat4 model_view_proj_matrix;
    mat4 model_view_pro_bias_matrix;
    mat3 normal_matrix;
    mat4 light_view = LightViewMatrixOriginal;
    render_mesh_orientation_firstpass.GetCompositeMats( model_view_matrix, model_view_proj_matrix, model_view_pro_bias_matrix, normal_matrix );
    render->AddToProcess( eRenderType::MODEL_VIEW_MATRIX, model_view_matrix );
    render->AddToProcess( eRenderType::MODEL_VIEW_PERSPECTIVE_MATRIX, model_view_proj_matrix );
    render->AddToProcess( eRenderType::MODEL_VIEW_PERSPECTIVE_BIAS_MATRIX, model_view_pro_bias_matrix );
    render->AddToProcess( eRenderType::NORMAL_MATRIX, normal_matrix );
    render->AddToProcess( eRenderType::LIGHT_VIEW_MATRIX, light_view );

    render->ProcessNow( _GLSLProgram, "DEPTH" );
    render->Clear();
    
    //2nd pass render
    glViewport( 0, 0, 500, 500 );
    ViewMatrix = glm::lookAt( vec3(-5.0,-5.0,8.0), 
			      vec3(0.0,0.0,0.0),
			      vec3(0.0,1.0,0.0) );
        
    mat4 ViewOrientationMatrix = glm::rotate( Model, - scene_data->_dAngle, vec3( 0.0f, 0.2f, 0.7f ) );
    ViewMatrix = ViewMatrix * ViewOrientationMatrix;
                
    //draw on 2nd pass
    ModelViewMatrix = ViewMatrix * ModelMatrix;
    mat4 ProjectionMatrix = glm::perspective( 90.0f, 1.0f, 0.1f, 500.0f );

    //set rendering matrices
    RenderMeshOrientation render_mesh_orientation_secondpass;
    render_mesh_orientation_secondpass._MatOrientation = ModelMatrix;
    render_mesh_orientation_secondpass._MatView = ViewMatrix;
    render_mesh_orientation_secondpass._MatProjection = ProjectionMatrix;
    render_mesh_orientation_secondpass._MatLightProjection = ProjectionMatrixLight;
    render_mesh_orientation_secondpass._MatLightView = LightViewMatrixOriginal;
    render_mesh_orientation_secondpass.ComputeCompositeMats();

    //get computed matrices for rendering
    mat4 model_view_matrix_2nd;
    mat4 model_view_proj_matrix_2nd;
    mat4 model_view_pro_bias_matrix_2nd;
    mat3 normal_matrix_2nd;
    mat4 light_view_2nd = LightViewMatrixOriginal;
    render_mesh_orientation_secondpass.GetCompositeMats( model_view_matrix_2nd, model_view_proj_matrix_2nd, model_view_pro_bias_matrix_2nd, normal_matrix_2nd );
    render->AddToProcess( eRenderType::MODEL_VIEW_MATRIX, model_view_matrix_2nd );
    render->AddToProcess( eRenderType::MODEL_VIEW_PERSPECTIVE_MATRIX, model_view_proj_matrix_2nd );
    render->AddToProcess( eRenderType::MODEL_VIEW_PERSPECTIVE_BIAS_MATRIX, model_view_pro_bias_matrix_2nd );
    render->AddToProcess( eRenderType::NORMAL_MATRIX, normal_matrix_2nd );
    render->AddToProcess( eRenderType::LIGHT_VIEW_MATRIX, light_view_2nd );

    //render all objects for 2nd pass
    if( _GLSLProgram->GetMapTexture("ShadowTexture", ShadowTexture ) ) {
	ShadowTexture->UnbindFbo();
    }
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
    bRet = _GLSLProgram->SetUniform( "Light.Position", LightPosition );

    render->ProcessNow( _GLSLProgram, "NORMAL" );
    render->Clear();
    
    return true;
}

void InitWindow( enGameData * game_data, string strPathPolyMesh ){

    //instance manager setup for game entities
    game_data->_InstanceManagerPackage->CreateManager( eInstanceType::PolyVertices, {} );
    game_data->_InstanceManagerPackage->CreateManager( eInstanceType::LightAmbient, {} );
    game_data->_InstanceManagerPackage->CreateManager( eInstanceType::LightSpectral, {} );
    game_data->_InstanceManagerPackage->CreateManager( eInstanceType::LightDiffuse, {} );
    game_data->_InstanceManagerPackage->CreateManager( eInstanceType::CameraProjection, {} );
    game_data->_InstanceManagerPackage->CreateManager( eInstanceType::MaterialAmbient, {} );
    game_data->_InstanceManagerPackage->CreateManager( eInstanceType::MaterialDiffuse, {} );
    game_data->_InstanceManagerPackage->CreateManager( eInstanceType::MaterialSpectral, {} );
    game_data->_InstanceManagerPackage->CreateManager( eInstanceType::MaterialShininess, {} );
    game_data->_InstanceManagerPackage->CreateManager( eInstanceType::OrientOffset, {} );
    game_data->_InstanceManagerPackage->CreateManager( eInstanceType::OrientRotation, {} );
    game_data->_InstanceManagerPackage->CreateManager( eInstanceType::LightComposition, { eInstanceType::LightAmbient, eInstanceType::LightSpectral, eInstanceType::LightDiffuse } );
    game_data->_InstanceManagerPackage->CreateManager( eInstanceType::MaterialComposition, { eInstanceType::MaterialAmbient, eInstanceType::MaterialDiffuse, eInstanceType::MaterialSpectral, eInstanceType::MaterialShininess } );
    game_data->_InstanceManagerPackage->CreateManager( eInstanceType::EntityOrientation, { eInstanceType::OrientOffset, eInstanceType::OrientRotation } );
    game_data->_InstanceManagerPackage->CreateManager( eInstanceType::EntityPackage, { eInstanceType::PolyVertices, eInstanceType::EntityOrientation, eInstanceType::MaterialComposition  } );
    game_data->_InstanceManagerPackage->LinkManagers();

    glfwMakeContextCurrent( game_data->_Window );
    GLPrintInfo();

    glEnable(GL_DEPTH_TEST);
    glClearColor(0, 0, 0, 1.0);

    enScene * scene = game_data->_Scenes["DEFAULT"];
    GLSceneManager * scene_manager = game_data->_SceneManagers["DEFAULT"];
    
    scene->_strPathPolyMesh = strPathPolyMesh;    
    scene_manager->SetGLSLProgram( game_data->_ProgramGlsl );
    
    std::function< bool( GLSLProgram * ) > func_wrap_init_01 = bind( SceneInit, game_data, scene, std::placeholders::_1 );
    std::function< bool( GLSLProgram * ) > func_wrap_body_01 = bind( SceneRender, game_data, scene, std::placeholders::_1 );
    std::function< bool( GLSLProgram * ) > func_wrap_body_02 = func_body_02;
    std::function< bool( GLSLProgram * ) > func_wrap_cleanup_01 = func_cleanup_01;
    scene_manager->RegisterRoutine( "init_LoadShaders", func_wrap_init_01, GLSceneRoutineType::INIT );
    scene_manager->RegisterRoutine( "body_RenderScene", func_wrap_body_01, GLSceneRoutineType::BODY );
    scene_manager->RegisterRoutine( "body_02", func_wrap_body_02, GLSceneRoutineType::BODY );
    scene_manager->UnregisterRoutine( "body_02" );
    scene_manager->RegisterRoutine( "cleanup_01", func_wrap_cleanup_01, GLSceneRoutineType::CLEANUP );

    scene_manager->RunInit();
}

void RenderScene( enGameData * game_data ){
    glfwMakeContextCurrent( game_data->_Window ); // this is need when calling rendering APIs on separate thread
    // game_data->_SceneManagers["DEFAULT"]->RunBody();
    bool bRet = game_data->_SceneManagers["DEFAULT"]->RunBodySpecified("body_RenderScene");
    if( !bRet ){
	cout << "Rendering call returned false" << endl;
    }
    glfwSwapBuffers( game_data->_Window );
}

void ClockTask( enGameData * game_data ) {
    Clock render_clock;
    render_clock.SetFps(28);
    bool bClockTicked = false;
    auto FuncClockTicked = [ &bClockTicked ](){
	bClockTicked = true;
    };
    render_clock.SetTickFunc( FuncClockTicked );
    render_clock.Run();
    
    while( !glfwWindowShouldClose( game_data->_Window ) ){
	if( bSignalExit ){
            break;
        }
	render_clock.Tick();
	if( bClockTicked ){
	    std::future<void> ret = game_data->_Threadpool->AddTask( RenderScene, game_data ); //notify render task
	    ret.get();  
	    bClockTicked = false;
	}
    }
    render_clock.Pause();
}

int main( int argc, char ** argv ){

    if( argc < 2 ){
        assert( 0 && "Require argument: path_to_polymesh_file" );
        return -1;
    }

    string strPathPolyMesh = string( argv[1] );

    //init window
    if (!glfwInit())
        exit(EXIT_FAILURE);

    glfwSetErrorCallback(error_callback);
    
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);  // yes, 3 and 2!!!
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_DEPTH_BITS,24);

    //get version
    int major, minor, rev;
    glfwGetVersion( &major, &minor, &rev );
    cout<< "major: "<<major<<", minor: "<<minor<<", rev: "<<rev<<endl;

    //create winow and initialize game data managers
    int iWindowId = 1;
    enGameData game_data( 1, 500, 500, "EN_GAME_MAIN" );
    enGameData * p_game_data = & game_data;
    GLFWwindow * window = game_data._Window;
    glfwMakeContextCurrent(window);

    //add key combo and associated callback
    map<KeyButtonWhich::Enum, KeyButtonState::Enum> map_key_combo {
        { KeyButtonWhich::KEY_J, KeyButtonState::DOWN },
        { KeyButtonWhich::KEY_A, KeyButtonState::DOWN }      
    };
    if( !game_data._ManagerWindow->SetKeyComboCallback( map_key_combo, key_callback_0 ) ){
        cout << "Error setting key combo callback" << endl;
        glfwTerminate();
        exit( EXIT_FAILURE );
    }
    map<KeyButtonWhich::Enum, KeyButtonState::Enum> map_key_combo_exit {
        { KeyButtonWhich::KEY_L, KeyButtonState::DOWN }
    };
    if( !game_data._ManagerWindow->SetKeyComboCallback( map_key_combo_exit, key_callback_1 ) ){
        cout << "Error setting key combo callback" << endl;
        glfwTerminate();
        exit( EXIT_FAILURE );
    }
    //register callback
    if( !game_data._ManagerWindow->SetDefaultCb() ){
        cout << "Error registering key combo callback" << endl;
        exit( EXIT_FAILURE );
    }

    std::future<void> ret1 = game_data._Threadpool->AddTask(InitWindow, p_game_data, strPathPolyMesh );
    int count = 0;

    game_data._Threadpool->RunThreads();
    
    std::this_thread::sleep_for (std::chrono::seconds(3));
    std::future<void> retClock = game_data._Threadpool->AddTask(ClockTask, p_game_data );

    while( !glfwWindowShouldClose( window ) )
    {
        if( bSignalExit ){
            break;
        }
        glfwPollEvents();
        game_data._ManagerWindow->ProcessKeyButtonCombo();
    }
    ret1.get();
    retClock.get();

    game_data._Threadpool->EndAllThreads();
    
    glfwDestroyWindow(window);
    glfwTerminate();
    exit(EXIT_SUCCESS);
  
    return 0;
}


#endif
