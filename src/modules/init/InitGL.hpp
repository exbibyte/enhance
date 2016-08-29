#ifndef INITGL_H
#define INITGL_H

#include "IInit.hpp"

#include "WindowInfo.hpp"

#include <initializer_list>

class GLFWwindow;

class InitGL : public IInit {
public:
    InitGL() : _window(nullptr) {}
    ~InitGL();
    char const * get_id(){ return "initGL"; }
    bool init( std::initializer_list<unsigned> const & window_args );
    WindowInfo GetWindowResource();
private:
    void print_info_opengl();
    GLFWwindow * _window;
    unsigned int _width;
    unsigned int _height;
};

#endif
