//application specific
#include "enTPCommon.h"
#include "GLTexture.h"
#include "GLSLProgram.h"
#include "GLAttribData.h"
#include "GLRender.h"
#include "textfile.h"
//opengl includes
#include <GL/glew.h>
#ifdef MAC_OS
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

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

void RenderTask(int argc, char **argv) {

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_3_2_CORE_PROFILE | GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowPosition(100,100);
    glutInitWindowSize(500,500);
    glutCreateWindow("Render Thread Task");

    glutDisplayFunc(GLRender::RenderScene);
    glutIdleFunc(GLRender::RenderScene);
    glutReshapeFunc(GLRender::ChangeSize);
    glutKeyboardFunc(GLRender::ProcessNormalKeys);

    glEnable(GL_DEPTH_TEST);
    glClearColor(0, 0, 0, 1.0);

    GLenum err = glewInit();
    if (GLEW_OK != err)
    {
        /* Problem: glewInit failed, something is seriously wrong. */
        cerr << "Error: " << glewGetErrorString(err) << endl;
    }
    cout << "Status: Using GLEW: " << glewGetString(GLEW_VERSION) << endl;

    if(glewIsSupported("GL_VERSION_3_2"))
        printf("Ready for OpenGL 3.0\n");
    else {
        printf("OpenGL 3.0 not supported\n");
        exit(1);
    }

    GLPrintInfo();

    GLRender::SetShaders();

    glutMainLoop();
}

void Idle( int count ){
    while(1) {
        cout << count << endl;
        ++count;
    }
}

int main(int argc, char **argv) {

  enTPCommon tp;
  tp.SetNumThreads(4);
  enTPCommon * ptp = &tp;

  int count = 0;

  std::future<void> ret1 = tp.AddTask(RenderTask, argc, argv );
  std::future<void> ret2 = tp.AddTask( Idle, count );

  tp.RunThreads();

  ret1.get();
  ret2.get();

  tp.EndAllThreads();

}
