#include <string>
#include <cstring>
#include <iostream>
#include <fstream>

#include "Ui0.hpp"

#include <GLFW/glfw3.h>

#include <list>
#include <set>
#include <unordered_map>
#include <utility>

std::unordered_multimap<IUi::handle_resource, Ui0*> Ui0:: _map_resource_to_instance {};

bool Ui0::get_characters( std::list<character> & characters ){
    characters.splice( characters.end(), _chars );
    return true;
}
bool Ui0::init(){
    _debug = true;
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
	character c;
	c._handle_resource = window;
	c._input_type = input_type::MOUSE_COORD;
        c._coordinate = { xpos, ypos, 0 };
	if(instance->_debug)
	    std::cout << "mouse coordinate: ( " << xpos << ", " << ypos << " )" << std::endl;
	instance->_chars.push_back( std::move(c) );
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
	{
	    mouse_char = mouse_character::LEFT;
	    if(instance->_debug)
		std::cout << "mouse L ";
	}
	break;
	case GLFW_MOUSE_BUTTON_RIGHT:
	{
	    mouse_char = mouse_character::RIGHT;
	    if(instance->_debug)
		std::cout << "mouse R ";
	}
	break;
	case GLFW_MOUSE_BUTTON_MIDDLE:
	{
	    mouse_char = mouse_character::MID;
	    if(instance->_debug)
		std::cout << "mouse M ";
	}
	break;
	default:
	{
	    mouse_char = mouse_character::OTHER;
	    if(instance->_debug)
		std::cout << "mouse other ";
	}
	}
	switch( action ){
	case GLFW_PRESS:
	{
	    mouse_state = state::DOWN;
	    if(instance->_debug)
		std::cout << "mouse down" << std::endl;
	}
	break;
	case GLFW_RELEASE:
	{
	    mouse_state = state::UP;
	    if(instance->_debug)
		std::cout << "mouse up" << std::endl;
	}
	break;
	default:
	{
	    mouse_state = state::OTHER;
	    if(instance->_debug)
		std::cout << "mouse other" << std::endl;
	}
	}
	character c;
	c._handle_resource = window;
	c._input_type = input_type::MOUSE;
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
	if(instance->_debug)
	    std::cout << "key " << key;
        state key_state;
	switch( action ){
	case GLFW_PRESS:
	{
	    key_state = state::DOWN;
	    if(instance->_debug)
		std::cout << "down" << std::endl;
	}
	break;
	case GLFW_RELEASE:
	{
	    key_state = state::UP;
	    if(instance->_debug)
		std::cout << "up" << std::endl;
	}
	break;
	case GLFW_REPEAT:
	{
	    key_state = state::REPEAT;
	    if(instance->_debug)
		std::cout << "repeat" << std::endl;
	}
	break;
	default:
	{
	    key_state = state::OTHER;
	    if(instance->_debug)
		std::cout << "other" << std::endl;
	}
	}
	character c;
	c._handle_resource = window;
	c._input_type = input_type::KEY;
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
bool Ui0::deregister_resource_to_monitor( handle_resource resource ){
    if( !resource )
	return false;
    //assume resource is a valid GLFWwindow *
    _resources_to_monitor.erase( resource );
    glfwSetCursorPosCallback( (GLFWwindow *)resource, 0 );
    glfwSetMouseButtonCallback( (GLFWwindow *)resource, 0 );
    glfwSetKeyCallback( (GLFWwindow *)resource, 0 );
    _map_resource_to_instance.erase( resource );
    return true;
}
bool Ui0::clear_characters(){
    _chars.clear();
}
