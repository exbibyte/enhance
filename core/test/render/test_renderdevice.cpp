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
    char * source_vertex_shader = "assets/Test.vert";
    renderback.load_shader( &buf, &pkg_load_shader_vertex, &vertex_shader_handle, &shader_type_vertex, source_vertex_shader, ::e2::interface::e_renderresource_subtype_shader_file );

    //load frag shader
    ::e2::interface::i_renderpackage * pkg_load_shader_frag;
    uint64_t frag_shader_handle;
    uint64_t shader_type_frag = ::e2::render::gl::FRAGMENT;
    char * source_frag_shader = "assets/Test.frag";
    renderback.load_shader( &buf, &pkg_load_shader_frag, &frag_shader_handle, &shader_type_frag, source_frag_shader, ::e2::interface::e_renderresource_subtype_shader_file );

    //bind vertex shader
    ::e2::interface::i_renderpackage * pkg_bind_shader_vertex;
    renderback.bind_shader( &buf, &pkg_bind_shader_vertex, p_program_handle, &vertex_shader_handle );
    //bind fragment shader
    ::e2::interface::i_renderpackage * pkg_bind_shader_frag;
    renderback.bind_shader( &buf, &pkg_bind_shader_frag, p_program_handle, &frag_shader_handle );

    //todo: set up and map attributes, uniform variables, frag outputs

    //link program
    ::e2::interface::i_renderpackage * pkg_link_program;
    renderback.compute_program( &buf, &pkg_link_program, p_program_handle );

    //compile opengl program
    ::e2::interface::i_renderpackage * pkg_bind_program;
    renderback.bind_program( &buf, &pkg_bind_program, p_program_handle );

    //query active attributes
    ::e2::interface::i_renderpackage * pkg_query_active_attrib;
    renderback.query_attrib( &buf, &pkg_query_active_attrib, p_program_handle );

    //query active uniforms
    ::e2::interface::i_renderpackage * pkg_query_active_persistent;
    renderback.query_persistent( &buf, &pkg_query_active_persistent, p_program_handle );

    //bind attrib location, vertex position
    char * attrib_name_vert_pos = "VertexPosition";
    uint64_t attrib_index_vert_pos = 0;
    ::e2::interface::i_renderpackage * pkg_bind_attrib_loc_vert_pos;
    renderback.bind_attrib( &buf, &pkg_bind_attrib_loc_vert_pos, p_program_handle, ::e2::interface::e_renderresource_subtype_attrib_attrib_location, &attrib_index_vert_pos, attrib_name_vert_pos );

    //bind attrib location, vertex colour
    char * attrib_name_vert_color = "VertexColor";
    uint64_t attrib_index_vert_color = 1;
    ::e2::interface::i_renderpackage * pkg_bind_attrib_loc_vert_color;
    renderback.bind_attrib( &buf, &pkg_bind_attrib_loc_vert_color, p_program_handle, ::e2::interface::e_renderresource_subtype_attrib_attrib_location, &attrib_index_vert_color, attrib_name_vert_color );

    //bind frag location
    char * attrib_name_frag_color = "FragColor";
    uint64_t attrib_index_frag_color = 0;
    ::e2::interface::i_renderpackage * pkg_bind_attrib_loc_frag_color;
    renderback.bind_attrib( &buf, &pkg_bind_attrib_loc_frag_color, p_program_handle, ::e2::interface::e_renderresource_subtype_attrib_frag_location, &attrib_index_frag_color, attrib_name_frag_color );

    //shader input data definition
    uint64_t vaoHandle;	
    float positionData[] = {
	-0.8f, -0.8f, 0.0f,
	0.8f, -0.8f, 0.0f,
	0.0f, 0.8f, 0.0f };
    float colorData[] = {
	1.0f, 0.0f, 0.0f,
	0.0f, 1.0f, 0.0f,
	0.0f, 0.0f, 1.0f };

    //vbos handle creation
    uint64_t num_buf = 2;
    uint64_t vbos[2];
    ::e2::interface::i_renderpackage * pkg_init_vbos;
    renderback.init_buffer( &buf, &pkg_init_vbos, &num_buf, vbos );

    //vbos data population : vert pos
    uint64_t buf_type_array = ::e2::interface::e_renderresource_buffer_array_buffer;
    ::e2::interface::i_renderpackage * pkg_bind_pos_buf;
    renderback.bind_buffer( &buf, &pkg_bind_pos_buf, &buf_type_array, &vbos[0] );
    ::e2::interface::i_renderpackage * pkg_store_pos_buf;
    uint64_t buffer_size_pos = sizeof( positionData );
    uint64_t buffer_usage = GL_STATIC_DRAW;
    renderback.store_buffer( &buf, &pkg_store_pos_buf, &buf_type_array, &buffer_size_pos, positionData, &buffer_usage );

    ::e2::interface::i_renderpackage * pkg_bind_color_buf;
    renderback.bind_buffer( &buf, &pkg_bind_color_buf, &buf_type_array, &vbos[1] );
    ::e2::interface::i_renderpackage * pkg_store_color_buf;
    uint64_t buffer_size_color = sizeof( colorData );
    renderback.store_buffer( &buf, &pkg_store_color_buf, &buf_type_array, &buffer_size_color, colorData, &buffer_usage );

    //vertex array object creation
    ::e2::interface::i_renderpackage * pkg_init_object_va;
    uint64_t num_va = 1;
    uint64_t vao;
    uint64_t obj_type_va = ::e2::interface::e_renderresource_subtype_object_vertex_array;
    renderback.init_object( &buf, &pkg_init_object_va, obj_type_va, &num_va, &vao );

    //vao bind
    ::e2::interface::i_renderpackage * pkg_bind_object_va;
    renderback.bind_object( &buf, &pkg_bind_object_va, obj_type_va, &vao );
    
    //vertex attribute arrays enable
    uint64_t attrib_index_pos = 0;
    ::e2::interface::i_renderpackage * pkg_enable_vaa_pos;
    renderback.enable_attrib( &buf, &pkg_enable_vaa_pos, obj_type_va, &attrib_index_pos );

    uint64_t attrib_index_color = 1;
    ::e2::interface::i_renderpackage * pkg_enable_vaa_color;
    renderback.enable_attrib( &buf, &pkg_enable_vaa_color, obj_type_va, &attrib_index_color );

    //map data buffers to corresponding shader input variables
    ::e2::interface::i_renderpackage * pkg_map_attrib_vert_pos;
    uint64_t attrib_size = 3;
    bool attrib_normalized = false;
    uint64_t attrib_stride = 0;
    void * attrib_pointer = 0;
    uint64_t attrib_data_type = ::e2::interface::e_renderresource_data_type_float;
    renderback.store_defineformat_vertexattrib( &buf, &pkg_map_attrib_vert_pos, &attrib_index_vert_pos, &attrib_size, &attrib_data_type, &attrib_normalized, &attrib_stride, attrib_pointer );
    ::e2::interface::i_renderpackage * pkg_map_attrib_vert_color;
    renderback.store_defineformat_vertexattrib( &buf, &pkg_map_attrib_vert_color, &attrib_index_vert_color, &attrib_size, &attrib_data_type, &attrib_normalized, &attrib_stride, attrib_pointer );

    ::e2::interface::i_renderpackage * pkg_draw_batch;
    uint64_t primitive_type = ::e2::interface::e_renderresource_primitive_triangles;
    int64_t offset = 0;
    uint64_t count = 3;
    renderback.exec_drawbatch( &buf, &pkg_draw_batch, &primitive_type, &offset, &count );
	
    //setup for shaders and program
    rd.renderdevice_process( *pkg_init_window );
    rd.renderdevice_process( *pkg_init_program );
    rd.renderdevice_process( *pkg_load_shader_vertex );
    rd.renderdevice_process( *pkg_load_shader_frag );
    rd.renderdevice_process( *pkg_bind_shader_vertex );
    rd.renderdevice_process( *pkg_bind_shader_frag );
    
    //setup shader variable pipeline mapping
    rd.renderdevice_process( *pkg_bind_attrib_loc_vert_pos );
    rd.renderdevice_process( *pkg_bind_attrib_loc_vert_color );
    rd.renderdevice_process( *pkg_bind_attrib_loc_frag_color );

    //compile opengl program
    rd.renderdevice_process( *pkg_link_program );
    rd.renderdevice_process( *pkg_bind_program );
    rd.renderdevice_process( *pkg_query_active_attrib );
    rd.renderdevice_process( *pkg_query_active_persistent );
    
    //store shader input data to buffers
    rd.renderdevice_process( *pkg_init_vbos );
    rd.renderdevice_process( *pkg_bind_pos_buf );
    rd.renderdevice_process( *pkg_store_pos_buf );
    rd.renderdevice_process( *pkg_bind_color_buf );
    rd.renderdevice_process( *pkg_store_color_buf );

    //create vertex array object to map different buffer data to one entity
    rd.renderdevice_process( *pkg_init_object_va );
    
    rd.renderdevice_process( *pkg_bind_object_va );
    //map vertex array index for position buffer
    rd.renderdevice_process( *pkg_bind_pos_buf);
    rd.renderdevice_process( *pkg_enable_vaa_pos );
    rd.renderdevice_process( *pkg_map_attrib_vert_pos);
    //map vertex array index for colour buffer
    rd.renderdevice_process( *pkg_bind_color_buf);
    rd.renderdevice_process( *pkg_enable_vaa_color );
    rd.renderdevice_process( *pkg_map_attrib_vert_color);
	
    glfwMakeContextCurrent( rd._window );
    glfwSetKeyCallback( rd._window, process_key_input );
    glfwSetMouseButtonCallback( rd._window, process_mouse_button );
    glfwSetCursorPosCallback( rd._window, process_mouse_move );
    while( false == glfwWindowShouldClose( rd._window ) ){
	glfwMakeContextCurrent( rd._window );
	if( quit ){
	    glfwSetWindowShouldClose( rd._window, GLFW_TRUE);
	}
	glClearColor( 0.0f, 0.1f, 0.3f, 1.0f );
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glDisable( GL_DEPTH_TEST );
	rd.renderdevice_process( *pkg_bind_program );
	rd.renderdevice_process( *pkg_bind_object_va);
	rd.renderdevice_process( *pkg_draw_batch );
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
