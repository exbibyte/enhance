#include <string>
#include <cstring>
#include <iostream>
#include <fstream>

#include "Ui0.hpp"
#include "IUi.hpp"

#include <GLFW/glfw3.h>

#include <list>
#include <set>
#include <unordered_map>
#include <utility>

std::unordered_multimap<IUi::handle_resource, Ui0*> Ui0:: _map_resource_to_instance {};

bool Ui0::get_coordinates_3( std::list<coordinate> & coords ){
    coords.splice( coords.end(), _coords );
    return true;
}
bool Ui0::get_characters( std::list<character> & characters ){
    characters.splice( characters.end(), _chars );
    return true;
}
bool Ui0::init(){
    return true;
}
bool Ui0::deinit(){
    return true;
}
void Ui0::process_mouse_move( GLFWwindow * window, double xpos, double ypos ){
    //find the instance mapped from window resource
    auto it_range = _map_resource_to_instance.equal_range( window );
    auto it = it_range.first;
    while( it != it_range.second ){
	Ui0 * instance = it->second;
	instance->_coords.push_back( { window, xpos, ypos, 0 } );
	++it;
    }
}
void Ui0::process_mouse_button( GLFWwindow * window, int button, int action, int mods ){
    //find the instance mapped from window resource
    auto it_range = _map_resource_to_instance.equal_range( window );
    auto it = it_range.first;
    while( it != it_range.second ){
	Ui0 * instance = it->second;
	mouse_character mouse_char;
        state mouse_state;
	switch( button ){
	case GLFW_MOUSE_BUTTON_LEFT:
	    mouse_char = mouse_character::LEFT;
	    break;
	case GLFW_MOUSE_BUTTON_RIGHT:
	    mouse_char = mouse_character::RIGHT;
	    break;
	default:
	    mouse_char = mouse_character::OTHER;
	}
	switch( action ){
	case GLFW_PRESS:
	    mouse_state = state::DOWN;
	    break;
	case GLFW_RELEASE:
	    mouse_state = state::UP;
	    break;
	default:
	    mouse_state = state::OTHER;
	}
	character c;
	c._handle_resource = window;
	c._character_type = character_type::MOUSE;
	c._mouse_character = mouse_char;
	c._state = mouse_state;
	instance->_chars.push_back( std::move(c) );
	++it;
    }
}
void Ui0::process_key_input( GLFWwindow * window, int key, int scancode, int action, int mods ){
    //find the instance mapped from window resource
    auto it_range = _map_resource_to_instance.equal_range( window );
    auto it = it_range.first;
    while( it != it_range.second ){
	Ui0 * instance = it->second;
        key_character key_char = key;
        state key_state;
	switch( action ){
	case GLFW_PRESS:
	    key_state = state::DOWN;
	    break;
	case GLFW_RELEASE:
	    key_state = state::UP;
	    break;
	case GLFW_REPEAT:
	    key_state = state::REPEAT;
	    break;
	default:
	    key_state = state::OTHER;
	}
	character c;
	c._handle_resource = window;
	c._character_type = character_type::KEY;
	c._key_character = key_char;
	c._state = key_state;
	instance->_chars.push_back( std::move(c) );
	++it;
    }
}
bool Ui0::register_resource_to_monitor( handle_resource resource ){
    if( !resource )
	return false;
    //assume resource is a valid GLFWwindow *
    _resources_to_monitor.insert( resource );
    glfwSetCursorPosCallback( (GLFWwindow *)resource, process_mouse_move );
    glfwSetMouseButtonCallback( (GLFWwindow *)resource, process_mouse_button );
    glfwSetKeyCallback( (GLFWwindow *)resource, process_key_input );
    _map_resource_to_instance.insert( std::pair<handle_resource,Ui0*>( resource, this ) );
    return true;
}
