#ifndef EN_GAME_MAIN_H
#define EN_GAME_MAIN_H

//opengl includes
#include <GL/gl3w.h>
//#include <GLFW/glfw3.h>

#include "GLSceneManager.h"
#include "GLSLProgram.h"
#include "WindowManagerGlfw.h"
#include "WingedEdge.h"
#include "enTPCommon.h"
#include "enScene.h"
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
#include "RenderMeshOrientation.h"

#include <imgui.h>
#include "imgui_impl_glfw_gl3.h"
#include <GLFW/glfw3.h>
// #include "imgui_impl_glfw.h"

//testing starts
#include "RenderLight.h"
#include "RenderCamera.h"
#include "RenderEntity.h"
#include "RenderContext.h"
//testing ends

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

ImVec4 clear_color = ImColor(60, 60, 90);

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

    // game_data->_render_pass->AddToProcess( eRenderType::POLY_VERT, vert_pos );
    // game_data->_render_pass->AddToProcess( eRenderType::POLY_NORM, vert_norm );
    game_data->_temporary_game_data[ "POLY_VERT" ] = vert_pos;
    game_data->_temporary_game_data[ "POLY_NORM" ] = vert_norm;
	
   //deallocate data
    delete [] data_vertex;
    data_vertex = nullptr;
    delete [] data_normal;
    data_normal = nullptr;

    game_data->_temporary_game_data["Entity_RotateAngle"] = vector<double>{ 0 };
    
    cout << "End of Init Phase" << endl;

    return true;
}

bool SceneRender( enGameData * game_data, enScene * scene_data, GLSLProgram * _GLSLProgram ){
    glfwMakeContextCurrent( game_data->_Window ); // this is need when calling rendering APIs on separate thread
    bool bRet;

    enRenderPass_ShadowMap_OpGL * render = game_data->_render_pass;

    //light ----------------------------------------------------------------------
    vector<double> light_position     { 0, 0, 10 };
    vector<double> light_lookat       { 0, 0, 0 };
    vector<double> light_up           { 0, 1, 0 };
    vector<double> light_perspective  { 60.0f, 1.0f, 0.1f, 1000.0f };
    vector<double> light_ambient      { 0.05f, 0.05f, 0.05f };
    vector<double> light_diffuse      { 0.5f, 0.5f, 0.5f };
    vector<double> light_specular     { 0.45f, 0.45f, 0.45f };

    //camera ---------------------------------------------------------------------
    vector<double> camera_position    { -5, -5, 8.0 };
    vector<double> camera_lookat      { 0, 0, 0 };
    vector<double> camera_up          { 0, 1, 0 };
    vector<double> camera_perspective { 90.0f, 1.0f, 0.1f, 500.0f };
    vector<double> camera_ambient     { 0.05f, 0.05f, 0.05f };
    vector<double> camera_diffuse     { 0.5f, 0.5f, 0.5f };
    vector<double> camera_specular    { 0.45f, 0.45f, 0.45f };

    //context --------------------------------------------------------------------
    vector<int> context_windowsize { 500, 500 };
    vector<int> context_texturesize_shadowmap { 2500, 2500 };
    string context_title = "ENHANCE_WIP";

    //entities -------------------------------------------------------------------
    //orientation	
    vector<double> entity_translate    { 0, 0, 0};
    vector<double> entity_rotate_axis  { 1, 0, 0};
    // vector<double> entity_rotate_angle { 0 };
    double rotation_angle = game_data->_temporary_game_data["Entity_RotateAngle"][0];
    vector<double> entity_rotate_angle { rotation_angle };
    //set material data
    vector<double> entity_material_ambient   { 1.0, 1.0, 1.0 };
    vector<double> entity_material_diffuse   { 1, 1, 1 };
    vector<double> entity_material_specular  { 1, 1, 1 };
    vector<double> entity_material_shininess { 2 };
    //set vertex data 
    vector<double> entity_vertices = game_data->_temporary_game_data[ "POLY_VERT" ];
    vector<double> entity_normals = game_data->_temporary_game_data[ "POLY_NORM" ];
	
    RenderLight * light = new RenderLight;;
    light->AddDataSingle( RenderLightData::Coordinate(),  light_position    );
    light->AddDataSingle( RenderLightData::Lookat(),      light_lookat      );
    light->AddDataSingle( RenderLightData::Up(),          light_up          );
    light->AddDataSingle( RenderLightData::Perspective(), light_perspective );
    light->AddDataSingle( RenderLightData::Ambient(),     light_ambient     );
    light->AddDataSingle( RenderLightData::Diffuse(),     light_diffuse     );
    light->AddDataSingle( RenderLightData::Specular(),    light_specular    );

    RenderCamera * camera = new RenderCamera;
    camera->AddDataSingle( RenderCameraData::Coordinate(),  camera_position    );
    camera->AddDataSingle( RenderCameraData::Lookat(),      camera_lookat      );
    camera->AddDataSingle( RenderCameraData::Up(),          camera_up          );
    camera->AddDataSingle( RenderCameraData::Perspective(), camera_perspective );
    camera->AddDataSingle( RenderCameraData::Ambient(),     camera_ambient     );
    camera->AddDataSingle( RenderCameraData::Diffuse(),     camera_diffuse     );
    camera->AddDataSingle( RenderCameraData::Specular(),    camera_specular    );

    RenderContext * context = new RenderContext;
    context->AddDataSingle( RenderContextData::WindowSize(),           context_windowsize );
    context->AddDataSingle( RenderContextData::TextureSizeShadowMap(), context_texturesize_shadowmap );
    context->AddDataSingle( RenderContextData::Title(),                context_title );

    list< RenderEntity * > * entities = new list< RenderEntity * >;
    RenderEntity * entity_01 = new RenderEntity;
    entity_01->AddDataSingle( RenderPolyData::Coordinate(),    entity_translate );
    entity_01->AddDataSingle( RenderPolyData::RotationAxis(),  entity_rotate_axis );
    entity_01->AddDataSingle( RenderPolyData::RotationAngle(), entity_rotate_angle );

    entity_01->AddDataSingle( RenderMaterialData::Ambient(),   entity_material_ambient );
    entity_01->AddDataSingle( RenderMaterialData::Diffuse(),   entity_material_diffuse );
    entity_01->AddDataSingle( RenderMaterialData::Specular(),  entity_material_specular );
    entity_01->AddDataSingle( RenderMaterialData::Shininess(), entity_material_shininess );
    entity_01->AddDataSingle( RenderVertexData::Normals(),  entity_normals );
    entity_01->AddDataSingle( RenderVertexData::Vertices(), entity_vertices );

    entities->push_back( entity_01 );

    render->Process( _GLSLProgram, entities, light, camera, context );

    delete entity_01;
    delete entities;
    delete context;
    delete camera;
    delete light;

    return true;
}

void InitWindow( enGameData * game_data, string strPathPolyMesh ){

    // //instance manager setup for game entities
    // game_data->_InstanceManagerPackage->CreateManager( eInstanceType::PolyVertices, {} );
    // game_data->_InstanceManagerPackage->CreateManager( eInstanceType::LightAmbient, {} );
    // game_data->_InstanceManagerPackage->CreateManager( eInstanceType::LightSpectral, {} );
    // game_data->_InstanceManagerPackage->CreateManager( eInstanceType::LightDiffuse, {} );
    // game_data->_InstanceManagerPackage->CreateManager( eInstanceType::CameraProjection, {} );
    // game_data->_InstanceManagerPackage->CreateManager( eInstanceType::MaterialAmbient, {} );
    // game_data->_InstanceManagerPackage->CreateManager( eInstanceType::MaterialDiffuse, {} );
    // game_data->_InstanceManagerPackage->CreateManager( eInstanceType::MaterialSpectral, {} );
    // game_data->_InstanceManagerPackage->CreateManager( eInstanceType::MaterialShininess, {} );
    // game_data->_InstanceManagerPackage->CreateManager( eInstanceType::OrientOffset, {} );
    // game_data->_InstanceManagerPackage->CreateManager( eInstanceType::OrientRotation, {} );
    // game_data->_InstanceManagerPackage->CreateManager( eInstanceType::LightComposition, { eInstanceType::LightAmbient, eInstanceType::LightSpectral, eInstanceType::LightDiffuse } );
    // game_data->_InstanceManagerPackage->CreateManager( eInstanceType::MaterialComposition, { eInstanceType::MaterialAmbient, eInstanceType::MaterialDiffuse, eInstanceType::MaterialSpectral, eInstanceType::MaterialShininess } );
    // game_data->_InstanceManagerPackage->CreateManager( eInstanceType::EntityOrientation, { eInstanceType::OrientOffset, eInstanceType::OrientRotation } );
    // game_data->_InstanceManagerPackage->CreateManager( eInstanceType::EntityPackage, { eInstanceType::PolyVertices, eInstanceType::EntityOrientation, eInstanceType::MaterialComposition  } );
    // game_data->_InstanceManagerPackage->LinkManagers();

    glfwMakeContextCurrent( game_data->_Window );

    gl3wInit();

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
    //Imgui initialization starts
    bool bInstallCallback = false;
    ImGui_ImplGlfwGL3_Init( game_data->_Window, bInstallCallback );
    //Imgui initialization ends
}

void RenderScene( enGameData * game_data ){
    glfwMakeContextCurrent( game_data->_Window ); // this is need when calling rendering APIs on separate thread

    //update rotation angle of entity    
    game_data->_temporary_game_data["Entity_RotateAngle"][0] += 0.05;

    bool bRet = game_data->_SceneManagers["DEFAULT"]->RunBodySpecified("body_RenderScene");
    if( !bRet ){
    	cout << "Rendering call returned false" << endl;
    }

    //Imgui renderig starts
    ImGui_ImplGlfwGL3_NewFrame();
    {
    	ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiSetCond_FirstUseEver);
    	ImGui::SetNextWindowSize(ImVec2(120,70), ImGuiSetCond_FirstUseEver);
    	ImGui::Begin("Status UI");
    	ImGui::ColorEdit3("clear color", (float*)&clear_color);
    	ImGui::Text("Rendering: %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
    	ImGui::End();

    	int display_w, display_h;
        glfwGetFramebufferSize( game_data->_Window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
    }
    ImGui::Render();
    //Imgui rendering ends

    glfwSwapBuffers( game_data->_Window );
}

void ClockTask( enGameData * game_data ) {
    Clock render_clock;
    render_clock.SetFps(25);
    // render_clock.SetFps(0.1);
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

    std::this_thread::sleep_for (std::chrono::seconds(1));
    
    return 0;
}


#endif

