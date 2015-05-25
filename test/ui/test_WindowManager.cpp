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
    cout<< "key press count: None" << endl;
    return true;
}
bool test_key_callback(){
    cout<< "key press count: Two" << endl;
    return true;
}
bool test_key_callback2(){
    cout<< "key press count: 1" << endl;
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

  KeyButtonWhich key_a = KeyButtonWhich::KEY_A;
  KeyButtonWhich key_space = KeyButtonWhich::KEY_SPC;
  KeyButtonState state_a = KeyButtonState::DOWN;
  KeyButtonState state_space = KeyButtonState::DOWN;

  map< KeyButtonWhich, KeyButtonState > map_key_combo;
  map_key_combo[ key_a ] = state_a;
  map_key_combo[ key_space ] = state_space;
  std::function<bool(void)> key_cb = test_key_callback;
  bRet = win_manager.SetKeyComboCallback( map_key_combo, key_cb );

  map< KeyButtonWhich, KeyButtonState > map_key_combo2;
  map_key_combo2[ key_a ] = state_a;
  std::function<bool(void)> key_cb2 = test_key_callback2;
  bRet = win_manager.SetKeyComboCallback( map_key_combo2, key_cb2 );

  map< KeyButtonWhich, KeyButtonState > map_key_combo0;
  std::function<bool(void)> key_cb0 = test_key_callback0;
  bRet = win_manager.SetKeyComboCallback( map_key_combo0, key_cb0 );

  bRet = win_manager.SetDefaultCb();
  
  glfwMakeContextCurrent(window);

//  GLPrintInfo();
  
//  glEnable(GL_DEPTH_TEST);
//  glClearColor(0, 0, 0, 1.0);
     
  while (!glfwWindowShouldClose(window))
  {
      glfwPollEvents();
      bRet = win_manager.ProcessKeyButtonCombo_Repeat();
      //usleep(1000);
      //      glfwSwapBuffers(window);
  }

  glfwDestroyWindow(window);
  glfwTerminate();
  exit(EXIT_SUCCESS);
}
