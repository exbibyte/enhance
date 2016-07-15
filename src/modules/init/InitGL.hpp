#ifndef INITGL_H
#define INITGL_H

#include "IInit.hpp"

class GLFWwindow;

class InitGL : public IInit {
public:
    InitGL() : _window(nullptr) {}
    ~InitGL();
    char const * get_id(){ return "initGL"; }
    bool init();
private:
    GLFWwindow * _window;
};

#endif
