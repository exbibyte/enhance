//opengl includes
#include <GLFW/glfw3.h>

//application specific
#include "enTPCommon.h"
#include "GLTexture.h"
#include "GLSLProgram.h"
#include "GLAttribData.h"
#include "GLRender.h"
#include "textfile.h"

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
using namespace std;


GLFWwindow * window;

static void error_callback(int error, const char* description)
{
    fputs(description, stderr);
}
static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);
}

void RenderTask(int argc, char **argv) {

    glfwMakeContextCurrent( window );
    // if (!glfwInit())
    //     exit(EXIT_FAILURE);
    
    // glfwSetErrorCallback(error_callback);
    
    // glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);  // yes, 3 and 2!!!
    // glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    // glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    // glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    // glfwWindowHint(GLFW_DEPTH_BITS,24);

    // //get version
    // int major, minor, rev;
    // glfwGetVersion( &major, &minor, &rev );

    // cout<< "major: "<<major<<", minor: "<<minor<<", rev: "<<rev<<endl;
    
    // window = glfwCreateWindow(500, 500, "Shadow Test", NULL, NULL);
    // if (!window)
    // {
    //     glfwTerminate();
    //     exit(EXIT_FAILURE);
    // }
    // glfwMakeContextCurrent(window);

    // glfwSetKeyCallback(window, key_callback);

    GLPrintInfo();

    glEnable(GL_DEPTH_TEST);
    glClearColor(0, 0, 0, 1.0);

    GLRender::SetShaders();
     
    while (!glfwWindowShouldClose(window))
    {
        GLRender::RenderScene();
        glfwSwapBuffers(window);
//        glfwPollEvents();
    }
    //glfwDestroyWindow(window);
    //glfwTerminate();
    //exit(EXIT_SUCCESS);
}

void Idle( int count, enTPCommon * Pool ){
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
    
  window = glfwCreateWindow(500, 500, "Shadow Test", NULL, NULL);
  if (!window)
  {
      glfwTerminate();
      exit(EXIT_FAILURE);
  }

  glfwMakeContextCurrent(window);

  glfwSetKeyCallback(window, key_callback);

  enTPCommon tp;
  tp.SetNumThreads(4);
  enTPCommon * ptp = &tp;

  int count = 0;

  std::future<void> ret1 = tp.AddTask(RenderTask, argc, argv );
  std::future<void> ret2 = tp.AddTask( Idle, count, ptp );

  tp.RunThreads();

  while (!glfwWindowShouldClose(window))
  {
      glfwPollEvents();
  }
  
  ret1.get();
  ret2.get();

  tp.EndAllThreads();

  glfwDestroyWindow(window);
  glfwTerminate();
  exit(EXIT_SUCCESS);
}
