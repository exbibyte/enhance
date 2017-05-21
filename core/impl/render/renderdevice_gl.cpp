#include <iostream>
#include <cstdio>
#include <cassert>
#include <cstring>
#include <map>

#include "gl_includes.hpp"
#include "i_renderpackage.hpp"
#include "i_rendercmd.hpp"
#include "renderdevice_gl.hpp"
#include "renderdevice_dispatch.hpp"
#include "rendermap_gl.hpp"
#include "renderpackage_gl.hpp"
#include "gl_helper.hpp"

namespace e2 { namespace render {

renderdevice_gl_impl::renderdevice_gl_impl(){

    _dispatch._dispatch_map[ renderdispatch_key( ::e2::interface::e_rendercmd_type::init,
						 ::e2::interface::e_renderresource_type::windowing ) ] = &process_init_window;

    _dispatch._dispatch_map[ renderdispatch_key( ::e2::interface::e_rendercmd_type::deinit,
						 ::e2::interface::e_renderresource_type::windowing ) ] = &process_deinit_window;

    _dispatch._dispatch_map[ renderdispatch_key( ::e2::interface::e_rendercmd_type::exec,
						 ::e2::interface::e_renderresource_type::windowing  ) ] = &process_exec_window;

    _dispatch._dispatch_map[ renderdispatch_key( ::e2::interface::e_rendercmd_type::store,
						 ::e2::interface::e_renderresource_type::defineformat ) ] = &process_store_defineformat;
	
    _dispatch._dispatch_map[ renderdispatch_key( ::e2::interface::e_rendercmd_type::bind,
						 ::e2::interface::e_renderresource_type::object ) ] = &process_bind_object;

    _dispatch._dispatch_map[ renderdispatch_key( ::e2::interface::e_rendercmd_type::bind,
						 ::e2::interface::e_renderresource_type::buffer ) ] = &process_bind_buffer;

    _dispatch._dispatch_map[ renderdispatch_key( ::e2::interface::e_rendercmd_type::exec,
						 ::e2::interface::e_renderresource_type::drawbatch ) ] = &process_exec_drawbatch;

    _dispatch._dispatch_map[ renderdispatch_key( ::e2::interface::e_rendercmd_type::enable,
						 ::e2::interface::e_renderresource_type::attrib ) ] = &process_enable_attrib;

    _dispatch._dispatch_map[ renderdispatch_key( ::e2::interface::e_rendercmd_type::disable,
						 ::e2::interface::e_renderresource_type::attrib ) ] = &process_disable_attrib;

    _dispatch._dispatch_map[ renderdispatch_key( ::e2::interface::e_rendercmd_type::init,
						 ::e2::interface::e_renderresource_type::object ) ] = &process_init_object;

    _dispatch._dispatch_map[ renderdispatch_key( ::e2::interface::e_rendercmd_type::deinit,
						 ::e2::interface::e_renderresource_type::object ) ] = &process_deinit_object;

    _dispatch._dispatch_map[ renderdispatch_key( ::e2::interface::e_rendercmd_type::store,
						 ::e2::interface::e_renderresource_type::buffer ) ] = &process_store_buffer;

    _dispatch._dispatch_map[ renderdispatch_key( ::e2::interface::e_rendercmd_type::init,
						 ::e2::interface::e_renderresource_type::buffer ) ] = &process_init_buffer;

    _dispatch._dispatch_map[ renderdispatch_key( ::e2::interface::e_rendercmd_type::deinit,
						 ::e2::interface::e_renderresource_type::buffer ) ] = &process_deinit_buffer;

    _dispatch._dispatch_map[ renderdispatch_key( ::e2::interface::e_rendercmd_type::bind,
						 ::e2::interface::e_renderresource_type::attrib ) ] = &process_bind_attrib;

    _dispatch._dispatch_map[ renderdispatch_key( ::e2::interface::e_rendercmd_type::init,
						 ::e2::interface::e_renderresource_type::program ) ] = &process_init_program;

    _dispatch._dispatch_map[ renderdispatch_key( ::e2::interface::e_rendercmd_type::deinit,
						 ::e2::interface::e_renderresource_type::program ) ] = &process_deinit_program;

    _dispatch._dispatch_map[ renderdispatch_key( ::e2::interface::e_rendercmd_type::compute,
						 ::e2::interface::e_renderresource_type::program ) ] = &process_compute_program;

    _dispatch._dispatch_map[ renderdispatch_key( ::e2::interface::e_rendercmd_type::bind,
						 ::e2::interface::e_renderresource_type::program ) ] = &process_bind_program;

    _dispatch._dispatch_map[ renderdispatch_key( ::e2::interface::e_rendercmd_type::store,
						 ::e2::interface::e_renderresource_type::persistent ) ] = &process_store_persistent;
}

bool renderdevice_gl_impl::renderdevice_process( ::e2::interface::i_renderpackage p ){
    return _dispatch.dispatch_process( this, p );
}

bool renderdevice_gl_impl::process_init_window_size( renderdevice_gl_impl * context, ::e2::interface::i_renderpackage p ){
#ifdef debug_info
    std::cout << "process init window size" << std::endl;
#endif
    return true;
}

bool renderdevice_gl_impl::process_init_window( renderdevice_gl_impl * context, ::e2::interface::i_renderpackage p ){
#ifdef debug_info
    std::cout << "process init window" << std::endl;
#endif
    void * payload [256];
    if( false == glfwInit() ){
	assert( false && "glfw init failed." );
	return false;
    }
    {
	if( nullptr == p._payload ){
	    assert( false && "init window dim invalid." );
	    return false;
	}
	if( nullptr == p._payload->_buf ){
	    assert( false && "buffer invalid." );
	    return false;
	}
	if( false == renderpackage_gl::unpack( &p, payload ) )
	    return false;
	uint64_t * n_data = ( uint64_t * ) payload[0];
	assert( n_data );
	assert( 2 == *n_data );
	uint64_t * wind_dim = ( uint64_t * ) payload[1];
	assert( wind_dim );
	int dimx = (int)*wind_dim;
	++wind_dim;
	int dimy = (int)*wind_dim;
	glfwWindowHint( GLFW_CONTEXT_VERSION_MAJOR, 3 );
	glfwWindowHint( GLFW_CONTEXT_VERSION_MINOR, 3 );
	glfwWindowHint( GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE );
	context->_window = glfwCreateWindow( dimx, dimy, "renderdevice_gl window", nullptr, nullptr );
	if( false == context->_window ){
	    assert( false && "window creation failed." );
	    return false;
	}

	glfwMakeContextCurrent( context->_window );

	if( 0 != gl3wInit() ){
	    assert( false && "gl3w initialization failed." );
	    return false;
	}
	glEnable( GL_DEPTH_TEST );
	glClearColor( 50, 50, 50, 1.0 );

	const GLubyte * renderer = glGetString( GL_RENDERER );
	const GLubyte * vendor = glGetString( GL_VENDOR );
	const GLubyte * version = glGetString( GL_VERSION );
	const GLubyte * glslversion = glGetString( GL_SHADING_LANGUAGE_VERSION );
	GLint major, minor;
	glGetIntegerv(GL_MAJOR_VERSION, &major);
	glGetIntegerv(GL_MINOR_VERSION, &minor);
	printf("gl vendor: %s\n", vendor);
	printf("gl renderer : %s\n", renderer);
	printf("gl version (string) : %s\n", version);
	printf("gl version (integer) : %d.%d\n", major, minor);
	printf("glsl version : %s\n", glslversion);
    }
    return true;
}

bool renderdevice_gl_impl::process_deinit_window( renderdevice_gl_impl * context, ::e2::interface::i_renderpackage p ){
#ifdef debug_info
    std::cout << "process deinit window" << std::endl;
#endif
    if( nullptr != context->_window ){
	glfwDestroyWindow( context->_window );
	context->_window = nullptr;
    }
    glfwTerminate();
    return true;
}

bool renderdevice_gl_impl::process_exec_window( renderdevice_gl_impl * context, ::e2::interface::i_renderpackage p ){
    switch( p._resource->_id ){
    case ::e2::interface::e_renderresource_window::window_buf_swap:
    {
	assert( context->_window );
#ifdef debug_info
	std::cout << "process window buf swap" << std::endl;
#endif
	glfwMakeContextCurrent( context->_window );
	glfwSwapBuffers( context->_window );
    }
    break;
    default:
    {
	return false;
    }
    }
    return true;
}

bool renderdevice_gl_impl::process_exec_drawbatch( renderdevice_gl_impl * context, ::e2::interface::i_renderpackage p ){
    void * payload [256];
    GLenum gl_primitive_type;
    GLint gl_first;
    GLsizei gl_count;
    if( false == renderpackage_gl::unpack( &p, payload ) )
	return false;
    uint64_t * n_data = ( uint64_t * )payload[0];
    uint64_t * primitive_type = ( uint64_t * )payload[1];
    uint64_t * offset = ( uint64_t * )payload[2];
    uint64_t * count = ( uint64_t * )payload[3];
    assert( n_data );
    assert( 3 == *n_data );
    assert( primitive_type );
    assert( offset );
    assert( count );
    {
	auto it = rendermap_gl::_map_render_primitives.find( *primitive_type );
	if( rendermap_gl::_map_render_primitives.end() == it ){
	    assert( false && "map render primitive invalid." );
	    return false;
	}
	gl_primitive_type = it->second;
    }
    gl_first = static_cast< GLint >( *offset );
    gl_count = static_cast< GLsizei >( *count );
    glDrawArrays( gl_primitive_type, gl_first, gl_count );
    return ::e2::render::gl::gl_helper::check_error();
}

bool renderdevice_gl_impl::process_store_defineformat( renderdevice_gl_impl * context, ::e2::interface::i_renderpackage p ){
    void * payload [256];
    if( false == renderpackage_gl::unpack( &p, payload ) )
	return false;
    bool ret = false;
    if( ::e2::interface::renderresource_defineformat_vertex_attrib_data == p._resource->_id ){
	uint64_t * n_data = ( uint64_t * )payload[0];
	uint64_t * va_index = ( uint64_t * )payload[1];
	uint64_t * va_size = ( uint64_t * )payload[2];
	uint64_t * va_type = ( uint64_t * )payload[3];
	uint64_t * va_normalized = ( uint64_t * )payload[4];
	uint64_t * va_stride = ( uint64_t * )payload[5];
	uint64_t * va_pointer = ( uint64_t * )payload[6];
	assert( n_data );
	assert( va_index );
	assert( va_size );
	assert( va_type );
	assert( va_normalized );
	assert( va_stride );
	assert( va_pointer );
	assert( 6 == *n_data );
	GLuint gl_va_index = ( GLuint ) *va_index;
	GLint gl_va_size = ( GLint ) *va_size;
	GLenum gl_va_type = ( GLenum ) *va_type;
	GLboolean gl_va_normalized = ( GLboolean ) *va_normalized;
	GLsizei gl_va_stride = ( GLsizei ) *va_stride;
	GLvoid * gl_va_pointer = ( GLvoid * ) *va_pointer;
	ret = ::e2::render::gl::gl_helper::define_vertex_attrib_data( gl_va_index, gl_va_size, gl_va_type, gl_va_normalized, gl_va_stride, gl_va_pointer );
    }
    return ret;
}

bool renderdevice_gl_impl::process_bind_object( renderdevice_gl_impl * context, ::e2::interface::i_renderpackage p ){
    void * payload [256];
    if( false == renderpackage_gl::unpack( &p, payload ) )
	return false;
    bool ret = false;
    if( ::e2::interface::renderresource_object_vertex_array == p._resource->_id ){
	uint64_t * n_data = ( uint64_t * )payload[0];
	uint64_t * vertex_array_handle = ( uint64_t * )payload[1];
	assert( n_data );
	assert( 1 == *n_data );
	assert( vertex_array_handle );
	GLuint gl_vertex_array_handle = ( GLuint ) *vertex_array_handle;
	ret = ::e2::render::gl::gl_helper::bind_vertex_array( gl_vertex_array_handle );
    }
    return ret;
}

bool renderdevice_gl_impl::process_bind_buffer( renderdevice_gl_impl * context, ::e2::interface::i_renderpackage p ){
    void * payload [256];
    if( false == renderpackage_gl::unpack( &p, payload ) )
	return false;
    auto it = rendermap_gl::_map_render_bind_buffer.find( p._resource->_id );
    if( rendermap_gl::_map_render_bind_buffer.end() == it ){
	assert( false && "bind buffer type invalid." );
	return false;
    }
    uint64_t * n_data = ( uint64_t * )payload[0];
    uint64_t * buffer_handle = ( uint64_t * )payload[1];
    assert( n_data );
    assert( buffer_handle );
    assert( 1 == *n_data );
    GLenum gl_buffer_type = it->second;
    GLuint gl_buffer_handle = ( GLuint ) *buffer_handle;
    return ::e2::render::gl::gl_helper::bind_buffer( gl_buffer_type, gl_buffer_handle );
}

bool renderdevice_gl_impl::process_enable_attrib( renderdevice_gl_impl * context, ::e2::interface::i_renderpackage p ){
    void * payload [256];
    if( false == renderpackage_gl::unpack( &p, payload ) )
	return false;
    bool ret = false;
    if( ::e2::interface::renderresource_enable_attrib_vertex_attrib_array == p._resource->_id ){
	uint64_t * n_data = ( uint64_t * )payload[0];
	uint64_t * va_index = ( uint64_t * )payload[1];
	assert( n_data );
	assert( 1 == *n_data );
	assert( va_index );
	GLuint gl_va_index = ( GLuint ) *va_index;
	ret = ::e2::render::gl::gl_helper::enable_vertex_attrib_array( gl_va_index );
    }
    return ret;
}

bool renderdevice_gl_impl::process_disable_attrib( renderdevice_gl_impl * context, ::e2::interface::i_renderpackage p ){
    void * payload [256];
    if( false == renderpackage_gl::unpack( &p, payload ) )
	return false;
    bool ret = false;
    if( ::e2::interface::renderresource_disable_attrib_vertex_attrib_array == p._resource->_id ){
	uint64_t * n_data = ( uint64_t * )payload[0];
	uint64_t * va_index = ( uint64_t * )payload[1];
	assert( n_data );
	assert( 1 == *n_data );
	assert( va_index );
	GLuint gl_va_index = ( GLuint ) *va_index;
	ret = ::e2::render::gl::gl_helper::disable_vertex_attrib_array( gl_va_index );
    }
    return ret;
}

bool renderdevice_gl_impl::process_init_object( renderdevice_gl_impl * context, ::e2::interface::i_renderpackage p ){
    void * payload [256];
    if( false == renderpackage_gl::unpack( &p, payload ) )
	return false;
    bool ret = false;
    if( ::e2::interface::renderresource_object_vertex_array == p._resource->_id ){
	uint64_t * n_data = ( uint64_t * )payload[0];
	uint64_t * size_va_names = ( uint64_t * )payload[1];
	uint64_t * va_array = ( uint64_t * )payload[2];
	assert( n_data );
	assert( 2 == *n_data );
	assert( size_va_names );
	assert( va_array );
	GLsizei gl_size_va_names = ( GLsizei ) *size_va_names;
        GLuint * gl_va_array = ( GLuint * ) va_array;
	ret = ::e2::render::gl::gl_helper::generate_vertex_arrays( gl_size_va_names, gl_va_array );
    }
    return ret;
}

bool renderdevice_gl_impl::process_deinit_object( renderdevice_gl_impl * context, ::e2::interface::i_renderpackage p ){
    void * payload [256];
    if( false == renderpackage_gl::unpack( &p, payload ) )
	return false;
    bool ret = false;
    if( ::e2::interface::renderresource_object_vertex_array == p._resource->_id ){
	uint64_t * n_data = ( uint64_t * )payload[0];
	uint64_t * size_va_names = ( uint64_t * )payload[1];
	uint64_t * va_array = ( uint64_t * )payload[2];
	assert( n_data );
	assert( 2 == *n_data );
	assert( size_va_names );
	assert( va_array );
	GLsizei gl_size_va_names = ( GLsizei ) *size_va_names;
        GLuint * gl_va_array = ( GLuint * ) va_array;
	ret = ::e2::render::gl::gl_helper::delete_vertex_arrays( gl_size_va_names, gl_va_array );
    }
    return ret;
}

bool renderdevice_gl_impl::process_store_buffer( renderdevice_gl_impl * context, ::e2::interface::i_renderpackage p ){
    void * payload [256];
    if( false == renderpackage_gl::unpack( &p, payload ) )
	return false;
    auto it = rendermap_gl::_map_render_bind_buffer.find( p._resource->_id );
    if( rendermap_gl::_map_render_bind_buffer.end() == it ){
	assert( false && "bind buffer type invalid." );
	return false;
    }
    uint64_t * n_data = ( uint64_t * )payload[0];
    uint64_t * bytes_size = ( uint64_t * )payload[1];
    uint64_t * data = ( uint64_t * )payload[2];
    uint64_t * usage = ( uint64_t * )payload[3];
    assert( n_data );
    assert( bytes_size );
    assert( data );
    assert( usage );
    assert( 3 == *n_data );
    GLenum gl_buffer_type = it->second;
    GLsizeiptr gl_bytes_size = ( GLsizeiptr ) *bytes_size;
    GLvoid * gl_data = ( GLvoid * ) data;
    GLenum gl_usage = ( GLenum ) *usage;
    return ::e2::render::gl::gl_helper::store_buffer_data( gl_buffer_type, gl_bytes_size, gl_data, gl_usage );
}

bool renderdevice_gl_impl::process_init_buffer( renderdevice_gl_impl * context, ::e2::interface::i_renderpackage p ){
    void * payload [256];
    if( false == renderpackage_gl::unpack( &p, payload ) )
	return false;
    bool ret = false;
    uint64_t * n_data = ( uint64_t * )payload[0];
    uint64_t * num_buffers = ( uint64_t * )payload[1];
    uint64_t * buffers = ( uint64_t * )payload[2];
    assert( n_data );
    assert( 2 == *n_data );
    assert( num_buffers );
    assert( buffers );
    GLsizei gl_num_buffers = ( GLsizei ) *num_buffers;
    GLuint * gl_buffers = ( GLuint * ) buffers;
    ret = ::e2::render::gl::gl_helper::generate_buffers( gl_num_buffers, gl_buffers );
    return ret;
}

bool renderdevice_gl_impl::process_deinit_buffer( renderdevice_gl_impl * context, ::e2::interface::i_renderpackage p ){
    void * payload [256];
    if( false == renderpackage_gl::unpack( &p, payload ) )
	return false;
    bool ret = false;
    uint64_t * n_data = ( uint64_t * )payload[0];
    uint64_t * num_buffers = ( uint64_t * )payload[1];
    uint64_t * buffers = ( uint64_t * )payload[2];
    assert( n_data );
    assert( 2 == *n_data );
    assert( num_buffers );
    assert( buffers );
    GLsizei gl_num_buffers = ( GLsizei ) *num_buffers;
    GLuint * gl_buffers = ( GLuint * ) buffers;
    ret = ::e2::render::gl::gl_helper::delete_buffers( gl_num_buffers, gl_buffers );
    return ret;
}

bool renderdevice_gl_impl::process_bind_attrib( renderdevice_gl_impl * context, ::e2::interface::i_renderpackage p ){
    void * payload [256];
    if( false == renderpackage_gl::unpack( &p, payload ) )
	return false;
    bool ret = false;
    if( ::e2::interface::renderresource_attrib_attrib_location == p._resource->_id ){
	uint64_t * n_data = ( uint64_t * )payload[0];
	uint64_t * program_handle = ( uint64_t * )payload[1];
	uint64_t * attrib_index = ( uint64_t * )payload[2];
	uint64_t * attrib_name = ( uint64_t * )payload[3];
	assert( n_data );
	assert( 3 == *n_data );
	assert( program_handle );
	assert( attrib_index );
	assert( attrib_name );
	GLuint gl_program_handle = ( GLuint ) *program_handle;
	GLuint gl_attrib_index = ( GLuint ) *attrib_index;
        GLchar * gl_attrib_name = ( GLchar * ) attrib_name;
	ret = ::e2::render::gl::gl_helper::bind_attrib_location( gl_program_handle, gl_attrib_index, gl_attrib_name );
    }else if( ::e2::interface::renderresource_attrib_frag == p._resource->_id ){
	uint64_t * n_data = ( uint64_t * )payload[0];
	uint64_t * program_handle = ( uint64_t * )payload[1];
	uint64_t * attrib_index = ( uint64_t * )payload[2];
	uint64_t * attrib_name = ( uint64_t * )payload[3];
	assert( n_data );
	assert( 3 == *n_data );
	assert( program_handle );
	assert( attrib_index );
	assert( attrib_name );
	GLuint gl_program_handle = ( GLuint ) *program_handle;
	GLuint gl_color_num = ( GLuint ) *attrib_index;
        char * gl_varying_name = ( char * ) attrib_name;
	ret = ::e2::render::gl::gl_helper::bind_frag_data_location( gl_program_handle, gl_color_num, gl_varying_name );
    }
    return ret;
}

bool renderdevice_gl_impl::process_init_program( renderdevice_gl_impl * context, ::e2::interface::i_renderpackage p ){
    void * payload [256];
    if( false == renderpackage_gl::unpack( &p, payload ) )
	return false;
    uint64_t * n_data = ( uint64_t * )payload[0];
    uint64_t * program_handle = ( uint64_t * )payload[1];
    assert( n_data );
    assert( 1 == *n_data );
    assert( program_handle );
    GLuint * gl_program_handle = ( GLuint * ) program_handle;    
    return ::e2::render::gl::gl_helper::create_program( gl_program_handle );
}

bool renderdevice_gl_impl::process_deinit_program( renderdevice_gl_impl * context, ::e2::interface::i_renderpackage p ){
    void * payload [256];
    if( false == renderpackage_gl::unpack( &p, payload ) )
	return false;
    uint64_t * n_data = ( uint64_t * )payload[0];
    uint64_t * program_handle = ( uint64_t * )payload[1];
    assert( n_data );
    assert( 1 == *n_data );
    assert( program_handle );
    GLuint * gl_program_handle = ( GLuint * ) program_handle;
    return ::e2::render::gl::gl_helper::delete_program( *gl_program_handle );
}

bool renderdevice_gl_impl::process_bind_shader( renderdevice_gl_impl * context, ::e2::interface::i_renderpackage p ){
    void * payload [256];
    if( false == renderpackage_gl::unpack( &p, payload ) )
	return false;
    uint64_t * n_data = ( uint64_t * )payload[0];
    uint64_t * program_handle = ( uint64_t * )payload[1];
    uint64_t * shader_handle = ( uint64_t * )payload[2];
    assert( n_data );
    assert( 2 == *n_data );
    assert( program_handle );
    assert( shader_handle );
    GLuint * gl_program_handle = ( GLuint * ) program_handle;
    GLuint * gl_shader_handle = ( GLuint * ) shader_handle;
    return ::e2::render::gl::gl_helper::attach_shader( *gl_program_handle, *gl_shader_handle );
}

bool renderdevice_gl_impl::process_unbind_shader( renderdevice_gl_impl * context, ::e2::interface::i_renderpackage p ){
    void * payload [256];
    if( false == renderpackage_gl::unpack( &p, payload ) )
	return false;
    uint64_t * n_data = ( uint64_t * )payload[0];
    uint64_t * program_handle = ( uint64_t * )payload[1];
    uint64_t * shader_handle = ( uint64_t * )payload[2];
    assert( n_data );
    assert( 2 == *n_data );
    assert( program_handle );
    assert( shader_handle );
    GLuint * gl_program_handle = ( GLuint * ) program_handle;
    GLuint * gl_shader_handle = ( GLuint * ) shader_handle;
    return ::e2::render::gl::gl_helper::detach_shader( *gl_program_handle, *gl_shader_handle );
}

bool renderdevice_gl_impl::process_compute_program( renderdevice_gl_impl * context, ::e2::interface::i_renderpackage p ){
    void * payload [256];
    if( false == renderpackage_gl::unpack( &p, payload ) )
	return false;
    uint64_t * n_data = ( uint64_t * )payload[0];
    uint64_t * program_handle = ( uint64_t * )payload[1];
    assert( n_data );
    assert( 1 == *n_data );
    assert( program_handle );
    GLuint * gl_program_handle = ( GLuint * ) program_handle;
    return ::e2::render::gl::gl_helper::link_program( *gl_program_handle );
}

bool renderdevice_gl_impl::process_bind_program( renderdevice_gl_impl * context, ::e2::interface::i_renderpackage p ){
    void * payload [256];
    if( false == renderpackage_gl::unpack( &p, payload ) )
	return false;
    uint64_t * n_data = ( uint64_t * )payload[0];
    uint64_t * program_handle = ( uint64_t * )payload[1];
    assert( n_data );
    assert( 1 == *n_data );
    assert( program_handle );
    GLuint * gl_program_handle = ( GLuint * ) program_handle;
    return ::e2::render::gl::gl_helper::use_program( *gl_program_handle );
}

bool renderdevice_gl_impl::process_store_persistent( renderdevice_gl_impl * context, ::e2::interface::i_renderpackage p ){
    void * payload [256];
    if( false == renderpackage_gl::unpack( &p, payload ) )
	return false;
    switch( p._resource->_id ){
    case ::e2::interface::renderresource_persistent_vec_f:
    {
	uint64_t * n_data = ( uint64_t * )payload[0];
	uint64_t * dim = ( uint64_t * )payload[1];
        GLuint * gl_program_handle = ( GLuint * )payload[2];
        char * name = ( char * )payload[3];
        float * vec = ( float * )payload[4];
	assert( n_data );
	assert( dim );
	assert( gl_program_handle );
	assert( name );
	assert( vec );
	assert( 4 == *n_data );
	switch( *dim ){
	case 1:
	{
	    return ::e2::render::gl::gl_helper::set_uniform_f1( *gl_program_handle, name, vec );
	}
	break;
	case 2:
	{
	    return ::e2::render::gl::gl_helper::set_uniform_f2( *gl_program_handle, name, vec );
	}
	break;
	case 3:
	{
	    return ::e2::render::gl::gl_helper::set_uniform_f3( *gl_program_handle, name, vec );
	}
	break;
	case 4:
	{
	    return ::e2::render::gl::gl_helper::set_uniform_f4( *gl_program_handle, name, vec );
	}
	break;
	}
    }
    break;
    case ::e2::interface::renderresource_persistent_mat_f:
    {
	uint64_t * n_data = ( uint64_t * )payload[0];
	uint64_t * dim = ( uint64_t * )payload[1];
        GLuint * gl_program_handle = ( GLuint * )payload[2];
        char * name = ( char * )payload[3];
        float * m = ( float * )payload[4];
	assert( n_data );
	assert( dim );
	assert( gl_program_handle );
	assert( name );
	assert( m );
	assert( 4 == *n_data );
	switch( *dim ){
	case 2:
	{
	    return ::e2::render::gl::gl_helper::set_uniform_matf2( *gl_program_handle, name, m );
	}
	break;
	case 3:
	{
	    return ::e2::render::gl::gl_helper::set_uniform_matf3( *gl_program_handle, name, m );
	}
	break;
	case 4:
	{
	    return ::e2::render::gl::gl_helper::set_uniform_matf4( *gl_program_handle, name, m );
	}
	break;
	}
    }
    break;
    default:
    {
    }
    break;
    }
    return false;
}

} }
