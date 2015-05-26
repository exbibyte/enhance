//opengl includes
#include <GLFW/glfw3.h>

//math library
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/transform2.hpp>
using glm::mat4;
using glm::vec3;

//user specific headers
#include "WindowManagerGlfw.h"
#include "WindowManager.h"

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

#include <unistd.h>

using namespace std;

GLFWwindow * window;

static void error_callback(int error, const char* description)
{
    fputs(description, stderr);
}

bool test_key_callback0(){
    cout << "callback 0" << endl;
    return true;
}
bool test_key_callback1(){
    cout << "callback 1" << endl;
    return true;
}
bool test_key_callback2(){
    cout << "callback 2" << endl;
    return true;
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
  
  WindowManagerGlfw win_manager;
  int iId = 0;
  bool bRet = win_manager.CreateWindow( iId, 500, 500, "Window Manager Test" );
  
  if( !bRet ){
      glfwTerminate();
      exit(EXIT_FAILURE);
  }

  bRet = win_manager.GetWindow( window );

  KeyButtonWhich::Enum key_a = KeyButtonWhich::KEY_A;
  KeyButtonWhich::Enum key_space = KeyButtonWhich::KEY_SPC;
  KeyButtonWhich::Enum key_j = KeyButtonWhich::KEY_J;
  KeyButtonState::Enum state_a = KeyButtonState::DOWN;
  KeyButtonState::Enum state_space = KeyButtonState::DOWN;
  KeyButtonState::Enum state_j = KeyButtonState::DOWN;

  std::function<bool(void)> key_cb0 = test_key_callback0;
  std::function<bool(void)> key_cb1 = test_key_callback1;
  std::function<bool(void)> key_cb2 = test_key_callback2;
    
  map< KeyButtonWhich::Enum, KeyButtonState::Enum > map_key_combo0;
  map_key_combo0[ key_a ] = state_a;
  map_key_combo0[ key_space ] = state_space;
  bRet = win_manager.SetKeyComboCallback( map_key_combo0, key_cb0 );

  map< KeyButtonWhich::Enum, KeyButtonState::Enum > map_key_combo1;
  map_key_combo1[ key_space ] = state_space;
//  bRet = win_manager.SetKeyComboCallback( map_key_combo1, key_cb1 );

  map< KeyButtonWhich::Enum, KeyButtonState::Enum > map_key_combo2;
  map_key_combo2[ key_j ] = state_j;
  bRet = win_manager.SetKeyComboCallback( map_key_combo2, key_cb2 );

  bRet = win_manager.SetDefaultCb();
  
  glfwMakeContextCurrent(window);

//  GLPrintInfo();
  
//  glEnable(GL_DEPTH_TEST);
//  glClearColor(0, 0, 0, 1.0);
     
  while (!glfwWindowShouldClose(window))
  {
      glfwPollEvents();
      bRet = win_manager.ProcessKeyButtonCombo();
      usleep(10000);
      //      glfwSwapBuffers(window);
  }

  glfwDestroyWindow(window);
  glfwTerminate();
  exit(EXIT_SUCCESS);
}
