#include <iostream>
#include <cstdio>
#include <cassert>
#include <cstring>
#include <map>

#include "i_renderpackage.hpp"
#include "i_rendercmd.hpp"
#include "renderdevice_gl.hpp"
#include "gl_includes.hpp"
#include "renderdevice_dispatch.hpp"

namespace e2 { namespace render {

renderdevice_gl_impl::renderdevice_gl_impl(){

    _dispatch._dispatch_map[ renderdispatch_key( ::e2::interface::e_rendercmd_type::init,
						 ::e2::interface::e_renderresource_type::windowing,
						 ::e2::interface::e_renderpayload_type::int_2 ) ] = &process_init_window;

    _dispatch._dispatch_map[ renderdispatch_key( ::e2::interface::e_rendercmd_type::deinit,
						 ::e2::interface::e_renderresource_type::windowing,
						 ::e2::interface::e_renderpayload_type::na ) ] = &process_deinit_window;

    _buf = nullptr;
}

bool renderdevice_gl_impl::renderdevice_process( ::e2::interface::i_renderpackage p ){
    return _dispatch.dispatch_process( this, p );
    return true;
}

bool renderdevice_gl_impl::process_init_window_size( renderdevice_gl_impl * context, ::e2::interface::i_renderpackage p ){
    std::cout << "process init window size" << std::endl;
    return true;
}

bool renderdevice_gl_impl::process_init_window( renderdevice_gl_impl * context, ::e2::interface::i_renderpackage p ){
    std::cout << "process init window" << std::endl;
    if( false == glfwInit() ){
	assert( false && "glfw init failed." );
	return false;
    }
    {
	if( nullptr == p._payload ){
	    assert( false && "init window dim invalid." );
	    return false;
	}
	if( nullptr == context->_buf ){
	    assert( false && "buffer invalid." );
	    return false;
	}
        int * wind_dim;
	if( false == context->_buf->buffer_get( p._payload->_offset, &wind_dim, 2 ) ){
	    assert( false && "buffer data invalid." );
	    return false;
	}
	context->_device_resources[ resource_id::window_dim ] = wind_dim;
		
	auto it = context->_device_resources.find( window_dim );
	if( context->_device_resources.end() == it ){
	    assert( false && "device resource not found" );
	    return false;
	}
	int * dim = (int *)it->second;
	if( nullptr == dim ){
	    assert( false && "window_dim invalid" );
	    return false;
	}
	glfwWindowHint( GLFW_CONTEXT_VERSION_MAJOR, 3 );
	glfwWindowHint( GLFW_CONTEXT_VERSION_MINOR, 3 );
	glfwWindowHint( GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE );
	context->_window = glfwCreateWindow( dim[0], dim[1], "renderdevice_gl window", nullptr, nullptr );
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
    return true;
}

bool renderdevice_gl_impl::process_deinit_window( renderdevice_gl_impl * context, ::e2::interface::i_renderpackage p ){
    std::cout << "process deinit window" << std::endl;
    if( nullptr != context->_window ){
	glfwDestroyWindow( context->_window );
	context->_window = nullptr;
    }
    glfwTerminate();
    return true;
}
	
} }
