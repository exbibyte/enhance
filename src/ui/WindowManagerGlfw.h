#ifndef WINDOW_MANAGER_GLFW_H
#define WINDOW_MANAGER_GLFW_H

#include <vector>
#include <WindowManager.h>
#include <GLFW/glfw3.h>

class WindowManagerGlfw : public WindowManager{
public:
    WindowManagerGlfw();
    //base implementation
    bool CreateWindow( int & iId, unsigned int ui_width, unsigned int ui_height, string const strTitle );
    bool SetSize( unsigned int ui_width, unsigned int ui_height );
    bool GetSize( unsigned int & ui_width, unsigned int & ui_height );
    bool SetFullScreen( bool bFullScreen );
    bool CloseWindow();
    bool GetCursorPos( double & xpos, double & ypos );    
    bool GetCursorState( KeyButtonWhich which, KeyButtonState & state );
    bool SetKeyComboCallback( std::map<KeyButtonWhich, KeyButtonState> combo, std::function<bool(void)> cb );
    bool ProcessKeyButtonCombo();
    
    //helpers
    bool GetWindow( GLFWwindow * & win );
    bool GetId( int & iId );
    bool SetCallbackKeyboard( void(*cb)( GLFWwindow*, int key, int scancode, int action, int mode ) );
    bool SetCallbackMouseMove( void(*cb)( GLFWwindow*, double xpos, double ypos ) );
    bool SetCallbackMouseButton( void(*cb)( GLFWwindow*, int button, int action, int mode ) );
    bool SetCallbackScroll( void(*cb)( GLFWwindow*, double xoffset, double yoffset ) );
    bool SetDefaultCb();
    
    //default key and button callbacks
    static void ProcessMouseMoveCb( GLFWwindow * window, double xpos, double ypos );
    static void ProcessKeyboardCb( GLFWwindow * window, int key, int scancode, int action, int mode );
    static void ProcessMouseButtonCb( GLFWwindow * window, int button, int action, int mode );
    static void ProcessScrollCb( GLFWwindow*, double xoffset, double yoffset );
private:
    GLFWwindow * _Window;
    double _Mousex;
    double _Mousey;
};

#endif
