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
    ::e2::interface::i_renderpackage * pkg_init_window;
    void ** data_ptr;
    {
	int * data;
	size_t offset;
	if( false == buf.buffer_get_next_available( &offset, &data, 2 ) ){
	    assert( false && "mem buffer for init window failed." );
	}
	int * data_start = data;
	*data++ = 700;
	*data++ = 500;
	if( false == buf.buffer_get_next_available( &offset, &data_ptr, 2 ) ){
	    assert( false && "mem buffer for init window failed." );
	}
	data_ptr[0] = data_start;
	data_ptr[1] = &data_start[1];
	uint64_t * key;
	if( false == buf.buffer_get_next_available( &offset, &key, 2 ) ){
	    assert( false && "mem buffer for init window failed." );
	}
	key[0] = ::e2::interface::e_renderresourcekey_wind_dimx;
	key[1] = ::e2::interface::e_renderresourcekey_wind_dimy;
	assert( ::e2::render::renderpackage_gl::pack( &buf, &pkg_init_window, key, (void**)data_ptr, 2 ) );
	pkg_init_window->set_render_cmd_type( ::e2::interface::e_rendercmd_type_init );
	pkg_init_window->set_render_resource_type( ::e2::interface::e_renderresource_type_windowing );
	pkg_init_window->_resource_subtype = ::e2::interface::e_renderresource_subtype_na;
    }
    rd.renderdevice_process( *pkg_init_window );

    // //create program
    // uint64_t * program_handle;
    // {
    // 	uint64_t * data;
    // 	size_t offset;
    // 	if( false == buf.buffer_get_next_available( &offset, &data, 2 ) ){
    // 	    assert( false && "mem buffer for create program failed." );
    // 	}
    // 	*data++ = 1;
    // 	program_handle = data;
    // 	::e2::interface::i_renderresource resource_create_program;
    //     resource_create_program._empty_payload = false;
    //     resource_create_program._offset_payload = 0;
    // 	::e2::interface::i_renderpayload payload_create_program;
    // 	payload_create_program._buf = &buf;
    //     payload_create_program._offset = offset;
    // 	payload_create_program._payload_type = ::e2::interface::uint_n;
	
    // 	::e2::interface::i_renderpackage pkg;
    // 	pkg.set_render_cmd_type( ::e2::interface::e_rendercmd_type::init );
    // 	pkg.set_render_resource_type( ::e2::interface::e_renderresource_type::program );
    // 	pkg._payload = &payload_create_program;
    // 	pkg._resource = &resource_create_program;
    // 	pkg._num_payload = 1;
    // 	pkg._num_resource = 1;
    // 	rd.renderdevice_process( pkg );
    // }
    // //load shaders
    // uint64_t * shader_vertex;
    // char * shader_vertex_source = "assets/ADS.vert";
    // {
    // 	uint64_t * data;
    // 	size_t offset;
    // 	if( false == buf.buffer_get_next_available( &offset, &data, 4 ) ){
    // 	    assert( false && "mem buffer for loading vertex shader failed." );
    // 	}
    // 	*data++ = 3;
    // 	shader_vertex = data++;
    // 	uint64_t shader_vertex_source_casted = *reinterpret_cast<uint64_t*>( shader_vertex_source );
    // 	*data++ = shader_vertex_source_casted;
    // 	*data = ::e2::render::gl::VERTEX;
	
    // 	::e2::interface::i_renderresource resource_load_shader;
    // 	resource_load_shader._id = ::e2::interface::renderresource_shader_file;
    //     resource_load_shader._empty_payload = false;
    //     resource_load_shader._offset_payload = 0;
    // 	::e2::interface::i_renderpayload payload_load_shader;
    // 	payload_load_shader._buf = &buf;
    //     payload_load_shader._offset = offset;
    //     payload_load_shader._payload_type = ::e2::interface::uint_n;
	
    // 	::e2::interface::i_renderpackage pkg;
    // 	pkg.set_render_cmd_type( ::e2::interface::e_rendercmd_type::load );
    // 	pkg.set_render_resource_type( ::e2::interface::e_renderresource_type::shader );
    // 	pkg._payload = &payload_load_shader;
    // 	pkg._resource = &resource_load_shader;
    // 	pkg._num_payload = 1;
    // 	pkg._num_resource = 1;
    // 	rd.renderdevice_process( pkg );
    // }
    // uint64_t * shader_frag;
    // char * shader_frag_source = "assets/ADS.frag";
    // {
    // 	uint64_t * data;
    // 	size_t offset;
    // 	if( false == buf.buffer_get_next_available( &offset, &data, 4 ) ){
    // 	    assert( false && "mem buffer for loading fragment shader failed." );
    // 	}
    // 	*data++ = 3;
    // 	shader_frag = data++;
    // 	*data++ = reinterpret_cast<uint64_t>(shader_frag_source);
    // 	*data = ::e2::render::gl::FRAGMENT;
	
    // 	::e2::interface::i_renderresource resource_load_shader;
    // 	resource_load_shader._id = ::e2::interface::renderresource_shader_file;
    //     resource_load_shader._empty_payload = false;
    //     resource_load_shader._offset_payload = 0;
    // 	::e2::interface::i_renderpayload payload_load_shader;
    // 	payload_load_shader._buf = &buf;
    //     payload_load_shader._offset = offset;
    //     payload_load_shader._payload_type = ::e2::interface::uint_n;
	
    // 	::e2::interface::i_renderpackage pkg;
    // 	pkg.set_render_cmd_type( ::e2::interface::e_rendercmd_type::load );
    // 	pkg.set_render_resource_type( ::e2::interface::e_renderresource_type::shader );
    // 	pkg._payload = &payload_load_shader;
    // 	pkg._resource = &resource_load_shader;
    // 	pkg._num_payload = 1;
    // 	pkg._num_resource = 1;
    // 	rd.renderdevice_process( pkg );
    // }

    // //bind shaders to program
    // {
    // 	uint64_t * data;
    // 	size_t offset;
    // 	if( false == buf.buffer_get_next_available( &offset, &data, 3 ) ){
    // 	    assert( false && "mem buffer for binding vertex shader failed." );
    // 	}
    // 	*data++ = 2;
    // 	*data++ = *program_handle;
    // 	*data++ = *shader_vertex;
	
    // 	::e2::interface::i_renderresource resource_bind_shader;
    //     resource_bind_shader._empty_payload = false;
    //     resource_bind_shader._offset_payload = 0;
    // 	::e2::interface::i_renderpayload payload_bind_shader;
    // 	payload_bind_shader._buf = &buf;
    //     payload_bind_shader._offset = offset;
    //     payload_bind_shader._payload_type = ::e2::interface::uint_n;
	
    // 	::e2::interface::i_renderpackage pkg;
    // 	pkg.set_render_cmd_type( ::e2::interface::e_rendercmd_type::bind );
    // 	pkg.set_render_resource_type( ::e2::interface::e_renderresource_type::shader );
    // 	pkg._payload = &payload_bind_shader;
    // 	pkg._resource = &resource_bind_shader;
    // 	pkg._num_payload = 1;
    // 	pkg._num_resource = 1;
    // 	rd.renderdevice_process( pkg );
    // }
    // {
    // 	uint64_t * data;
    // 	size_t offset;
    // 	if( false == buf.buffer_get_next_available( &offset, &data, 3 ) ){
    // 	    assert( false && "mem buffer for binding fragment shader failed." );
    // 	}
    // 	*data++ = 2;
    // 	*data++ = *program_handle;
    // 	*data++ = *shader_frag;
	
    // 	::e2::interface::i_renderresource resource_bind_shader;
    //     resource_bind_shader._empty_payload = false;
    //     resource_bind_shader._offset_payload = 0;
    // 	::e2::interface::i_renderpayload payload_bind_shader;
    // 	payload_bind_shader._buf = &buf;
    //     payload_bind_shader._offset = offset;
    //     payload_bind_shader._payload_type = ::e2::interface::uint_n;
	
    // 	::e2::interface::i_renderpackage pkg;
    // 	pkg.set_render_cmd_type( ::e2::interface::e_rendercmd_type::bind );
    // 	pkg.set_render_resource_type( ::e2::interface::e_renderresource_type::shader );
    // 	pkg._payload = &payload_bind_shader;
    // 	pkg._resource = &resource_bind_shader;
    // 	pkg._num_payload = 1;
    // 	pkg._num_resource = 1;
    // 	rd.renderdevice_process( pkg );
    // }


    // //compile program
    // {
    // 	uint64_t * data;
    // 	size_t offset;
    // 	if( false == buf.buffer_get_next_available( &offset, &data, 2 ) ){
    // 	    assert( false && "mem buffer for binding fragment shader failed." );
    // 	}
    // 	*data++ = 1;
    // 	*data = *program_handle;
	
    // 	::e2::interface::i_renderresource resource_compile_program;
    // 	resource_compile_program._empty_payload = false;
    // 	resource_compile_program._offset_payload = 0;
    // 	::e2::interface::i_renderpayload payload_compile_program;
    // 	payload_compile_program._buf = &buf;
    //     payload_compile_program._offset = offset;
    //     payload_compile_program._payload_type = ::e2::interface::uint_n;
	
    // 	::e2::interface::i_renderpackage pkg;
    // 	pkg.set_render_cmd_type( ::e2::interface::e_rendercmd_type::compute );
    // 	pkg.set_render_resource_type( ::e2::interface::e_renderresource_type::program );
    // 	pkg._payload = &payload_compile_program;
    // 	pkg._resource = &resource_compile_program;
    // 	pkg._num_payload = 1;
    // 	pkg._num_resource = 1;
    // 	rd.renderdevice_process( pkg );
    // }

    //task for swapping window buffer operation
    ::e2::interface::i_renderpackage * pkg_swap_window_buffer;
    {
	assert( ::e2::render::renderpackage_gl::pack( &buf, &pkg_swap_window_buffer, nullptr, nullptr, 0 ) );
	pkg_swap_window_buffer->set_render_cmd_type( ::e2::interface::e_rendercmd_type_exec );
	pkg_swap_window_buffer->set_render_resource_type( ::e2::interface::e_renderresource_type_windowing );
	pkg_swap_window_buffer->_resource_subtype = ::e2::interface::e_renderresource_subtype_window_buf_swap;
    }
    
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
	rd.renderdevice_process( *pkg_swap_window_buffer );
	glfwPollEvents();
	std::this_thread::sleep_for( std::chrono::milliseconds(25) );
    }

    {
	::e2::interface::i_renderpackage * pkg_deinit_window;
	assert( ::e2::render::renderpackage_gl::pack( &buf, &pkg_deinit_window, nullptr, nullptr, 0 ) );
        pkg_deinit_window->set_render_cmd_type( ::e2::interface::e_rendercmd_type_deinit );
        pkg_deinit_window->set_render_resource_type( ::e2::interface::e_renderresource_type_windowing );
        pkg_deinit_window->_resource_subtype = ::e2::interface::e_renderresource_subtype_na;
	rd.renderdevice_process( *pkg_deinit_window );
    }

    return 0;
}
