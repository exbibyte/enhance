//opengl includes
#include <GLFW/glfw3.h>

//application specific
#include "enTPCommon.h"
#include "GLTexture.h"
#include "GLSLProgram.h"
#include "GLAttribData.h"
#include "GLRender.h"
#include "textfile.h"
#include "WindowManagerGlfw.h"

//math library
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/transform2.hpp>
using glm::mat4;
using glm::vec3;

//STL
#include <iostream>
#include <future>
#include <math.h>
#include <utility>
#include <type_traits>
#include <vector>
#include <chrono>
#include <thread>
#include <functional>
#include <map>
using namespace std;


WindowManagerGlfw * manager_window;
GLFWwindow * window;
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

void RenderTask(int argc, char **argv) {

    glfwMakeContextCurrent( window );
    GLPrintInfo();

    glEnable(GL_DEPTH_TEST);
    glClearColor(0, 0, 0, 1.0);

    GLRender::SetShaders();
     
    while (!glfwWindowShouldClose(window))
    {
	if( bSignalExit ){
	    break;
	}
        GLRender::RenderScene();
        glfwSwapBuffers(window);
    }
}

void Idle( int count, enTPCommon * Pool ){
    if( bSignalExit ){
	return;
    }
    cout << count << endl;
    ++count;
    Pool->AddTask( Idle, count, Pool );
}

int main(int argc, char **argv) {

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

  //create window
  manager_window = new WindowManagerGlfw;
  int id_window = 1;
  if( !manager_window->CreateWindow( id_window, 500, 500, "Shadow Test" ) ){
      cout << "Error creating window" << endl;
      glfwTerminate();
      exit( EXIT_FAILURE );
  }
  if( !manager_window->GetWindow( window ) ){
      cout << "Error getting window" << endl;
      glfwTerminate();
      exit( EXIT_FAILURE );
  }
  
  glfwMakeContextCurrent(window);

  //add key combo and associated callback
  map<KeyButtonWhich::Enum, KeyButtonState::Enum> map_key_combo {
      { KeyButtonWhich::KEY_J, KeyButtonState::DOWN },
      { KeyButtonWhich::KEY_A, KeyButtonState::DOWN }      
  };
  if( !manager_window->SetKeyComboCallback( map_key_combo, key_callback_0 ) ){
      cout << "Error setting key combo callback" << endl;
      glfwTerminate();
      exit( EXIT_FAILURE );
  }
  map<KeyButtonWhich::Enum, KeyButtonState::Enum> map_key_combo_exit {
      { KeyButtonWhich::KEY_L, KeyButtonState::DOWN }
  };
  if( !manager_window->SetKeyComboCallback( map_key_combo_exit, key_callback_1 ) ){
      cout << "Error setting key combo callback" << endl;
      glfwTerminate();
      exit( EXIT_FAILURE );
  }
  //register callback
  if( !manager_window->SetDefaultCb() ){
      cout << "Error registering key combo callback" << endl;
      exit( EXIT_FAILURE );
  }
  
  enTPCommon tp;
  tp.SetNumThreads(4);
  enTPCommon * ptp = &tp;

  int count = 0;

  std::future<void> ret1 = tp.AddTask(RenderTask, argc, argv );
  std::future<void> ret2 = tp.AddTask( Idle, count, ptp );

  tp.RunThreads();

  while (!glfwWindowShouldClose(window))
  {
      if( bSignalExit ){
	  break;
      }
      glfwPollEvents();
      manager_window->ProcessKeyButtonCombo();
  }
  
  ret1.get();
  ret2.get();

  tp.EndAllThreads();

  glfwDestroyWindow(window);
  glfwTerminate();
  exit(EXIT_SUCCESS);
}
