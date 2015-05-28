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

class test_callback_class {
public:
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
    bool test_mousebutton_callback0(){
	cout << "mouse button 0" << endl;
	return true;
    }
    bool test_mousebutton_callback1(){
	cout << "mouse button 1" << endl;
	return true;
    }
    bool test_mousemove_callback( int x, int y ){
	cout << "mouse move: " << x <<", " << y << endl;
	return true;
    }
    bool test_scroll_callback( int x, int y ){
	cout << "scroll: " << x <<", " << y << endl;
	return true;
    }
};

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
  KeyButtonWhich::Enum mouse_l = KeyButtonWhich::MOUSE_L;
  KeyButtonWhich::Enum mouse_r = KeyButtonWhich::MOUSE_R;
  
  KeyButtonState::Enum state_a = KeyButtonState::DOWN;
  KeyButtonState::Enum state_space = KeyButtonState::DOWN;
  KeyButtonState::Enum state_j = KeyButtonState::DOWN;
  KeyButtonState::Enum state_mouse_l = KeyButtonState::DOWN;
  KeyButtonState::Enum state_mouse_r = KeyButtonState::DOWN;
  
  test_callback_class _cbs;
  
  std::function<bool(void)> key_cb0 = bind( &test_callback_class::test_key_callback0, &_cbs );
  std::function<bool(void)> key_cb1 = bind( &test_callback_class::test_key_callback1, &_cbs );
  std::function<bool(void)> key_cb2 = bind( &test_callback_class::test_key_callback2, &_cbs );
  std::function<bool(void)> mousebutton_cb0 = bind( &test_callback_class::test_mousebutton_callback0, &_cbs );
  std::function<bool(void)> mousebutton_cb1 = bind( &test_callback_class::test_mousebutton_callback1, &_cbs );
    
  map< KeyButtonWhich::Enum, KeyButtonState::Enum > map_key_combo0;
  map_key_combo0[ key_a ] = state_a;
  map_key_combo0[ key_space ] = state_space;
  bRet = win_manager.SetKeyComboCallback( map_key_combo0, key_cb0 );

  map< KeyButtonWhich::Enum, KeyButtonState::Enum > map_key_combo1;
  map_key_combo1[ key_space ] = state_space;
  bRet = win_manager.SetKeyComboCallback( map_key_combo1, key_cb1 );

  map< KeyButtonWhich::Enum, KeyButtonState::Enum > map_key_combo2;
  map_key_combo2[ key_j ] = state_j;
  bRet = win_manager.SetKeyComboCallback( map_key_combo2, key_cb2 );

  map< KeyButtonWhich::Enum, KeyButtonState::Enum > map_mousebutton_combo0;
  map_mousebutton_combo0[ mouse_l ] = state_mouse_l;
  bRet = win_manager.SetKeyComboCallback( map_mousebutton_combo0, mousebutton_cb0 );

  map< KeyButtonWhich::Enum, KeyButtonState::Enum > map_mousebutton_combo1;
  map_mousebutton_combo1[ mouse_r ] = state_mouse_r;
  bRet = win_manager.SetKeyComboCallback( map_mousebutton_combo1, mousebutton_cb1 );

  std::function<bool(int,int)> mouse_move_cb = bind( &test_callback_class::test_mousemove_callback, &_cbs, std::placeholders::_1, std::placeholders::_2 );
  bRet = win_manager.SetMouseMoveCallback( mouse_move_cb );

  std::function<bool(int,int)> scroll_cb = bind( &test_callback_class::test_scroll_callback, &_cbs, std::placeholders::_1, std::placeholders::_2 );
  bRet = win_manager.SetScrollCallback( scroll_cb );
  
  bRet = win_manager.SetDefaultCb();
  
  glfwMakeContextCurrent(window);

//  GLPrintInfo();
  
//  glEnable(GL_DEPTH_TEST);
//  glClearColor(0, 0, 0, 1.0);
     
  while (!glfwWindowShouldClose(window))
  {
      glfwPollEvents();
      bRet = win_manager.ProcessKeyButtonCombo();
      bRet = win_manager.ProcessMouseMoveCombo();
      bRet = win_manager.ProcessScrollCombo();
      //usleep(10000);
      //      glfwSwapBuffers(window);
  }

  glfwDestroyWindow(window);
  glfwTerminate();
  exit(EXIT_SUCCESS);
}
