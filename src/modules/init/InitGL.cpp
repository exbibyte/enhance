#include <string>
#include <cstring>
#include <iostream>
#include <fstream>

#include "InitGL.hpp"
#include "IInit.hpp"
#include <GL/gl3w.h>
#include <GLFW/glfw3.h>

// #include "GLSceneManager.h"
// #include "GLSLProgram.h"
// #include "enGameData.h"
// #include <GLFW/glfw3.h>

// #include "GLHelper.h"

#include <iostream>
#include <cstdio>

bool InitGL::init(){

    if (!glfwInit()) {
	printf("failed to initialize GLFW.\n");
	return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow * window = glfwCreateWindow(1000, 600, "awesome", nullptr, nullptr);
    if (!window) {
	return -1;
    }

    glfwMakeContextCurrent(window);
    if (gl3wInit()) {
	printf("failed to initialize OpenGL\n");
	return -1;
    }

    printf("OpenGL %s, GLSL %s\n", glGetString(GL_VERSION), glGetString(GL_SHADING_LANGUAGE_VERSION));
    
    // GLFWwindow * window = glfwCreateWindow( ui_width, ui_height, strTitle.c_str(), NULL, NULL );
    
    // glfwMakeContextCurrent( game_data->_Window );

    // gl3wInit();

    // GLPrintInfo();

    // glEnable(GL_DEPTH_TEST);
    // glClearColor(0, 0, 0, 1.0);

    std::cout << "InitGL::init invoked." << std::endl;

    return true;
}
