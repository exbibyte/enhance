#ifndef WINDOW_MANAGER_GLFW_H
#define WINDOW_MANAGER_GLFW_H

#include <vector>
#include <GLFW/glfw3.h>
#include <string>
#include <functional>

#include "WindowManager.h"

class WindowManagerGlfw : public WindowManager< std::function<bool(void)> > {
public:
    WindowManagerGlfw();
    //base implementation
    virtual bool CreateWindow( int iId, unsigned int ui_width, unsigned int ui_height, std::string const strTitle );
    virtual bool SetSize( unsigned int ui_width, unsigned int ui_height );
    virtual bool GetSize( unsigned int & ui_width, unsigned int & ui_height );
    virtual bool SetFullScreen( bool bFullScreen );
    virtual bool CloseWindow();
    virtual bool GetCursorPos( double & xpos, double & ypos );    
    virtual bool GetCursorState( KeyButtonWhich::Enum which, KeyButtonState::Enum & state );
    virtual bool SetKeyComboCallback( std::map<KeyButtonWhich::Enum, KeyButtonState::Enum> combo, std::function<bool(void)> cb );
    virtual bool SetMouseMoveCallback( std::function<bool(int,int)> cb );
    virtual bool SetScrollCallback( std::function<bool(int,int)> cb );
    virtual bool ProcessKeyButtonCombo();
    virtual bool ProcessMouseMoveCombo();
    virtual bool ProcessScrollCombo();
    
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
    bool _bMouseMoved;
    double _Scrollx;
    double _Scrolly;
    bool _bScrolled;
    static std::map< GLFWwindow *, WindowManagerGlfw * > _MapInstance;
};

#endif
