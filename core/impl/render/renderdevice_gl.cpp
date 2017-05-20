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
						 ::e2::interface::e_renderresource_type::binding ) ] = &process_store_binding;

    _dispatch._dispatch_map[ renderdispatch_key( ::e2::interface::e_rendercmd_type::exec,
						 ::e2::interface::e_renderresource_type::draw_batch ) ] = &process_exec_draw_batch;
	
    _dispatch._dispatch_map[ renderdispatch_key( ::e2::interface::e_rendercmd_type::store,
						 ::e2::interface::e_renderresource_type::buffer ) ] = &process_store_buffer;
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

bool renderdevice_gl_impl::process_exec_draw_batch( renderdevice_gl_impl * context, ::e2::interface::i_renderpackage p ){
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

bool renderdevice_gl_impl::process_store_buffer( renderdevice_gl_impl * context, ::e2::interface::i_renderpackage p ){
    return false;
}

bool renderdevice_gl_impl::process_store_binding( renderdevice_gl_impl * context, ::e2::interface::i_renderpackage p ){
    void * payload [256];
    if( false == renderpackage_gl::unpack( &p, payload ) )
	return false;
    bool ret = false;
    switch( p._resource->_id ){
    case ::e2::interface::renderresource_binding_vertex_array:
    {
	uint64_t * n_data = ( uint64_t * )payload[0];
	uint64_t * vertex_array_handle = ( uint64_t * )payload[1];
	assert( n_data );
	assert( 1 == *n_data );
	assert( vertex_array_handle );
	GLuint gl_vertex_array_handle = ( GLuint ) *vertex_array_handle;
	ret = ::e2::render::gl::gl_helper::bind_vertex_array( gl_vertex_array_handle );
    }
    break;
    default:
    {
    }
    break;	
    }
    return ret;
}

} }
