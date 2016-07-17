#ifndef WINDOW_INFO_H
#define WINDOW_INFO_H

enum class WindowType {
    GLFW,
    OTHER,
};

class GLFWwindow;

class WindowInfo {
public:
    union {
	GLFWwindow * _window;
    };
    WindowType _type;
    unsigned int _width;
    unsigned int _height;
};

#endif
