#ifndef WINDOW_MANAGER_GLFW_H
#define WINDOW_MANAGER_GLFW_H

#include <vector>
#include <GLFW/glfw3.h>

class WindowManagerGlfw : public WindowManager{
private:
    std::map< int, GLFWwindow * > _MapIdWindow;
};

#endif
