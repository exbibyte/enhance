#include <string>
#include <cstring>
#include <iostream>
#include <fstream>

#include "UiMouse0.hpp"
#include "IUi.hpp"

#include <GLFW/glfw3.h>

#include <list>
#include <set>
#include <unordered_map>
#include <utility>

std::unordered_multimap<IUi::handle_resource, UiMouse0*> UiMouse0:: _map_resource_to_instance {};

bool UiMouse0::get_coordinates_3( std::list<coordinate> & coords ){
    coords.splice( coords.end(), _coords );
    return true;
}
bool UiMouse0::get_characters( std::list<character> & ){
    return false;
}
bool UiMouse0::init(){
    return true;
}
bool UiMouse0::deinit(){
    return true;
}
void UiMouse0::process_mouse_move( GLFWwindow * window, double xpos, double ypos ){
    //find the instance mapped from window resource
    auto it_range = _map_resource_to_instance.equal_range( window );
    auto it = it_range.first;
    while( it != it_range.second ){
	UiMouse0 * instance = it->second;
	instance->_coords.push_back( { window, xpos, ypos, 0 } );
	++it;
    }
}
bool UiMouse0::register_resource_to_monitor( handle_resource resource ){
    if( !resource )
	return false;
    //assume resource is a valid GLFWwindow *
    _resources_to_monitor.insert( resource );
    glfwSetCursorPosCallback( (GLFWwindow *)resource, process_mouse_move );
    _map_resource_to_instance.insert( std::pair<handle_resource,UiMouse0*>( resource, this ) );
    return true;
}
