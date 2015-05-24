#ifndef WINDOW_MANAGER_GLFW_H
#define WINDOW_MANAGER_GLFW_H

#include <vector>
#include <WindowManager.h>
#include <GLFW/glfw3.h>

class WindowManagerGlfw : public WindowManager{
public:
    //base implementation
    bool CreateWindow( int & iId, unsigned int ui_width, unsigned int ui_height, string const strTitle );
    bool SetSize( int iId, unsigned int ui_width, unsigned int ui_height );
    bool GetSize( int iId, unsigned int & ui_width, unsigned int & ui_height );
    bool SetFullScreen( int iId, bool bFullScreen );
    bool CloseWindow( int iId );
    bool GetCursorPos( int iId, double & xpos, double & ypos );    
    bool GetCursorState( int iId, KeyButtonWhich which, KeyButtonState & state );
    bool SetKeyComboCallback( int iId, std::map<KeyButtonWhich, KeyButtonState> combo, std::function<bool(void)> cb );
    bool ProcessKeyButtonCombo( int iId );
    
    //helpers
    bool GetWindow( int iId, GLFWwindow * win );
    bool SetCallbackKeyboard( int iId, void(*cb)( GLFWwindow*, int key, int scancode, int action, int mode ) );
    bool SetCallbackMouseMove( int iId, void(*cb)( GLFWwindow*, double xpos, double ypos ) );
    bool SetCallbackMouseButton( int iId, void(*cb)( GLFWwindow*, int button, int action, int mode ) );
    bool SetCallbackScroll( int iId, void(*cb)( GLFWwindow*, double xoffset, double yoffset ) );
    bool SetDefaultCb();
    
    //default key and button callbacks
    static void ProcessMouseMoveCb( GLFWwindow * window, double xpos, double ypos );
    static void ProcessKeyboardCb( GLFWwindow * window, int key, int scancode, int action, int mode );
    static void ProcessMouseButtonCb( GLFWwindow * window, int button, int action, int mode );
    static void ProcessScrollCb( GLFWwindow*, double xoffset, double yoffset );
private:
    std::map< int, GLFWwindow * > _MapIdWindow;
    std::map< int, std::map< KeyButtonWhich, KeyButtonState > > _MapKeyButtonState;
};

#endif
