#include <chrono>
#include <thread>
#include <iostream>
#include <cassert>
#include <cstdint>

#include "i_rendernode.hpp"
#include "i_renderresource.hpp"

#include "gl_includes.hpp"
#include "gl_helper.hpp"
#include "rendergraphbuilder0.hpp"
#include "renderbackend0.hpp"
#include "memory_manager_p1t_g1_ff.hpp"

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

    std::list< ::e2::interface::i_rendernode * > nodes_init {};
    std::list< ::e2::interface::i_rendernode * > nodes_draw_loop {};
    std::list< ::e2::interface::i_rendernode * > nodes_deinit {};
    ::e2::render::rendergraphbuilder0 gb;
    ::e2::render::renderbackend0< ::e2::memory::memory_manager_p1t_g1_ff > rb;
    
    //define render tasks
    //todo: move these into render front end

    //init window
    ::e2::interface::i_rendernode_init_window n_init_win( 700, 500 );

    //init opengl program
    uint64_t program_handle;
    ::e2::interface::i_rendernode_init_program n_init_prog( &program_handle );

    //swap window buffer
    ::e2::interface::i_rendernode_swap_window_buffer n_swap_win;

    //deinint opengl program
    ::e2::interface::i_rendernode_deinit_program n_deinit_program( &program_handle );

    //deinint window
    ::e2::interface::i_rendernode_deinit_window n_deinit_window;

    //load vertex shader
    uint64_t vertex_shader_handle;
    uint64_t shader_type_vertex = ::e2::render::gl::VERTEX;
    char * source_vertex_shader = "assets/Test.vert";
    ::e2::interface::i_rendernode_load_shader n_load_shader_vert( &vertex_shader_handle, &shader_type_vertex, source_vertex_shader, ::e2::interface::e_renderresource_subtype_shader_file );

    //load frag shader
    uint64_t frag_shader_handle;
    uint64_t shader_type_frag = ::e2::render::gl::FRAGMENT;
    char * source_frag_shader = "assets/Test.frag";
    ::e2::interface::i_rendernode_load_shader n_load_shader_frag( &frag_shader_handle, &shader_type_frag, source_frag_shader, ::e2::interface::e_renderresource_subtype_shader_file );

    //bind vertex shader
    ::e2::interface::i_rendernode_bind_shader n_bind_shader_vert( &program_handle, &vertex_shader_handle );
    
    //bind fragment shader
    ::e2::interface::i_rendernode_bind_shader n_bind_shader_frag( &program_handle, &frag_shader_handle );
    
    //todo: set up and map attributes, uniform variables, frag outputs

    //link program
    ::e2::interface::i_rendernode_compute_program n_compute_program( &program_handle );

    //compile opengl program
    ::e2::interface::i_rendernode_bind_program n_bind_program( &program_handle );

    ::e2::interface::i_rendernode_bind_program n_bind_program_2( &program_handle );

    //query active attributes
    ::e2::interface::i_rendernode_query_attrib n_query_attrib( &program_handle );
        
    //query active uniforms
    ::e2::interface::i_rendernode_query_persistent n_query_persistent( &program_handle );

    //bind attrib location, vertex position
    char * attrib_name_vert_pos = "VertexPosition";
    uint64_t attrib_index_vert_pos = 0;
    ::e2::interface::i_rendernode_bind_attrib_attrib_loc n_bind_attrib_vert_pos( &program_handle, &attrib_index_vert_pos, attrib_name_vert_pos );

    //bind attrib location, vertex colour
    char * attrib_name_vert_color = "VertexColor";
    uint64_t attrib_index_vert_color = 1;
    ::e2::interface::i_rendernode_bind_attrib_attrib_loc n_bind_attrib_vert_color( &program_handle, &attrib_index_vert_color, attrib_name_vert_color );

    //bind frag location
    char * attrib_name_frag_color = "FragColor";
    uint64_t attrib_index_frag_color = 0;
    ::e2::interface::i_rendernode_bind_attrib_frag_loc n_bind_attrib_frag_color( &program_handle, &attrib_index_frag_color, attrib_name_frag_color );
    
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
    ::e2::interface::i_rendernode_init_buffer n_init_vbos( &num_buf, vbos );

    ::e2::interface::i_rendernode_deinit_buffer n_deinit_vbos( &num_buf, vbos );

    //vbos data population : vert pos
    uint64_t buf_type_array = ::e2::interface::e_renderresource_buffer_array_buffer;
    ::e2::interface::i_rendernode_bind_buffer n_bind_buffer_pos( &buf_type_array, &vbos[0] );

    ::e2::interface::i_rendernode_bind_buffer n_bind_buffer_pos_2( &buf_type_array, &vbos[0] );

    uint64_t buffer_size_pos = sizeof( positionData );
    uint64_t buffer_usage = GL_STATIC_DRAW;
    ::e2::interface::i_rendernode_store_buffer n_store_buffer_pos( &buf_type_array, &buffer_size_pos, positionData, &buffer_usage );

    ::e2::interface::i_rendernode_bind_buffer n_bind_buffer_color( &buf_type_array, &vbos[1] );

    ::e2::interface::i_rendernode_bind_buffer n_bind_buffer_color_2( &buf_type_array, &vbos[1] );

    uint64_t buffer_size_color = sizeof( colorData );
    ::e2::interface::i_rendernode_store_buffer n_store_buffer_color( &buf_type_array, &buffer_size_color, colorData, &buffer_usage );

    //vertex array object creation
    uint64_t num_va = 1;
    uint64_t vao;
    uint64_t obj_type_va = ::e2::interface::e_renderresource_subtype_object_vertex_array;
    ::e2::interface::i_rendernode_init_object_vertex_array n_init_object_va( &num_va, &vao );

    //vao bind
    ::e2::interface::i_rendernode_bind_object_vertex_array n_bind_object_va( &vao );

    ::e2::interface::i_rendernode_bind_object_vertex_array n_bind_object_va_2( &vao );

    //vertex attribute arrays enable
    ::e2::interface::i_rendernode_enable_attrib_vertex_array n_enable_vaa_pos( &attrib_index_vert_pos );

    ::e2::interface::i_rendernode_enable_attrib_vertex_array n_enable_vaa_color( &attrib_index_vert_color );

    //map data buffers to corresponding shader input variables
    uint64_t attrib_size = 3;
    bool attrib_normalized = false;
    uint64_t attrib_stride = 0;
    void * attrib_pointer = 0;
    uint64_t attrib_data_type = ::e2::interface::e_renderresource_data_type_float;
    ::e2::interface::i_rendernode_store_defineformat_vertexattrib n_defineformat_attrib_vert_pos( &attrib_index_vert_pos, &attrib_size, &attrib_data_type, &attrib_normalized, &attrib_stride, attrib_pointer );

    ::e2::interface::i_rendernode_store_defineformat_vertexattrib n_defineformat_attrib_vert_color( &attrib_index_vert_color, &attrib_size, &attrib_data_type, &attrib_normalized, &attrib_stride, attrib_pointer );

    uint64_t primitive_type = ::e2::interface::e_renderresource_primitive_triangles;
    int64_t offset = 0;
    uint64_t count = 3;
    ::e2::interface::i_rendernode_exec_drawbatch n_exec_drawbatch( &primitive_type, &offset, &count );

    ::e2::interface::i_rendernode_clear_window_buffer_colour n_clear_wind_buf_color;
    ::e2::interface::i_rendernode_clear_window_buffer_depth n_clear_wind_bud_depth;
    ::e2::interface::i_rendernode_disable_window_buffer_depth n_disable_win_buf_depth;

    ::e2::interface::i_rendernode_compute_program n_link_program( &program_handle );

    ::e2::interface::i_rendernode_query_attrib n_query_active_attrib( &program_handle );

    ::e2::interface::i_rendernode_query_persistent n_query_active_persistent( &program_handle );

    //sequencing render nodes
    //todo: move these into render front end
    {
	//setup for shaders and program
	gb.set_handle( &n_init_win );
	nodes_init.push_back( &n_init_win );

	gb.set_handle( &n_init_prog );
	gb.link_prerequisite( &n_init_prog, &n_init_win );
	nodes_init.push_back( &n_init_prog );

	gb.set_handle( &n_load_shader_vert );
	gb.link_prerequisite( &n_load_shader_vert, &n_init_prog );
	nodes_init.push_back( &n_load_shader_vert );
    
	gb.set_handle( &n_load_shader_frag );
	gb.link_prerequisite( &n_load_shader_frag, &n_init_prog );
	nodes_init.push_back( &n_load_shader_frag );

	gb.set_handle( &n_bind_shader_vert );
	gb.link_prerequisite( &n_bind_shader_vert, &n_load_shader_vert );
	nodes_init.push_back( &n_bind_shader_vert );

	gb.set_handle( &n_bind_shader_frag );
	gb.link_prerequisite( &n_bind_shader_frag, &n_load_shader_frag );
	nodes_init.push_back( &n_bind_shader_frag );
    
	//setup shader variable pipeline mapping
	gb.set_handle( &n_bind_attrib_vert_pos );
	gb.link_prerequisite( &n_bind_attrib_vert_pos, &n_bind_shader_frag );
	nodes_init.push_back( &n_bind_attrib_vert_pos );

	gb.set_handle( &n_bind_attrib_vert_color );
	gb.link_prerequisite( &n_bind_attrib_vert_color, &n_bind_attrib_vert_pos );
	nodes_init.push_back( &n_bind_attrib_vert_color );

	gb.set_handle( &n_bind_attrib_frag_color );
	gb.link_prerequisite( &n_bind_attrib_frag_color, &n_bind_attrib_vert_color );
	nodes_init.push_back( &n_bind_attrib_frag_color );

	//compile opengl program
	gb.set_handle( &n_link_program );
	gb.link_prerequisite( &n_link_program, &n_bind_attrib_frag_color );
	nodes_init.push_back( &n_link_program );

	gb.set_handle( &n_bind_program );
	gb.link_prerequisite( &n_bind_program, &n_link_program );
	nodes_init.push_back( &n_bind_program );

	gb.set_handle( &n_query_active_attrib );
	gb.link_prerequisite( &n_query_active_attrib, &n_bind_program );
	nodes_init.push_back( &n_query_active_attrib );

	gb.set_handle( &n_query_active_persistent );
	gb.link_prerequisite( &n_query_active_persistent, &n_query_active_attrib );
	nodes_init.push_back( &n_query_active_persistent );
    
	//store shader input data to buffers
	gb.set_handle( &n_init_vbos );
	gb.link_prerequisite( &n_init_vbos, &n_query_active_persistent );
	nodes_init.push_back( &n_init_vbos );

	gb.set_handle( &n_bind_buffer_pos );
	gb.link_prerequisite( &n_bind_buffer_pos, &n_init_vbos );
	nodes_init.push_back( &n_bind_buffer_pos );

	gb.set_handle( &n_store_buffer_pos );
	gb.link_prerequisite( &n_store_buffer_pos, &n_bind_buffer_pos );
	nodes_init.push_back( &n_store_buffer_pos );

	gb.set_handle( &n_bind_buffer_color );
	gb.link_prerequisite( &n_bind_buffer_color, &n_store_buffer_pos );
	nodes_init.push_back( &n_bind_buffer_color );

	gb.set_handle( &n_store_buffer_color );
	gb.link_prerequisite( &n_store_buffer_color, &n_bind_buffer_color );
	nodes_init.push_back( &n_store_buffer_color );

	gb.set_handle( &n_init_object_va );
	gb.link_prerequisite( &n_init_object_va, &n_store_buffer_color );
	nodes_init.push_back( &n_init_object_va );

	gb.set_handle( &n_bind_object_va );
	gb.link_prerequisite( &n_bind_object_va, &n_init_object_va );
	nodes_init.push_back( &n_bind_object_va );

	//map vertex array index for position buffer
	gb.set_handle( &n_bind_buffer_pos_2 );
	gb.link_prerequisite( &n_bind_buffer_pos_2, &n_bind_object_va );
	nodes_init.push_back( &n_bind_buffer_pos_2 );

	gb.set_handle( &n_enable_vaa_pos );
	gb.link_prerequisite( &n_enable_vaa_pos, &n_bind_buffer_pos_2 );
	nodes_init.push_back( &n_enable_vaa_pos );

	gb.set_handle( &n_defineformat_attrib_vert_pos );
	gb.link_prerequisite( &n_defineformat_attrib_vert_pos, &n_enable_vaa_pos );
	nodes_init.push_back( &n_defineformat_attrib_vert_pos );

	//map vertex array index for colour buffer
	gb.set_handle( &n_bind_buffer_color_2 );
	gb.link_prerequisite( &n_bind_buffer_color_2, &n_defineformat_attrib_vert_pos );
	nodes_init.push_back( &n_bind_buffer_color_2 );

	gb.set_handle( &n_enable_vaa_color );
	gb.link_prerequisite( &n_enable_vaa_color, &n_bind_buffer_color_2 );
	nodes_init.push_back( &n_enable_vaa_color );

	gb.set_handle( &n_defineformat_attrib_vert_color );
	gb.link_prerequisite( &n_defineformat_attrib_vert_color, &n_enable_vaa_color );
	nodes_init.push_back( &n_defineformat_attrib_vert_color );
    }

    {
	//drawing loop
	gb.set_handle( &n_clear_wind_buf_color);
	nodes_draw_loop.push_back( &n_clear_wind_buf_color );

	gb.set_handle( &n_disable_win_buf_depth );
	gb.link_prerequisite( &n_disable_win_buf_depth, &n_clear_wind_buf_color );
	nodes_draw_loop.push_back( &n_disable_win_buf_depth );

	gb.set_handle( &n_bind_program_2 );
	gb.link_prerequisite( &n_bind_program_2, &n_disable_win_buf_depth );
	nodes_draw_loop.push_back( &n_bind_program_2 );

	gb.set_handle( &n_bind_object_va_2 );
	gb.link_prerequisite( &n_bind_object_va_2, &n_bind_program_2 );
	nodes_draw_loop.push_back( &n_bind_object_va_2 );

	gb.set_handle( &n_exec_drawbatch );
	gb.link_prerequisite( &n_exec_drawbatch, &n_bind_object_va_2 );
	nodes_draw_loop.push_back( &n_exec_drawbatch );

	gb.set_handle( &n_swap_win );
	gb.link_prerequisite( &n_swap_win, &n_exec_drawbatch );
	nodes_draw_loop.push_back( &n_swap_win );
    }

    {
	//deinit sequence
	gb.set_handle( &n_deinit_program );
	nodes_deinit.push_back( &n_deinit_program );

	gb.set_handle( &n_deinit_vbos );
	gb.link_prerequisite( &n_deinit_vbos, &n_deinit_program );
	nodes_deinit.push_back( &n_deinit_vbos );

	gb.set_handle( &n_deinit_window );
	gb.link_prerequisite( &n_deinit_window, &n_deinit_program );
	nodes_deinit.push_back( &n_deinit_window );
    }

    //start actual sequence using render backend
    assert( rb.renderbackend_process_rendernodes( &nodes_init ) );
    assert( rb.renderbackend_process_commit() );
    assert( rb.renderbackend_process_batches(1) );

    void * win;
    GLFWwindow * win_glfw;
    assert( rb.renderbackend_get_window( &win ) );
    assert( win );
    //todo: access window functionality through an abstracted window utility
    win_glfw = (GLFWwindow *) win;
    glfwMakeContextCurrent( win_glfw );
    glfwSetKeyCallback( win_glfw, process_key_input );
    glfwSetMouseButtonCallback( win_glfw, process_mouse_button );
    glfwSetCursorPosCallback( win_glfw, process_mouse_move );
    while( false == glfwWindowShouldClose( win_glfw ) ){
	glfwMakeContextCurrent( win_glfw );
	if( quit ){
	    glfwSetWindowShouldClose( win_glfw, GLFW_TRUE);
	}
	glClearColor( 0.0f, 0.1f, 0.3f, 1.0f );

	assert( rb.renderbackend_process_rendernodes( &nodes_draw_loop ) );
	assert( rb.renderbackend_process_commit() );
	assert( rb.renderbackend_process_batches(1) );

	glfwPollEvents();
	std::this_thread::sleep_for( std::chrono::milliseconds(25) );
    }
    assert( rb.renderbackend_process_rendernodes( &nodes_deinit ) );
    assert( rb.renderbackend_process_commit() );
    assert( rb.renderbackend_process_batches(1) );

    return 0;
}
