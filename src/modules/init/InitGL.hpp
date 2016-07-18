#ifndef INITGL_H
#define INITGL_H

#include "IInit.hpp"

#include "WindowInfo.hpp"

class GLFWwindow;

class InitGL : public IInit {
public:
    InitGL() : _window(nullptr) {}
    ~InitGL();
    char const * get_id(){ return "initGL"; }
    bool init();
    WindowInfo GetWindowResource();
private:
    GLFWwindow * _window;
    unsigned int _width;
    unsigned int _height;
};

#endif
