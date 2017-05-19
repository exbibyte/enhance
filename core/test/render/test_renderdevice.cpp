#include <chrono>
#include <thread>
#include <iostream>
#include <cassert>
#include <cstdint>

#include "renderdevice_gl.hpp"
#include "gl_includes.hpp"
#include "buffer.hpp"
#include "i_renderpayload.hpp"

bool quit = false;
static void process_key_input( GLFWwindow * win, int key, int scancode, int action, int mods ){
    if( 'Q' == key ){
	quit = true;
    }
}

static void process_mouse_button( GLFWwindow * win, int button, int action, int mods ){
    
}

static void process_mouse_move( GLFWwindow * win, double xpos, double ypos ){

}

int main(){

    constexpr size_t buf_len_bytes = 1 << 20;
    std::cout << "allocated buffer of length: " << buf_len_bytes << std::endl;
    ::e2::memory::buffer buf( buf_len_bytes );

    ::e2::render::renderdevice_gl rd;

    //set window size
    uint64_t * window_dim;
    size_t offset;
    if( false == buf.buffer_get_next_available( &offset, &window_dim, 2 ) ){
	assert( false && "window dimenstion store failed." );
    }
    *window_dim++ = 700;
    *window_dim = 500;
    ::e2::interface::i_renderpayload payload;
    payload._buf = &buf;
    payload._offset = offset;
    payload._payload_type = ::e2::interface::uint_2;
    //task package for initializing window
    {
	::e2::interface::i_renderpackage pkg;
	pkg.set_render_cmd_type( ::e2::interface::e_rendercmd_type::init );
	pkg.set_render_resource_type( ::e2::interface::e_renderresource_type::windowing );
	pkg._payload = &payload;
	pkg._num_payload = 1;
	rd.renderdevice_process( pkg );
    }

    //task resource for swapping window buffer operation
    ::e2::interface::i_renderresource resource_window_buf_swap;
    resource_window_buf_swap._id = ::e2::interface::e_renderresource_window::window_buf_swap;
    
    glfwMakeContextCurrent( rd._window );
    glfwSetKeyCallback( rd._window, process_key_input );
    glfwSetMouseButtonCallback( rd._window, process_mouse_button );

    glfwSetCursorPosCallback( rd._window, process_mouse_move );
    while( false == glfwWindowShouldClose( rd._window ) ){
	glfwMakeContextCurrent( rd._window );
	if( quit ){
	    glfwSetWindowShouldClose( rd._window, GLFW_TRUE);
	}
	glClearColor( 50, 50, 50, 1.0 );

	//submit windowing buffer swap operation
	{
	    ::e2::interface::i_renderpackage pkg;
	    pkg.set_render_cmd_type( ::e2::interface::e_rendercmd_type::exec );
	    pkg.set_render_resource_type( ::e2::interface::e_renderresource_type::windowing );
	    pkg._resource = &resource_window_buf_swap;
	    pkg._num_payload = 0;
	    rd.renderdevice_process( pkg );
	}
	glfwPollEvents();
	std::this_thread::sleep_for( std::chrono::milliseconds(25) );
    }

    {
	::e2::interface::i_renderpackage pkg;
	pkg.set_render_cmd_type( ::e2::interface::e_rendercmd_type::deinit );
	pkg.set_render_resource_type( ::e2::interface::e_renderresource_type::windowing );
	pkg._num_payload = 0;
	rd.renderdevice_process( pkg );
    }
    return 0;
}
