#include <chrono>
#include <thread>
#include <iostream>
#include <cassert>
#include <cstdint>

#include "renderpackage_gl.hpp"
#include "renderdevice_gl.hpp"
#include "gl_includes.hpp"
#include "buffer.hpp"
#include "i_renderpayload.hpp"
#include "gl_helper.hpp"
#include "renderbackend_gl.hpp"

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
    ::e2::render::renderbackend_gl renderback;

    //serialize tasks
    //init window
    ::e2::interface::i_renderpackage * pkg_init_window;
    renderback.init_window( &buf, &pkg_init_window, 700, 500 );

    //init opengl program
    ::e2::interface::i_renderpackage * pkg_init_program;
    uint64_t program_handle;
    uint64_t * p_program_handle = &program_handle;
    renderback.init_program( &buf, &pkg_init_program, p_program_handle );

    //swap window buffer
    ::e2::interface::i_renderpackage * pkg_swap_window_buffer;
    renderback.swap_window_buffer( &buf, &pkg_swap_window_buffer );

    //deinint opengl program
    ::e2::interface::i_renderpackage * pkg_deinit_program;
    renderback.deinit_program( &buf, &pkg_deinit_program, p_program_handle );

    //deinint window
    ::e2::interface::i_renderpackage * pkg_deinit_window;
    renderback.deinit_window( &buf, &pkg_deinit_window );

    //load vertex shader
    ::e2::interface::i_renderpackage * pkg_load_shader_vertex;
    uint64_t vertex_shader_handle;
    uint64_t shader_type_vertex = ::e2::render::gl::VERTEX;
    char * source_vertex_shader = "assets/ADS.vert";
    renderback.load_shader( &buf, &pkg_load_shader_vertex, &vertex_shader_handle, &shader_type_vertex, source_vertex_shader, ::e2::interface::e_renderresource_subtype_shader_file );

    //load frag shader
    ::e2::interface::i_renderpackage * pkg_load_shader_frag;
    uint64_t frag_shader_handle;
    uint64_t shader_type_frag = ::e2::render::gl::FRAGMENT;
    char * source_frag_shader = "assets/ADS.frag";
    renderback.load_shader( &buf, &pkg_load_shader_frag, &frag_shader_handle, &shader_type_frag, source_frag_shader, ::e2::interface::e_renderresource_subtype_shader_file );

    //bind vertex shader
    ::e2::interface::i_renderpackage * pkg_bind_shader_vertex;
    renderback.bind_shader( &buf, &pkg_bind_shader_vertex, p_program_handle, &vertex_shader_handle );
    //bind fragment shader
    ::e2::interface::i_renderpackage * pkg_bind_shader_frag;
    renderback.bind_shader( &buf, &pkg_bind_shader_frag, p_program_handle, &frag_shader_handle );

    //todo: set up and map attributes, uniform variables, frag outputs

    //compile opengl program
    ::e2::interface::i_renderpackage * pkg_bind_program;
    renderback.bind_program( &buf, &pkg_bind_program, p_program_handle );

    //start sequences
    rd.renderdevice_process( *pkg_init_window );
    rd.renderdevice_process( *pkg_init_program );
    rd.renderdevice_process( *pkg_load_shader_vertex );
    rd.renderdevice_process( *pkg_load_shader_frag );
    rd.renderdevice_process( *pkg_bind_shader_vertex );
    rd.renderdevice_process( *pkg_bind_shader_frag );
    //todo: attributes, uniforms, frag output setup
    // rd.renderdevice_process( *pkg_bind_program ); 

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
	rd.renderdevice_process( *pkg_swap_window_buffer );
	glfwPollEvents();
	std::this_thread::sleep_for( std::chrono::milliseconds(25) );
    }
    rd.renderdevice_process( *pkg_deinit_program );
    rd.renderdevice_process( *pkg_deinit_window );
    double frac_free;
    buf.buffer_stat_fraction_free( &frac_free );
    std::cout << "buffer fraction free: " << frac_free << std::endl;
    return 0;
}
