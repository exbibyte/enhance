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

void InitWindow( enGameData * game_data, string strPathPolyMesh ){
    glfwMakeContextCurrent( game_data->_Window );
    GLPrintInfo();

    glEnable(GL_DEPTH_TEST);
    glClearColor(0, 0, 0, 1.0);

    enScene * scene = game_data->_Scenes["DEFAULT"];
    GLSceneManager * scene_manager = game_data->_SceneManagers["DEFAULT"];
    
    scene->_strPathPolyMesh = strPathPolyMesh;    
    scene_manager->SetGLSLProgram( game_data->_ProgramGlsl );
    
    std::function< bool( GLSLProgram * ) > func_wrap_init_01 = bind( &enScene::SetShaders, scene, std::placeholders::_1 );
    std::function< bool( GLSLProgram * ) > func_wrap_body_01 = bind( &enScene::Render, scene, std::placeholders::_1 );
    std::function< bool( GLSLProgram * ) > func_wrap_body_02 = func_body_02;
    std::function< bool( GLSLProgram * ) > func_wrap_cleanup_01 = func_cleanup_01;
    scene_manager->RegisterRoutine( "init_01", func_wrap_init_01, GLSceneRoutineType::INIT );
    scene_manager->RegisterRoutine( "body_01", func_wrap_body_01, GLSceneRoutineType::BODY );
    scene_manager->RegisterRoutine( "body_02", func_wrap_body_02, GLSceneRoutineType::BODY );
    scene_manager->UnregisterRoutine( "body_02" );
    scene_manager->RegisterRoutine( "cleanup_01", func_wrap_cleanup_01, GLSceneRoutineType::CLEANUP );

    scene_manager->RunInit();
}

void RenderScene( enGameData * game_data ){
    glfwMakeContextCurrent( game_data->_Window ); // this is need when calling rendering APIs on separate thread
    game_data->_SceneManagers["DEFAULT"]->RunBody();
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
