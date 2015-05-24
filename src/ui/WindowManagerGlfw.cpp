#include "WindowManagerGlfw.h"
#include <GLFW/glfw3.H>

#include <map>
#include <bitset>
#include <mutex>

using namespace std;

WindowManagerGlfw::WindowManagerGlfw(){

}
bool WindowManagerGlfw::CreateWindow( int & iId, unsigned int ui_width, unsigned int ui_height, string const strTitle ){
    if( _MapIdWindow.end() != _MapIdWindow.find( iId ) ){
	return false;
    }
    _MapIdWindow[ iId ] = glfwCreateWindow( ui_width, ui_height, strTitle.c_str(), NULL, NULL );

    if( !_MapIdWindow[ iId ] ){
	return false;
    }

    return true;
}
bool WindowManagerGlfw::SetSize( int iId, unsigned int ui_width, unsigned int ui_height ){
    GLFWwindow * window;
    if( !GetWindow( iId, window ) )
    {
	return false;
    }
    glfwSetWindowSize( window, 640, 480 );
    
    return true;
}
bool WindowManagerGlfw::GetSize( int iId, unsigned int & ui_width, unsigned int & ui_height ){
    GLFWwindow * window;
    if( !GetWindow( iId, window ) )
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
bool WindowManagerGlfw::SetFullScreen( int iId, bool WindowManagerGlfw::bFullScreen ){
    //TODO
    return false;
}
bool WindowManagerGlfw::CloseWindow( int iId ){
    GLFWwindow * win;
    if( !GetWindow( iId, win ) )
    {	
	return false;
    }
    glfwDestroyWindow( win );
    _MapIdWindow.erase( iId );
    return true;
}
bool WindowManagerGlfw::GetCursorPos( int iId, double & xpos, double & ypos ){
    GLFWwindow * window;
    if( !GetWindow( iId, window ) )
    {
	return false;
    }
    glfwGetCursorPos( window, &xpos, &ypos );
    return true;
}
bool WindowManagerGlfw::GetCursorState( int iId, KeyButtonWhich which, KeyButtonState state ){
    GLFWwindow * window;
    if( !GetWindow( iId, window ) )
    {
	return false;
    }
    return true;
}
bool WindowManagerGlfw::SetKeyComboCallback( int iId, std::map<KeyButtonWhich, KeyButtonState> combo, std::function<bool(void)> cb ){
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
bool WindowManagerGlfw::GetWindow( int iId, GLFWwindow * win ){
    auto it = _MapIdWindow.find( iId );
    if( _MapIdWindow.end() == it ){
	return false;
    }
    win = it->second;
    return true;
}

bool WindowManagerGlfw::SetCallbackKeyboard( int iId, void(*cb)( GLFWwindow*, int, int, int, int) ){
    GLFWwindow * window;
    if( !GetWindow( iId, window ) )
    {
	return false;
    }
    glfwSetKeyCallback( window, cb );
    return true;
}
bool WindowManagerGlfw::SetCallbackMouseMove( int iId, void(*cb)( GLFWwindow*, double, double ) ){
    GLFWwindow * window;
    if( !GetWindow( iId, window ) )
    {
	return false;
    }
    glfwSetCursorPosCallback( window, cb );
    return true;
}

bool WindowManagerGlfw::SetCallbackMouseButton( int iId, void(*cb)( GLFWwindow*, int button, int action, int mode ) ){
    GLFWwindow * window;
    if( !GetWindow( iId, window ) )
    {
	return false;
    }
    glfwSetMouseButtonCallback( window, cb );
    return true;
}
bool WindowManagerGlfw::SetCallbackScroll( int iId, void(*cb)( GLFWwindow*, double xoffset, double yoffset ) ){
    GLFWwindow * window;
    if( !GetWindow( iId, window ) )
    {
	return false;
    }
    glfwSetScrollCallback( window, cb );
    return true;
}
bool WindowManagerGlfw::ProcessKeyButtonCombo( int iId ){
    //copy current state of keys and buttons
    bitset<32> current_pattern = _KeyButtonComboCurrent;
    //reset current state
    _KeyButtonComboCurrent.reset();
    //find and call key combination callback if exists
    string str_current_pattern = current_pattern.to_string();
    auto it = _MapKeyButtonComboCb.find( str_current_pattern );
    if( it != _MapKeyButtonComboCb.end() ){
	std::function<bool(void)> cb = it->second;
	cb();
	return true;
    }    	
    return false;
}
bool WindowManagerGlfw::SetDefaultCb( int iId ){
    if( SetCallbackKeyboard( iId, &ProcessKeyboardCb ) ){
	return false;
    }
    if( SetCallbackMouseMove( iId, &ProcessMouseMoveCb ) ){
	return false;
    }
    if( SetCallbackMouseButton( iId, &ProcessMouseButtonCb ) ){
	return false;
    }
    if( SetCallbackScroll( iId, &ProcessScrollCb ) ){
	return false;
    }
    return true;
}
void WindowManagerGlfw::ProcessMouseMoveCb( GLFWwindow * window, double xpos, double ypos ){
}
void WindowManagerGlfw::ProcessKeyboardCb( GLFWwindow * window, int key, int scancode, int action, int mode ){
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
    }
    bool val = (bool)static_cast<unsigned int>( state );
    //set current state
    _KeyButtonComboCurrent.set( offset, val );
}
void WindowManagerGlfw::ProcessMouseButtonCb( GLFWwindow * window, int button, int action, int mode ){

}
void WindowManagerGlfw::ProcessScrollCb( GLFWwindow*, double xoffset, double yoffset ){

}
