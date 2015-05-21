#include "WindowManagerGlfw.h"
#include <GLFW/glfw3.H>

#include <map>

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
bool WindowManager::SetSize( int iId, unsigned int ui_width, unsigned int ui_height ){
    auto it = _MapIdWindow.find( iId );
    if( _MapIdWindow.end() == it ){
	return false;
    }
    GLFWwindow * window = it->second;
    glfwSetWindowSize( window, 640, 480 );
    
    return true;
}
bool WindowManager::GetSize( int iId, unsigned int & ui_width, unsigned int & ui_height ){
    auto it = _MapIdWindow.find( iId );
    if( _MapIdWindow.end() == it ){
	return false;
    }
    GLFWwindow * window = it->second;
    int width;
    int height;
    glfwGetWindowSize( window, &width, &height );

    ui_width = (unsigned int )width;
    ui_height = (unsigned int )height;

    return true;
}
bool WindowManager::SetFullScreen( int iId, bool WindowManager::bFullScreen ){
    //TODO
    return false;
}
bool WindowManager::CloseWindow( int iId ){
    auto it = _MapIdWindow.find( iId );
    if( _MapIdWindow.end() == it ){
	return false;
    }
    GLFWwindow * window = it->second;
    glfwDestroyWindow( window );
    return true;
}
