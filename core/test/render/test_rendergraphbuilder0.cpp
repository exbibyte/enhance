#include <chrono>
#include <thread>
#include <iostream>
#include <cassert>
#include <cstdint>

#include "i_rendernode.hpp"
#include "rendertaskpackager_gl.hpp"
#include "renderdevice_gl.hpp"
#include "gl_includes.hpp"
#include "buffer.hpp"
#include "i_renderpayload.hpp"
#include "gl_helper.hpp"
#include "rendertaskpackager_gl.hpp"
#include "rendergraphbuilder0.hpp"
#include "i_rendernode.hpp"
#include "renderpkgexec0.hpp"
#include "rendergraphconverter_gl.hpp"
#include "rendergraphscheduler0.hpp"

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
    ::e2::render::rendertaskpackager_gl packager;

    std::list< ::e2::interface::i_rendernode * > nodes_init {};
    std::list< ::e2::interface::i_rendernode * > nodes_draw_loop {};
    std::list< ::e2::interface::i_rendernode * > nodes_deinit {};
    ::e2::render::rendergraphbuilder0 gb_init;
    ::e2::render::rendergraphbuilder0 gb_draw_loop;
    ::e2::render::rendergraphbuilder0 gb_deinit;

    ::e2::render::renderpkgexec0 executer;
    ::e2::render::rendergraphconverter_gl converter;
    
    //serialize tasks
    //init window
    ::e2::interface::i_rendernode_init_window n_init_win;
    n_init_win._x = 700;
    n_init_win._y = 500;

    //init opengl program
    uint64_t program_handle;
    ::e2::interface::i_rendernode_init_program n_init_prog;
    n_init_prog._program_handle = &program_handle;

    //swap window buffer
    ::e2::interface::i_rendernode_swap_window_buffer n_swap_win;

    //deinint opengl program
    ::e2::interface::i_rendernode_deinit_program n_deinit_program;
    n_deinit_program._program_handle = &program_handle;

    //deinint window
    ::e2::interface::i_renderpackage * pkg_deinit_window;
    ::e2::interface::i_rendernode_deinit_window n_deinit_window;

    //load vertex shader
    uint64_t vertex_shader_handle;
    uint64_t shader_type_vertex = ::e2::render::gl::VERTEX;
    char * source_vertex_shader = "assets/Test.vert";
    ::e2::interface::i_rendernode_load_shader n_load_shader_vert;
    n_load_shader_vert._shader_handle = &vertex_shader_handle;
    n_load_shader_vert._shader_type = &shader_type_vertex;
    n_load_shader_vert._source = source_vertex_shader;
    n_load_shader_vert._source_type = ::e2::interface::e_renderresource_subtype_shader_file;

    //load frag shader
    uint64_t frag_shader_handle;
    uint64_t shader_type_frag = ::e2::render::gl::FRAGMENT;
    char * source_frag_shader = "assets/Test.frag";
    ::e2::interface::i_rendernode_load_shader n_load_shader_frag;
    n_load_shader_frag._shader_handle = &frag_shader_handle;
    n_load_shader_frag._shader_type = &shader_type_frag;
    n_load_shader_frag._source = source_frag_shader;
    n_load_shader_frag._source_type = ::e2::interface::e_renderresource_subtype_shader_file;

    //bind vertex shader
    ::e2::interface::i_rendernode_bind_shader n_bind_shader_vert;
    n_bind_shader_vert._program_handle = &program_handle;
    n_bind_shader_vert._shader_handle = &vertex_shader_handle;
    
    //bind fragment shader
    ::e2::interface::i_rendernode_bind_shader n_bind_shader_frag;
    n_bind_shader_frag._program_handle = &program_handle;
    n_bind_shader_frag._shader_handle = &frag_shader_handle;
    
    //todo: set up and map attributes, uniform variables, frag outputs

    //link program
    ::e2::interface::i_rendernode_compute_program n_compute_program;
    n_compute_program._program_handle = &program_handle;

    //compile opengl program
    ::e2::interface::i_rendernode_bind_program n_bind_program;
    n_bind_program._program_handle = &program_handle;

    ::e2::interface::i_rendernode_bind_program n_bind_program_2 = n_bind_program;

    //query active attributes
    ::e2::interface::i_rendernode_query_attrib n_query_attrib;
    n_query_attrib._program_handle = &program_handle;
        
    //query active uniforms
    ::e2::interface::i_rendernode_query_persistent n_query_persistent;
    n_query_persistent._program_handle = &program_handle;

    //bind attrib location, vertex position
    char * attrib_name_vert_pos = "VertexPosition";
    uint64_t attrib_index_vert_pos = 0;
    ::e2::interface::i_rendernode_bind_attrib n_bind_attrib_vert_pos;
    n_bind_attrib_vert_pos._program_handle = &program_handle;
    n_bind_attrib_vert_pos._location_type = ::e2::interface::e_renderresource_subtype_attrib_attrib_location;
    n_bind_attrib_vert_pos._index = &attrib_index_vert_pos;
    n_bind_attrib_vert_pos._var_name = attrib_name_vert_pos;

    //bind attrib location, vertex colour
    char * attrib_name_vert_color = "VertexColor";
    uint64_t attrib_index_vert_color = 1;
    ::e2::interface::i_rendernode_bind_attrib n_bind_attrib_vert_color;
    n_bind_attrib_vert_color._program_handle = &program_handle;
    n_bind_attrib_vert_color._location_type = ::e2::interface::e_renderresource_subtype_attrib_attrib_location;
    n_bind_attrib_vert_color._index = &attrib_index_vert_color;
    n_bind_attrib_vert_color._var_name = attrib_name_vert_color;

    //bind frag location
    char * attrib_name_frag_color = "FragColor";
    uint64_t attrib_index_frag_color = 0;
    ::e2::interface::i_rendernode_bind_attrib n_bind_attrib_frag_color;
    n_bind_attrib_frag_color._program_handle = &program_handle;
    n_bind_attrib_frag_color._location_type = ::e2::interface::e_renderresource_subtype_attrib_frag_location;
    n_bind_attrib_frag_color._index = &attrib_index_frag_color;
    n_bind_attrib_frag_color._var_name = attrib_name_frag_color;
    
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
    ::e2::interface::i_rendernode_init_buffer n_init_vbos;
    n_init_vbos._num_buffers = &num_buf;
    n_init_vbos._buffers = vbos;

    ::e2::interface::i_rendernode_deinit_buffer n_deinit_vbos;
    n_deinit_vbos._num_buffers = &num_buf;
    n_deinit_vbos._buffers = vbos;

    //vbos data population : vert pos
    uint64_t buf_type_array = ::e2::interface::e_renderresource_buffer_array_buffer;
    ::e2::interface::i_rendernode_bind_buffer n_bind_buffer_pos;
    n_bind_buffer_pos._buffer_type = &buf_type_array;
    n_bind_buffer_pos._buffer_handle = &vbos[0];

    ::e2::interface::i_rendernode_bind_buffer n_bind_buffer_pos_2 = n_bind_buffer_pos;

    uint64_t buffer_size_pos = sizeof( positionData );
    uint64_t buffer_usage = GL_STATIC_DRAW;
    ::e2::interface::i_rendernode_store_buffer n_store_buffer_pos;
    n_store_buffer_pos._buffer_type = &buf_type_array;
    n_store_buffer_pos._buffer_size = &buffer_size_pos;
    n_store_buffer_pos._buffer_data = positionData;
    n_store_buffer_pos._buffer_usage = &buffer_usage;

    ::e2::interface::i_rendernode_bind_buffer n_bind_buffer_color;
    n_bind_buffer_color._buffer_type = &buf_type_array;
    n_bind_buffer_color._buffer_handle = &vbos[1];

    ::e2::interface::i_rendernode_bind_buffer n_bind_buffer_color_2 = n_bind_buffer_color;

    uint64_t buffer_size_color = sizeof( colorData );
    ::e2::interface::i_rendernode_store_buffer n_store_buffer_color;
    n_store_buffer_color._buffer_type = &buf_type_array;
    n_store_buffer_color._buffer_size = &buffer_size_color;
    n_store_buffer_color._buffer_data = colorData;
    n_store_buffer_color._buffer_usage = &buffer_usage;

    //vertex array object creation
    uint64_t num_va = 1;
    uint64_t vao;
    uint64_t obj_type_va = ::e2::interface::e_renderresource_subtype_object_vertex_array;
    ::e2::interface::i_rendernode_init_object n_init_object_va;
    n_init_object_va._object_type = obj_type_va;
    n_init_object_va._num_objects = &num_va;
    n_init_object_va._handle_objects = &vao;

    //vao bind
    ::e2::interface::i_rendernode_bind_object n_bind_object_va;
    n_bind_object_va._object_type = obj_type_va;
    n_bind_object_va._object_handle = &vao;

    ::e2::interface::i_rendernode_bind_object n_bind_object_va_2 = n_bind_object_va;

    //vertex attribute arrays enable
    uint64_t attrib_index_pos = 0;
    ::e2::interface::i_rendernode_enable_attrib n_enable_vaa_pos;
    n_enable_vaa_pos._attrib_type = obj_type_va;
    n_enable_vaa_pos._attrib_handle = &attrib_index_pos;

    uint64_t attrib_index_color = 1;
    ::e2::interface::i_rendernode_enable_attrib n_enable_vaa_color;
    n_enable_vaa_color._attrib_type = obj_type_va;
    n_enable_vaa_color._attrib_handle = &attrib_index_color;

    //map data buffers to corresponding shader input variables
    uint64_t attrib_size = 3;
    bool attrib_normalized = false;
    uint64_t attrib_stride = 0;
    void * attrib_pointer = 0;
    uint64_t attrib_data_type = ::e2::interface::e_renderresource_data_type_float;
    ::e2::interface::i_rendernode_store_defineformat_vertexattrib n_defineformat_attrib_vert_pos;
    n_defineformat_attrib_vert_pos._attrib_handle = &attrib_index_vert_pos;
    n_defineformat_attrib_vert_pos._attrib_size = &attrib_size;
    n_defineformat_attrib_vert_pos._attrib_data_type = &attrib_data_type;
    n_defineformat_attrib_vert_pos._attrib_normalized = &attrib_normalized;
    n_defineformat_attrib_vert_pos._attrib_stride = &attrib_stride;
    n_defineformat_attrib_vert_pos._attrib_pointer = attrib_pointer;

    ::e2::interface::i_rendernode_store_defineformat_vertexattrib n_defineformat_attrib_vert_color;
    n_defineformat_attrib_vert_color._attrib_handle = &attrib_index_vert_color;
    n_defineformat_attrib_vert_color._attrib_size = &attrib_size;
    n_defineformat_attrib_vert_color._attrib_data_type = &attrib_data_type;
    n_defineformat_attrib_vert_color._attrib_normalized = &attrib_normalized;
    n_defineformat_attrib_vert_color._attrib_stride = &attrib_stride;
    n_defineformat_attrib_vert_color._attrib_pointer = attrib_pointer;

    uint64_t primitive_type = ::e2::interface::e_renderresource_primitive_triangles;
    int64_t offset = 0;
    uint64_t count = 3;
    ::e2::interface::i_rendernode_exec_drawbatch n_exec_drawbatch;
    n_exec_drawbatch._primitive_type = &primitive_type;
    n_exec_drawbatch._offset = &offset;
    n_exec_drawbatch._count = &count;

    ::e2::interface::i_rendernode_clear_window_buffer_colour n_clear_wind_buf_color;
    ::e2::interface::i_rendernode_clear_window_buffer_depth n_clear_wind_bud_depth;
    ::e2::interface::i_rendernode_disable_window_buffer_depth n_disable_win_buf_depth;

    ::e2::interface::i_rendernode_compute_program n_link_program;
    n_link_program._program_handle = &program_handle;

    ::e2::interface::i_rendernode_query_attrib n_query_active_attrib;
    n_query_active_attrib._program_handle = &program_handle;

    ::e2::interface::i_rendernode_query_persistent n_query_active_persistent;
    n_query_active_persistent._program_handle = &program_handle;
	
    //setup for shaders and program
    gb_init.set_handle( &n_init_win );
    nodes_init.push_back( &n_init_win );

    gb_init.set_handle( &n_init_prog );
    gb_init.link_prerequisite( &n_init_prog, &n_init_win );
    nodes_init.push_back( &n_init_prog );

    gb_init.set_handle( &n_load_shader_vert );
    gb_init.link_prerequisite( &n_load_shader_vert, &n_init_prog );
    nodes_init.push_back( &n_load_shader_vert );
    
    gb_init.set_handle( &n_load_shader_frag );
    gb_init.link_prerequisite( &n_load_shader_frag, &n_init_prog );
    nodes_init.push_back( &n_load_shader_frag );

    gb_init.set_handle( &n_bind_shader_vert );
    gb_init.link_prerequisite( &n_bind_shader_vert, &n_load_shader_vert );
    nodes_init.push_back( &n_bind_shader_vert );

    gb_init.set_handle( &n_bind_shader_frag );
    gb_init.link_prerequisite( &n_bind_shader_frag, &n_load_shader_frag );
    nodes_init.push_back( &n_bind_shader_frag );
    
    //setup shader variable pipeline mapping
    gb_init.set_handle( &n_bind_attrib_vert_pos );
    gb_init.link_prerequisite( &n_bind_attrib_vert_pos, &n_bind_shader_frag );
    nodes_init.push_back( &n_bind_attrib_vert_pos );

    gb_init.set_handle( &n_bind_attrib_vert_color );
    gb_init.link_prerequisite( &n_bind_attrib_vert_color, &n_bind_attrib_vert_pos );
    nodes_init.push_back( &n_bind_attrib_vert_color );

    gb_init.set_handle( &n_bind_attrib_frag_color );
    gb_init.link_prerequisite( &n_bind_attrib_frag_color, &n_bind_attrib_vert_color );
    nodes_init.push_back( &n_bind_attrib_frag_color );

    //compile opengl program
    gb_init.set_handle( &n_link_program );
    gb_init.link_prerequisite( &n_link_program, &n_bind_attrib_frag_color );
    nodes_init.push_back( &n_link_program );

    gb_init.set_handle( &n_bind_program );
    gb_init.link_prerequisite( &n_bind_program, &n_link_program );
    nodes_init.push_back( &n_bind_program );

    gb_init.set_handle( &n_query_active_attrib );
    gb_init.link_prerequisite( &n_query_active_attrib, &n_bind_program );
    nodes_init.push_back( &n_query_active_attrib );

    gb_init.set_handle( &n_query_active_persistent );
    gb_init.link_prerequisite( &n_query_active_persistent, &n_query_active_attrib );
    nodes_init.push_back( &n_query_active_persistent );
    
    //store shader input data to buffers
    gb_init.set_handle( &n_init_vbos );
    gb_init.link_prerequisite( &n_init_vbos, &n_query_active_persistent );
    nodes_init.push_back( &n_init_vbos );

    gb_init.set_handle( &n_bind_buffer_pos );
    gb_init.link_prerequisite( &n_bind_buffer_pos, &n_init_vbos );
    nodes_init.push_back( &n_bind_buffer_pos );

    gb_init.set_handle( &n_store_buffer_pos );
    gb_init.link_prerequisite( &n_store_buffer_pos, &n_bind_buffer_pos );
    nodes_init.push_back( &n_store_buffer_pos );

    gb_init.set_handle( &n_bind_buffer_color );
    gb_init.link_prerequisite( &n_bind_buffer_color, &n_store_buffer_pos );
    nodes_init.push_back( &n_bind_buffer_color );

    gb_init.set_handle( &n_store_buffer_color );
    gb_init.link_prerequisite( &n_store_buffer_color, &n_bind_buffer_color );
    nodes_init.push_back( &n_store_buffer_color );

    gb_init.set_handle( &n_init_object_va );
    gb_init.link_prerequisite( &n_init_object_va, &n_store_buffer_color );
    nodes_init.push_back( &n_init_object_va );

    gb_init.set_handle( &n_bind_object_va );
    gb_init.link_prerequisite( &n_bind_object_va, &n_init_object_va );
    nodes_init.push_back( &n_bind_object_va );

    //map vertex array index for position buffer
    gb_init.set_handle( &n_bind_buffer_pos_2 );
    gb_init.link_prerequisite( &n_bind_buffer_pos_2, &n_bind_object_va );
    nodes_init.push_back( &n_bind_buffer_pos_2 );

    gb_init.set_handle( &n_enable_vaa_pos );
    gb_init.link_prerequisite( &n_enable_vaa_pos, &n_bind_buffer_pos_2 );
    nodes_init.push_back( &n_enable_vaa_pos );

    gb_init.set_handle( &n_defineformat_attrib_vert_pos );
    gb_init.link_prerequisite( &n_defineformat_attrib_vert_pos, &n_enable_vaa_pos );
    nodes_init.push_back( &n_defineformat_attrib_vert_pos );

    //map vertex array index for colour buffer
    gb_init.set_handle( &n_bind_buffer_color_2 );
    gb_init.link_prerequisite( &n_bind_buffer_color_2, &n_defineformat_attrib_vert_pos );
    nodes_init.push_back( &n_bind_buffer_color_2 );

    gb_init.set_handle( &n_enable_vaa_color );
    gb_init.link_prerequisite( &n_enable_vaa_color, &n_bind_buffer_color_2 );
    nodes_init.push_back( &n_enable_vaa_color );

    gb_init.set_handle( &n_defineformat_attrib_vert_color );
    gb_init.link_prerequisite( &n_defineformat_attrib_vert_color, &n_enable_vaa_color );
    nodes_init.push_back( &n_defineformat_attrib_vert_color );

    //drawing loop
    gb_draw_loop.set_handle( &n_clear_wind_buf_color);
    nodes_draw_loop.push_back( &n_clear_wind_buf_color );

    gb_draw_loop.set_handle( &n_disable_win_buf_depth );
    gb_draw_loop.link_prerequisite( &n_disable_win_buf_depth, &n_clear_wind_buf_color );
    nodes_draw_loop.push_back( &n_disable_win_buf_depth );

    gb_draw_loop.set_handle( &n_bind_program_2 );
    gb_draw_loop.link_prerequisite( &n_bind_program_2, &n_disable_win_buf_depth );
    nodes_draw_loop.push_back( &n_bind_program_2 );

    gb_draw_loop.set_handle( &n_bind_object_va_2 );
    gb_draw_loop.link_prerequisite( &n_bind_object_va_2, &n_bind_program_2 );
    nodes_draw_loop.push_back( &n_bind_object_va_2 );

    gb_draw_loop.set_handle( &n_exec_drawbatch );
    gb_draw_loop.link_prerequisite( &n_exec_drawbatch, &n_bind_object_va_2 );
    nodes_draw_loop.push_back( &n_exec_drawbatch );

    gb_draw_loop.set_handle( &n_swap_win );
    gb_draw_loop.link_prerequisite( &n_swap_win, &n_exec_drawbatch );
    nodes_draw_loop.push_back( &n_swap_win );

    //deinit sequence
    gb_deinit.set_handle( &n_deinit_program );
    nodes_deinit.push_back( &n_deinit_program );

    gb_deinit.set_handle( &n_deinit_vbos );
    gb_draw_loop.link_prerequisite( &n_deinit_vbos, &n_deinit_program );
    nodes_deinit.push_back( &n_deinit_vbos );

    gb_deinit.set_handle( &n_deinit_window );
    gb_draw_loop.link_prerequisite( &n_deinit_window, &n_deinit_program );
    nodes_deinit.push_back( &n_deinit_window );

    //create render graphs
    //todo: implement flow control and automatic bind dependencies into render graph
    std::list< ::e2::interface::i_rendernode * > rg_init;
    std::list< ::e2::interface::i_rendernode * > rg_draw;
    std::list< ::e2::interface::i_rendernode * > rg_deinit;

    ::e2::render::rendergraphscheduler0 sch;
    assert( sch.schedule_graph( &nodes_init, &rg_init ) );
    assert( sch.schedule_graph( &nodes_draw_loop, &rg_draw ) );
    assert( sch.schedule_graph( &nodes_deinit, &rg_deinit ) );

    assert( false == rg_init.empty() );
    assert( false == rg_draw.empty() );
    assert( false == rg_deinit.empty() );

    //convert graph nodes into sequences of render task packages
    std::list< ::e2::interface::i_renderpackage * > renderpackages_init;
    std::list< ::e2::interface::i_renderpackage * > renderpackages_draw;
    std::list< ::e2::interface::i_renderpackage * > renderpackages_deinit;
    converter.process( &buf, &rg_init, &renderpackages_init );
    converter.process( &buf, &rg_draw, &renderpackages_draw );
    converter.process( &buf, &rg_deinit, &renderpackages_deinit );

    //start actual sequence
    executer.process( &rd, &renderpackages_init );
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
	executer.process( &rd, &renderpackages_draw );
	glfwPollEvents();
	std::this_thread::sleep_for( std::chrono::milliseconds(25) );
    }
    executer.process( &rd, &renderpackages_deinit );
	
    double frac_free;
    buf.buffer_stat_fraction_free( &frac_free );
    std::cout << "buffer fraction free: " << frac_free << std::endl;
    return 0;
}
