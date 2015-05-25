#include "WindowManagerGlfw.h"
#include "WindowManager.h"
#include <GLFW/glfw3.h>

#include <map>
#include <bitset>
#include <mutex>
#include <string>

using namespace std;

map< GLFWwindow *, WindowManagerGlfw * > WindowManagerGlfw::_MapInstance;

WindowManagerGlfw::WindowManagerGlfw() : WindowManager() {
    _Window = nullptr;    
}
bool WindowManagerGlfw::CreateWindow( int & iId, unsigned int ui_width, unsigned int ui_height, string const strTitle ){
    if( _iId == iId || _iId >= 0){
	return false;
    }
    GLFWwindow * window = glfwCreateWindow( ui_width, ui_height, strTitle.c_str(), NULL, NULL );
    if( !window ){
	return false;
    }
    _iId = iId;
    _MapInstance[ window ] = this;
    _Window = window;
    return true;
}
bool WindowManagerGlfw::SetSize( unsigned int ui_width, unsigned int ui_height ){
    GLFWwindow * window;
    if( !GetWindow( window ) )
    {
	return false;
    }
    glfwSetWindowSize( window, 640, 480 );
    
    return true;
}
bool WindowManagerGlfw::GetSize( unsigned int & ui_width, unsigned int & ui_height ){
    GLFWwindow * window;
    if( !GetWindow( window ) )
    {
	return false;
    }
    int width;
    int height;
    glfwGetWindowSize( window, &width, &height );

    ui_width = (unsigned int )width;
    ui_height = (unsigned int )height;

    return true;
}
bool WindowManagerGlfw::SetFullScreen( bool bFullScreen ){
    //TODO
    return false;
}
bool WindowManagerGlfw::CloseWindow(){
    //TODO
    GLFWwindow * window;
    if( !GetWindow( window ) )
    {	
	return false;
    }
    glfwDestroyWindow( window );
    _Window = nullptr;
    _iId = -1;
    _MapInstance.erase( window );
    return true;
}
bool WindowManagerGlfw::GetCursorPos( double & xpos, double & ypos ){
    //TODO
    GLFWwindow * window;
    if( !GetWindow( window ) )
    {
	return false;
    }
    glfwGetCursorPos( window, &xpos, &ypos );
    return true;
}
bool WindowManagerGlfw::GetCursorState( KeyButtonWhich which, KeyButtonState & state ){
    GLFWwindow * window;
    if( !GetWindow( window ) )
    {
	return false;
    }
    return true;
}
bool WindowManagerGlfw::SetKeyComboCallback( std::map<KeyButtonWhich, KeyButtonState> combo, std::function<bool(void)> cb ){
    //TODO: save callback into a trie data structure
    for( auto i : combo ){
	unsigned int button_type = i.first;
	unsigned char val = 1 << i.second;	
    }
    return true;
}
bool WindowManagerGlfw::GetWindow( GLFWwindow * & window ){
    if( !_Window ){
        return false;
    }
    window = _Window;
    return true;
}
bool WindowManagerGlfw::GetId( int & iId ){
    iId = _iId;
    return true;
}
bool WindowManagerGlfw::SetCallbackKeyboard( void(*cb)( GLFWwindow*, int, int, int, int) ){
    GLFWwindow * window;
    if( !GetWindow( window ) )
    {
	return false;
    }
    glfwSetKeyCallback( window, cb );
    return true;
}
bool WindowManagerGlfw::SetCallbackMouseMove( void(*cb)( GLFWwindow*, double, double ) ){
    GLFWwindow * window;
    if( !GetWindow( window ) )
    {
	return false;
    }
    glfwSetCursorPosCallback( window, cb );
    return true;
}

bool WindowManagerGlfw::SetCallbackMouseButton( void(*cb)( GLFWwindow*, int button, int action, int mode ) ){
    GLFWwindow * window;
    if( !GetWindow( window ) )
    {
	return false;
    }
    glfwSetMouseButtonCallback( window, cb );
    return true;
}
bool WindowManagerGlfw::SetCallbackScroll( void(*cb)( GLFWwindow*, double xoffset, double yoffset ) ){
    GLFWwindow * window;
    if( !GetWindow( window ) )
    {
	return false;
    }
    glfwSetScrollCallback( window, cb );
    return true;
}
bool WindowManagerGlfw::ProcessKeyButtonCombo(){
    //copy current state of keys and buttons
    
    //clear current state of keys and buttons
    _KeyButtonDataCurrent.Clear();
    //TODO: find key combinations and get callback
    //call callback
    //std::function<bool(void)> cb;
    //cb();
    return true;
}
bool WindowManagerGlfw::SetDefaultCb(){
    if( !SetCallbackKeyboard( &ProcessKeyboardCb ) ){
	return false;
    }
//     if( !SetCallbackMouseMove( &ProcessMouseMoveCb ) ){
// 	return false;
//     }
//     if( !SetCallbackMouseButton( &ProcessMouseButtonCb ) ){
// 	return false;
//     }
//     if( !SetCallbackScroll( &ProcessScrollCb ) ){
// 	return false;
//     }
    return true;
}
void WindowManagerGlfw::ProcessMouseMoveCb( GLFWwindow * window, double xpos, double ypos ){
    auto it = _MapInstance.find( window );
    if( it == _MapInstance.end() ){
        return;
    }
    WindowManagerGlfw * instance = it->second;
    instance->_Mousex = xpos;
    instance->_Mousey = ypos;
}
void WindowManagerGlfw::ProcessKeyboardCb( GLFWwindow * window, int key, int scancode, int action, int mode ){
    auto it = _MapInstance.find( window );
    if( it == _MapInstance.end() ){
        return;
    }
    WindowManagerGlfw * instance = it->second;
    //get key
    KeyButtonWhich which;
    switch( key ){
    case GLFW_KEY_W:
	which = KeyButtonWhich::KEY_W;
	break;
    case GLFW_KEY_A:
	which = KeyButtonWhich::KEY_A;
	break;
    case GLFW_KEY_S:
	which = KeyButtonWhich::KEY_S;
	break;
    case GLFW_KEY_D:
	which = KeyButtonWhich::KEY_D;
	break;
    case GLFW_KEY_SPACE:
	which = KeyButtonWhich::KEY_SPC;
	break;
    case GLFW_KEY_J:
	which = KeyButtonWhich::KEY_J;
	break;
    case GLFW_KEY_K:
	which = KeyButtonWhich::KEY_K;
	break;
    case GLFW_KEY_L:
	which = KeyButtonWhich::KEY_L;
	break;
    default:
        return;
    }
    KeyButtonState state;
    bool bRepeat = false;
    switch( action ){
    case GLFW_PRESS:
	state = KeyButtonState::DOWN;
	break;
    case GLFW_RELEASE:
	state = KeyButtonState::UP;
	break;	
    default:      
      //GLFW_REPEAT case
      state = KeyButtonState::REPEAT;
    }
    //save state
    unsigned char val = 1 << state;
    instance->_KeyButtonDataCurrent.Array[ which ] |= val;    
}
void WindowManagerGlfw::ProcessMouseButtonCb( GLFWwindow * window, int button, int action, int mode ){
    auto it = _MapInstance.find( window );
    if( it == _MapInstance.end() ){
        return;
    }
    WindowManagerGlfw * instance = it->second;
    //get key
    KeyButtonWhich which;
    switch( button ){
    case GLFW_MOUSE_BUTTON_LEFT:
	which = KeyButtonWhich::MOUSE_L;
	break;
    case GLFW_MOUSE_BUTTON_RIGHT:
	which = KeyButtonWhich::MOUSE_R;
	break;
    case GLFW_MOUSE_BUTTON_MIDDLE:
	which = KeyButtonWhich::MOUSE_M;
	break;
    default:
        return;
    }
    KeyButtonState state;
    switch( action ){
    case GLFW_PRESS:
	state = KeyButtonState::DOWN;	
	break;
    case GLFW_RELEASE:
	state = KeyButtonState::UP;
	break;
    default:
	//repeat case
	state = KeyButtonState::REPEAT;
    }
    //save state
    unsigned char val = 1 << state;
    instance->_KeyButtonDataCurrent.Array[ which ] |= val;
}
void WindowManagerGlfw::ProcessScrollCb( GLFWwindow*, double xoffset, double yoffset ){
    
}
