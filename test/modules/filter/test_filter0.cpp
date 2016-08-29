#include "Ui0.hpp"
#include "FilterUiDrag.hpp"

#include "GLIncludes.hpp"

#include "imgui.h"
#include "imgui_impl_glfw_gl3.h"

#include <iostream>
#include <chrono>
#include <thread>
#include <list>

void print_info_opengl(){
    const GLubyte * renderer = glGetString( GL_RENDERER );
    const GLubyte * vendor = glGetString( GL_VENDOR );
    const GLubyte * version = glGetString( GL_VERSION );
    const GLubyte * glslVersion = glGetString( GL_SHADING_LANGUAGE_VERSION );
    
    GLint major, minor;
    glGetIntegerv(GL_MAJOR_VERSION, &major);
    glGetIntegerv(GL_MINOR_VERSION, &minor);
    printf("GL Vendor: %s\n", vendor);
    printf("GL Renderer : %s\n", renderer);
    printf("GL Version (string) : %s\n", version);
    printf("GL Version (integer) : %d.%d\n", major, minor);
    printf("GLSL Version : %s\n", glslVersion);
}

int main(){

    Ui0 ui;
    FilterUiDrag filtdrag;

    if ( !glfwInit() ) {
    	printf( "failed to initialize GLFW.\n" );
    	return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    int _width = 500;
    int _height = 500;
    auto _window = glfwCreateWindow( _width, _height, "Render Window", nullptr, nullptr );
    if ( !_window ) {
	return -1;
    }

    glfwMakeContextCurrent( _window );

    if (gl3wInit()) {
	printf( "failed to initialize OpenGL\n" );
	return -1;
    }

    print_info_opengl();

    glEnable( GL_DEPTH_TEST );
    glClearColor( 0, 0, 0, 1.0 );

    std::cout << "InitGL::init invoked." << std::endl;

    ui.register_resource_to_monitor( _window );

    std::list<IUi::character> characters{};
    
    std::cout << "mouse coordinates" << std::endl;
    while(1){
	glfwPollEvents();
	
	ui.get_characters( characters );

	for( auto & i : characters ){
	    if( IUi::input_type::MOUSE == i._input_type ){
		if( IUi::mouse_character::LEFT == i._mouse_character ){
		    std::cout << "mouse L ";
		}
		else if( IUi::mouse_character::RIGHT == i._mouse_character ){
		    std::cout << "mouse R ";
		}
		else if( IUi::mouse_character::MID == i._mouse_character ){
		    std::cout << "mouse M ";
		}

		if( IUi::state::DOWN == i._state ){
		    std::cout << "down" << std::endl;
		}
		else if( IUi::state::UP == i._state ){
		    std::cout << "up" << std::endl;
		}
	    }
	    else if( IUi::input_type::KEY == i._input_type ){
		std::cout << "key " << i._key_character << " ";

		if( IUi::state::DOWN == i._state ){
		    std::cout << "down" << std::endl;
		    if( 'Q' == i._key_character ){
			std::cout << "exit" << std::endl;
			return 0;
		    }
		}
		else if( IUi::state::UP == i._state ){
		    std::cout << "up" << std::endl;
		}
		else if( IUi::state::REPEAT == i._state ){
		    std::cout << "repeat" << std::endl;
		}
	    }else{ //MOUSE_COORD
		// std::cout << "x: " << i._coordinate._a << ", y: " << i._coordinate._b << std::endl;
	    }
	}
	
	std::list<drag_coordinate> drag{};
	filtdrag.process( drag, characters );
	for( auto & i : drag ){
	    if( IUi::mouse_character::LEFT == i._mouse_character ){
		std::cout << "drag: left, ";
	    }else if( IUi::mouse_character::RIGHT == i._mouse_character ){
		std::cout << "drag: right, ";
	    }else if( IUi::mouse_character::MID == i._mouse_character ){
		std::cout << "drag: mid, ";
	    }else {
		continue;
		// std::cout << "drag: other, ";
	    }
	    std::cout << "x: " << i._coordinate_delta._a << ", y: " << i._coordinate_delta._b << std::endl;
	}
			  
        characters.clear();
	glfwSwapBuffers( _window );
	std::this_thread::sleep_for(std::chrono::milliseconds(25));
    }

    
    return 0;
}
