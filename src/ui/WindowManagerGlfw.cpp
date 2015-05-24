#include "WindowManagerGlfw.h"
#include <GLFW/glfw3.H>

#include <map>
#include <bitset>
#include <mutex>

using namespace std;

WindowManagerGlfw::WindowManagerGlfw(){
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
bool WindowManagerGlfw::SetFullScreen( bool WindowManagerGlfw::bFullScreen ){
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
    _window = nullptr;
    _iId = -1;
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
bool WindowManagerGlfw::GetCursorState( KeyButtonWhich which, KeyButtonState state ){
    GLFWwindow * window;
    if( !GetWindow( window ) )
    {
	return false;
    }
    return true;
}
bool WindowManagerGlfw::SetKeyComboCallback( std::map<KeyButtonWhich, KeyButtonState> combo, std::function<bool(void)> cb ){
    bitset<32> keycombo;
    keycombo.set();
    for( auto i : combo ){
	unsigned int offset = static_cast<unsigned int>( i->first );
	bool val = (bool) static_cast<unsigned int>( i->second );
	keycombo.set( offset, val );	
    }
    //convert pattern to string and add callback for the pattern
    string strPattern = keycombo.to_string();
    _MapKeyButtonComboCb[ strPattern ] = cb;
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
    bitset<32> current_pattern = _KeyButtonComboCurrent;
    //reset current state
    _KeyButtonComboCurrent.reset();
    //find and call key combination callback if exists
    string str_current_pattern = current_pattern.to_string();
    auto it = _MapKeyButtonComboCb.find( str_current_pattern );
    if( it != it2->second.end() ){
	return false;
    }    	
    std::function<bool(void)> cb = it->second;
    cb();
    return true;
}
bool WindowManagerGlfw::SetDefaultCb(){
    if( SetCallbackKeyboard( &ProcessKeyboardCb ) ){
	return false;
    }
    if( SetCallbackMouseMove( &ProcessMouseMoveCb ) ){
	return false;
    }
    if( SetCallbackMouseButton( &ProcessMouseButtonCb ) ){
	return false;
    }
    if( SetCallbackScroll( &ProcessScrollCb ) ){
	return false;
    }
    return true;
}
void WindowManagerGlfw::ProcessMouseMoveCb( GLFWwindow * window, double xpos, double ypos ){
    if( _Window != window ){
        return;
    }
    _Mousex = xpos;
    _Mousey = ypos;
}
void WindowManagerGlfw::ProcessKeyboardCb( GLFWwindow * window, int key, int scancode, int action, int mode ){
    if( _Window != window ){
        return;
    }
    //get key
    KeyButtonWhich which;
    switch( key ){
    case GLFW_KEY_W:
	which = KEY_W;
	break;
    case GLFW_KEY_A:
	which = KEY_A;
	break;
    case GLFW_KEY_S:
	which = KEY_S;
	break;
    case GLFW_KEY_D:
	which = KEY_D;
	break;
    case GLFW_KEY_SPACE:
	which = KEY_SPC;
	break;
    case GLFW_KEY_J:
	which = KEY_J;
	break;
    case GLFW_KEY_K:
	which = KEY_K;
	break;
    case GLFW_KEY_L:
	which = KEY_L;
	break;
    default:
        return;
    }
    unsigned int offset = static_cast<unsigned int>( which );
    KeyButtonState state;
    switch( action ){
    case GLFW_PRESS:
	state = DOWN;
	break;
    case GLFW_RELEASE:
	state = UP;
	break;
    default:
        return;
    }
    bool val = (bool)static_cast<unsigned int>( state );
    //set current state
    _KeyButtonComboCurrent.set( offset, val );
}
void WindowManagerGlfw::ProcessMouseButtonCb( GLFWwindow * window, int button, int action, int mode ){
    if( _Window != window ){
        return;
    }
    //get key
    KeyButtonWhich which;
    switch( button ){
    case GLFW_MOUSE_BUTTON_LEFT:
	which = MOUSE_L;
	break;
    case GLFW_MOUSE_BUTTON_RIGHT:
	which = MOUSE_R;
	break;
    case GLFW_MOUSE_BUTTON_MIDDLE:
	which = MOUSE_M;
	break;
    default:
        return;
    }
    unsigned int offset = static_cast<unsigned int>( which );
    KeyButtonState state;
    switch( action ){
    case GLFW_PRESS:
	state = DOWN;
	break;
    case GLFW_RELEASE:
	state = UP;
	break;
    default:
        return;
    }
    bool val = (bool)static_cast<unsigned int>( state );
    //set current state
    _KeyButtonComboCurrent.set( offset, val );
}
void WindowManagerGlfw::ProcessScrollCb( GLFWwindow*, double xoffset, double yoffset ){
    
}
