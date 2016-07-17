#ifndef GLINCLUDES_H
#define GLINCLUDES_H

#ifdef __APPLE__

#include <OpenGL/gl3.h>

#elif __linux__

#include <GL/gl3w.h>
#include <GLFW/glfw3.h>

#endif

#endif
