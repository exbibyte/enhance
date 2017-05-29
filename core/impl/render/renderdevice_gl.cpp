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

    _dispatch._dispatch_map[ renderdispatch_key( ::e2::interface::e_rendercmd_type_init,
                                                 ::e2::interface::e_renderresource_type_windowing ) ] = &process_init_window;

    _dispatch._dispatch_map[ renderdispatch_key( ::e2::interface::e_rendercmd_type_deinit,
                                                 ::e2::interface::e_renderresource_type_windowing ) ] = &process_deinit_window;

    _dispatch._dispatch_map[ renderdispatch_key( ::e2::interface::e_rendercmd_type_exec,
                                                 ::e2::interface::e_renderresource_type_windowing  ) ] = &process_exec_window;

    _dispatch._dispatch_map[ renderdispatch_key( ::e2::interface::e_rendercmd_type_store,
                                                 ::e2::interface::e_renderresource_type_defineformat ) ] = &process_store_defineformat;
        
    _dispatch._dispatch_map[ renderdispatch_key( ::e2::interface::e_rendercmd_type_bind,
                                                 ::e2::interface::e_renderresource_type_object ) ] = &process_bind_object;

    _dispatch._dispatch_map[ renderdispatch_key( ::e2::interface::e_rendercmd_type_bind,
                                                 ::e2::interface::e_renderresource_type_buffer ) ] = &process_bind_buffer;

    _dispatch._dispatch_map[ renderdispatch_key( ::e2::interface::e_rendercmd_type_exec,
                                                 ::e2::interface::e_renderresource_type_drawbatch ) ] = &process_exec_drawbatch;

    _dispatch._dispatch_map[ renderdispatch_key( ::e2::interface::e_rendercmd_type_enable,
                                                 ::e2::interface::e_renderresource_type_attrib ) ] = &process_enable_attrib;

    _dispatch._dispatch_map[ renderdispatch_key( ::e2::interface::e_rendercmd_type_disable,
                                                 ::e2::interface::e_renderresource_type_attrib ) ] = &process_disable_attrib;

    _dispatch._dispatch_map[ renderdispatch_key( ::e2::interface::e_rendercmd_type_init,
                                                 ::e2::interface::e_renderresource_type_object ) ] = &process_init_object;

    _dispatch._dispatch_map[ renderdispatch_key( ::e2::interface::e_rendercmd_type_deinit,
                                                 ::e2::interface::e_renderresource_type_object ) ] = &process_deinit_object;

    _dispatch._dispatch_map[ renderdispatch_key( ::e2::interface::e_rendercmd_type_store,
                                                 ::e2::interface::e_renderresource_type_buffer ) ] = &process_store_buffer;

    _dispatch._dispatch_map[ renderdispatch_key( ::e2::interface::e_rendercmd_type_init,
                                                 ::e2::interface::e_renderresource_type_buffer ) ] = &process_init_buffer;

    _dispatch._dispatch_map[ renderdispatch_key( ::e2::interface::e_rendercmd_type_deinit,
                                                 ::e2::interface::e_renderresource_type_buffer ) ] = &process_deinit_buffer;

    _dispatch._dispatch_map[ renderdispatch_key( ::e2::interface::e_rendercmd_type_bind,
                                                 ::e2::interface::e_renderresource_type_attrib ) ] = &process_bind_attrib;

    _dispatch._dispatch_map[ renderdispatch_key( ::e2::interface::e_rendercmd_type_init,
                                                 ::e2::interface::e_renderresource_type_program ) ] = &process_init_program;

    _dispatch._dispatch_map[ renderdispatch_key( ::e2::interface::e_rendercmd_type_deinit,
                                                 ::e2::interface::e_renderresource_type_program ) ] = &process_deinit_program;

    _dispatch._dispatch_map[ renderdispatch_key( ::e2::interface::e_rendercmd_type_compute,
                                                 ::e2::interface::e_renderresource_type_program ) ] = &process_compute_program;

    _dispatch._dispatch_map[ renderdispatch_key( ::e2::interface::e_rendercmd_type_bind,
                                                 ::e2::interface::e_renderresource_type_shader ) ] = &process_bind_shader;
	
    _dispatch._dispatch_map[ renderdispatch_key( ::e2::interface::e_rendercmd_type_bind,
                                                 ::e2::interface::e_renderresource_type_program ) ] = &process_bind_program;

    _dispatch._dispatch_map[ renderdispatch_key( ::e2::interface::e_rendercmd_type_store,
                                                 ::e2::interface::e_renderresource_type_persistent ) ] = &process_store_persistent;

    _dispatch._dispatch_map[ renderdispatch_key( ::e2::interface::e_rendercmd_type_load,
                                                 ::e2::interface::e_renderresource_type_shader ) ] = &process_load_shader;

    _dispatch._dispatch_map[ renderdispatch_key( ::e2::interface::e_rendercmd_type_store,
                                                 ::e2::interface::e_renderresource_type_program ) ] = &process_store_program;

    _dispatch._dispatch_map[ renderdispatch_key( ::e2::interface::e_rendercmd_type_query,
                                                 ::e2::interface::e_renderresource_type_persistent ) ] = &process_query_persistent;

    _dispatch._dispatch_map[ renderdispatch_key( ::e2::interface::e_rendercmd_type_query,
                                                 ::e2::interface::e_renderresource_type_attrib ) ] = &process_query_attrib;
						 
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
    if( false == glfwInit() ){
        assert( false && "glfw init failed." );
        return false;
    }
    {
	::e2::interface::i_package_filter f( { ::e2::interface::e_renderresourcekey_wind_dimx,
		                               ::e2::interface::e_renderresourcekey_wind_dimy } );
        if( false == renderpackage_gl::unpack( &p, &f ) )
            return false;
        int * dimx = ( int * ) f[ ::e2::interface::e_renderresourcekey_wind_dimx ];
	int * dimy = ( int * ) f[ ::e2::interface::e_renderresourcekey_wind_dimy ];
        glfwWindowHint( GLFW_CONTEXT_VERSION_MAJOR, 3 );
        glfwWindowHint( GLFW_CONTEXT_VERSION_MINOR, 3 );
        glfwWindowHint( GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE );
        context->_window = glfwCreateWindow( *dimx, *dimy, "renderdevice_gl window", nullptr, nullptr );
        if( false == context->_window ){
            assert( false && "window creation failed." );
            return false;
        }

        glfwMakeContextCurrent( context->_window );

	if (gl3wInit()) {
            assert( false && "gl3w initialization failed." );
	    return false;
	}
	if (!gl3wIsSupported(3, 3)) {
	    assert( false && "OpenGL 3.3 not supported");
	    return false;
	}		

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

	glEnable( GL_DEPTH_TEST );
        glClearColor( 50, 50, 50, 1.0 );
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
    switch( p._resource_subtype ){
    case ::e2::interface::e_renderresource_subtype_window_buf_swap:
    {
        assert( context->_window );
#ifdef debug_info
        std::cout << "process window buf swap" << std::endl;
#endif
        glfwMakeContextCurrent( context->_window );
        glfwSwapBuffers( context->_window );
    }
    break;
    case ::e2::interface::e_renderresource_subtype_window_clear_colour:
    {
	glClear( GL_COLOR_BUFFER_BIT );
    }
    break;
    case ::e2::interface::e_renderresource_subtype_window_clear_depth:
    {
	glClear( GL_DEPTH_BUFFER_BIT );
    }
    break;
    case ::e2::interface::e_renderresource_subtype_window_buffer_disable_depth:
    {
	glDisable( GL_DEPTH_TEST );
    }
    break;
    case ::e2::interface::e_renderresource_subtype_window_buffer_enable_depth:
    {
	glEnable( GL_DEPTH_TEST );
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
    ::e2::interface::i_package_filter f( { ::e2::interface::e_renderresourcekey_primitive_type,
		                           ::e2::interface::e_renderresourcekey_primitive_offset,
		                           ::e2::interface::e_renderresourcekey_primitive_count } );
    if( false == renderpackage_gl::unpack( &p, &f ) )
	return false;
    uint64_t * primitive_type = ( uint64_t * ) f[ ::e2::interface::e_renderresourcekey_primitive_type ];
    int * offset = ( int * ) f[ ::e2::interface::e_renderresourcekey_primitive_offset ];
    int * count = ( int * ) f[ ::e2::interface::e_renderresourcekey_primitive_count ];
    assert( primitive_type );
    assert( offset );
    assert( count );
    GLenum gl_primitive_type;
    GLint gl_first;
    GLsizei gl_count;
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
    if( ::e2::interface::e_renderresource_subtype_vertex_attrib_data == p._resource_subtype ){

	::e2::interface::i_package_filter f( { ::e2::interface::e_renderresourcekey_index,
		                               ::e2::interface::e_renderresourcekey_size,
		                               ::e2::interface::e_renderresourcekey_type,
		                               ::e2::interface::e_renderresourcekey_normalized,
		                               ::e2::interface::e_renderresourcekey_stride,
		                               ::e2::interface::e_renderresourcekey_pointer } );
	if( false == renderpackage_gl::unpack( &p, &f ) )
	    return false;
	
        uint64_t * va_index = ( uint64_t * ) f[ ::e2::interface::e_renderresourcekey_index ];
        uint64_t * va_size = ( uint64_t * ) f[ ::e2::interface::e_renderresourcekey_size ];
        uint64_t * va_type = ( uint64_t * ) f[ ::e2::interface::e_renderresourcekey_type ];
        bool * va_normalized = ( bool * ) f[ ::e2::interface::e_renderresourcekey_normalized ];
        uint64_t * va_stride = ( uint64_t * ) f[ ::e2::interface::e_renderresourcekey_stride ];
        void * va_pointer = ( void * ) f[ ::e2::interface::e_renderresourcekey_pointer ];
        assert( va_index );
        assert( va_size );
        assert( va_type );
        assert( va_normalized );
        assert( va_stride );
	GLuint gl_va_index = *va_index;
        GLint gl_va_size = *va_size;
        GLboolean gl_va_normalized = *va_normalized;
        GLsizei gl_va_stride = *va_stride;
        GLvoid * gl_va_pointer = va_pointer;
	auto it = rendermap_gl::_map_render_data_type.find( *va_type );
	if( rendermap_gl::_map_render_data_type.end() == it ){
	    assert( false && "data type invalid." );
	    return false;
	}
	GLenum gl_va_type = it->second;
	return ::e2::render::gl::gl_helper::define_vertex_attrib_data( gl_va_index, gl_va_size, gl_va_type, gl_va_normalized, gl_va_stride, gl_va_pointer );
    }
    return false;
}

bool renderdevice_gl_impl::process_bind_object( renderdevice_gl_impl * context, ::e2::interface::i_renderpackage p ){
    if( ::e2::interface::e_renderresource_subtype_object_vertex_array == p._resource_subtype ){
	::e2::interface::i_package_filter f( { ::e2::interface::e_renderresourcekey_va_handle } );
	if( false == renderpackage_gl::unpack( &p, &f ) )
	    return false;
        GLuint * gl_vertex_array_handle = ( GLuint * ) f[ ::e2::interface::e_renderresourcekey_va_handle ];
        assert( gl_vertex_array_handle );
	return ::e2::render::gl::gl_helper::bind_vertex_array( *gl_vertex_array_handle );
    }
    return false;
}

bool renderdevice_gl_impl::process_bind_buffer( renderdevice_gl_impl * context, ::e2::interface::i_renderpackage p ){
    ::e2::interface::i_package_filter f( { ::e2::interface::e_renderresourcekey_buffer_type,
		                           ::e2::interface::e_renderresourcekey_buffer_handle } );
    if( false == renderpackage_gl::unpack( &p, &f ) )
	return false;
    uint64_t * buffer_type = ( uint64_t * ) f[ ::e2::interface::e_renderresourcekey_buffer_type ];
    uint64_t * buffer_handle = ( uint64_t * ) f[ ::e2::interface::e_renderresourcekey_buffer_handle ];
    assert( buffer_type );
    assert( buffer_handle );
    GLuint gl_buffer_handle = *buffer_handle;
    auto it = rendermap_gl::_map_render_bind_buffer.find( *buffer_type );
    if( rendermap_gl::_map_render_bind_buffer.end() == it ){
        assert( false && "bind buffer type invalid." );
        return false;
    }
        // GLenum gl_buffer_type = it->second;
    GLenum gl_buffer_type = GL_ARRAY_BUFFER;
    return ::e2::render::gl::gl_helper::bind_buffer( gl_buffer_type, gl_buffer_handle );
}

bool renderdevice_gl_impl::process_enable_attrib( renderdevice_gl_impl * context, ::e2::interface::i_renderpackage p ){
    if( ::e2::interface::e_renderresource_subtype_object_vertex_array == p._resource_subtype ){
	::e2::interface::i_package_filter f( { ::e2::interface::e_renderresourcekey_va_index } );
	if( false == renderpackage_gl::unpack( &p, &f ) )
	    return false;        uint64_t * va_index = ( uint64_t * ) f[ ::e2::interface::e_renderresourcekey_va_index ];
	assert( va_index );
	GLuint gl_va_index = (GLuint) *va_index;
        return ::e2::render::gl::gl_helper::enable_vertex_attrib_array( gl_va_index );
    }
    return false;
}

bool renderdevice_gl_impl::process_disable_attrib( renderdevice_gl_impl * context, ::e2::interface::i_renderpackage p ){;
    if( ::e2::interface::e_renderresource_subtype_vertex_attrib_array == p._resource_subtype ){
	::e2::interface::i_package_filter f( { ::e2::interface::e_renderresourcekey_va_index } );
	if( false == renderpackage_gl::unpack( &p, &f ) )
	    return false;
	GLuint * gl_va_index = ( GLuint * ) f[ ::e2::interface::e_renderresourcekey_va_index ];
        assert( gl_va_index );
        return ::e2::render::gl::gl_helper::disable_vertex_attrib_array( *gl_va_index );
    }
    return false;
}

bool renderdevice_gl_impl::process_init_object( renderdevice_gl_impl * context, ::e2::interface::i_renderpackage p ){
    if( ::e2::interface::e_renderresource_subtype_object_vertex_array == p._resource_subtype ){
	::e2::interface::i_package_filter f( { ::e2::interface::e_renderresourcekey_va_num,
		                               ::e2::interface::e_renderresourcekey_va_handle } );
	if( false == renderpackage_gl::unpack( &p, &f ) )
	    return false;
	GLsizei * gl_size_va_names = ( GLsizei * ) f[ ::e2::interface::e_renderresourcekey_va_num ];
        GLuint * gl_va_array = ( GLuint * ) f[ ::e2::interface::e_renderresourcekey_va_handle ];
        assert( gl_size_va_names );
        assert( gl_va_array );
	return ::e2::render::gl::gl_helper::generate_vertex_arrays( *gl_size_va_names, gl_va_array );
    }
    return false;
}

bool renderdevice_gl_impl::process_deinit_object( renderdevice_gl_impl * context, ::e2::interface::i_renderpackage p ){
    if( ::e2::interface::e_renderresource_subtype_object_vertex_array == p._resource_subtype ){
	::e2::interface::i_package_filter f( { ::e2::interface::e_renderresourcekey_va_num,
		                               ::e2::interface::e_renderresourcekey_va_array } );
	if( false == renderpackage_gl::unpack( &p, &f ) )
	    return false;
	GLsizei * gl_size_va_names = ( GLsizei * ) f[ ::e2::interface::e_renderresourcekey_va_num ];
        GLuint * gl_va_array = ( GLuint * ) f[ ::e2::interface::e_renderresourcekey_va_array ];
        assert( gl_size_va_names );
        assert( gl_va_array );
	return ::e2::render::gl::gl_helper::delete_vertex_arrays( *gl_size_va_names, gl_va_array );
    }
    return false;
}

bool renderdevice_gl_impl::process_store_buffer( renderdevice_gl_impl * context, ::e2::interface::i_renderpackage p ){
    ::e2::interface::i_package_filter f( { ::e2::interface::e_renderresourcekey_buffer_type,
		                           ::e2::interface::e_renderresourcekey_buffer_size,
		                           ::e2::interface::e_renderresourcekey_buffer_data,
		                           ::e2::interface::e_renderresourcekey_buffer_usage } );
    if( false == renderpackage_gl::unpack( &p, &f ) )
	return false;
    uint64_t * buffer_type = ( uint64_t * ) f[ ::e2::interface::e_renderresourcekey_buffer_type ];
    GLsizeiptr * gl_bytes_size = ( GLsizeiptr * ) f[ ::e2::interface::e_renderresourcekey_buffer_size ];
    GLvoid * gl_data = ( GLvoid * ) f[ ::e2::interface::e_renderresourcekey_buffer_data ];
    GLenum * gl_usage = ( GLenum * ) f[ ::e2::interface::e_renderresourcekey_buffer_usage ];
    assert( buffer_type );
    assert( gl_bytes_size );
    assert( gl_data );
    assert( gl_usage );
    auto it = rendermap_gl::_map_render_bind_buffer.find( *buffer_type );
    if( rendermap_gl::_map_render_bind_buffer.end() == it ){
        assert( false && "bind buffer type invalid." );
        return false;
    }
    GLenum gl_buffer_type = it->second;
    return ::e2::render::gl::gl_helper::store_buffer_data( gl_buffer_type, *gl_bytes_size, gl_data, *gl_usage );
}

bool renderdevice_gl_impl::process_init_buffer( renderdevice_gl_impl * context, ::e2::interface::i_renderpackage p ){
    ::e2::interface::i_package_filter f( { ::e2::interface::e_renderresourcekey_buffer_num,
		                           ::e2::interface::e_renderresourcekey_buffer_handle } );
    if( false == renderpackage_gl::unpack( &p, &f ) )
	return false;
    uint64_t * num_buffers = ( uint64_t * ) f[ ::e2::interface::e_renderresourcekey_buffer_num ];
    uint64_t * buffers = ( uint64_t * ) f[ ::e2::interface::e_renderresourcekey_buffer_handle ];
    assert( num_buffers );
    assert( buffers );
    GLsizei gl_num_buffers = ( GLsizei ) *num_buffers;
    std::vector< GLuint > gl_buffers( *num_buffers );
    assert( ::e2::render::gl::gl_helper::generate_buffers( gl_num_buffers, &gl_buffers[0] ) );
    for( size_t i = 0; i < *num_buffers; ++i ){
	buffers[i] = gl_buffers[i];
    }
    return true;
}

bool renderdevice_gl_impl::process_deinit_buffer( renderdevice_gl_impl * context, ::e2::interface::i_renderpackage p ){
    ::e2::interface::i_package_filter f( { ::e2::interface::e_renderresourcekey_buffer_num,
		                           ::e2::interface::e_renderresourcekey_buffer_handle } );
    if( false == renderpackage_gl::unpack( &p, &f ) )
	return false;
    uint64_t * num_buffers = ( uint64_t * ) f[ ::e2::interface::e_renderresourcekey_buffer_num ];
    uint64_t * buffers = ( uint64_t * ) f[ ::e2::interface::e_renderresourcekey_buffer_handle ];
    assert( num_buffers );
    assert( buffers );
    GLsizei gl_num_buffers = ( GLsizei ) *num_buffers;
    std::vector< GLuint > gl_buffers( *num_buffers );
    for( size_t i = 0; i < gl_buffers.size(); ++i ){
	gl_buffers[i] = buffers[i];
    }
    return ::e2::render::gl::gl_helper::delete_buffers( gl_num_buffers, &gl_buffers[0] );
}

bool renderdevice_gl_impl::process_bind_attrib( renderdevice_gl_impl * context, ::e2::interface::i_renderpackage p ){
    if( ::e2::interface::e_renderresource_subtype_attrib_attrib_location == p._resource_subtype ){
	::e2::interface::i_package_filter f( { ::e2::interface::e_renderresourcekey_program_handle,
		                               ::e2::interface::e_renderresourcekey_attrib_index,
		                               ::e2::interface::e_renderresourcekey_attrib_name } );
	if( false == renderpackage_gl::unpack( &p, &f ) )
	    return false;
	GLuint * gl_program_handle = ( GLuint * ) f[ ::e2::interface::e_renderresourcekey_program_handle ];
	GLuint * gl_attrib_index = ( GLuint * ) f[ ::e2::interface::e_renderresourcekey_attrib_index ];
	GLchar * gl_attrib_name = ( GLchar * ) f[ ::e2::interface::e_renderresourcekey_attrib_name ];
	assert( gl_program_handle );
        assert( gl_attrib_index );
        assert( gl_attrib_name );
        return ::e2::render::gl::gl_helper::bind_attrib_location( *gl_program_handle, *gl_attrib_index, gl_attrib_name );
    }else if( ::e2::interface::e_renderresource_subtype_attrib_frag_location == p._resource_subtype ){
	::e2::interface::i_package_filter f( { ::e2::interface::e_renderresourcekey_program_handle,
		                               ::e2::interface::e_renderresourcekey_frag_location,
		                               ::e2::interface::e_renderresourcekey_frag_name } );
	if( false == renderpackage_gl::unpack( &p, &f ) )
	    return false;
	GLuint * gl_program_handle = ( GLuint * ) f[ ::e2::interface::e_renderresourcekey_program_handle ];
	GLuint * gl_frag_location = ( GLuint * ) f[ ::e2::interface::e_renderresourcekey_frag_location ];
	char * gl_frag_name = ( char * ) f[ ::e2::interface::e_renderresourcekey_frag_name ];
	assert( gl_program_handle );
        assert( gl_frag_location );
        assert( gl_frag_name );
        return ::e2::render::gl::gl_helper::bind_frag_data_location( *gl_program_handle, *gl_frag_location, gl_frag_name );
    }
    return false;
}

bool renderdevice_gl_impl::process_init_program( renderdevice_gl_impl * context, ::e2::interface::i_renderpackage p ){
    ::e2::interface::i_package_filter f( { ::e2::interface::e_renderresourcekey_program_handle } );
    if( false == renderpackage_gl::unpack( &p, &f ) )
	return false;
    GLuint * gl_program_handle = ( GLuint * ) f[ ::e2::interface::e_renderresourcekey_program_handle ];
    assert( gl_program_handle );
    return ::e2::render::gl::gl_helper::create_program( gl_program_handle );
}

bool renderdevice_gl_impl::process_deinit_program( renderdevice_gl_impl * context, ::e2::interface::i_renderpackage p ){
    ::e2::interface::i_package_filter f( { ::e2::interface::e_renderresourcekey_program_handle } );
    if( false == renderpackage_gl::unpack( &p, &f ) )
	return false;
    GLuint * gl_program_handle = ( GLuint * ) f[ ::e2::interface::e_renderresourcekey_program_handle ];
    assert( gl_program_handle );
    return ::e2::render::gl::gl_helper::delete_program( *gl_program_handle );
}

bool renderdevice_gl_impl::process_bind_shader( renderdevice_gl_impl * context, ::e2::interface::i_renderpackage p ){
    ::e2::interface::i_package_filter f( { ::e2::interface::e_renderresourcekey_program_handle,
		                           ::e2::interface::e_renderresourcekey_shader_handle } );
    if( false == renderpackage_gl::unpack( &p, &f ) )
	return false;
    GLuint * gl_program_handle = ( GLuint * ) f[ ::e2::interface::e_renderresourcekey_program_handle ];
    GLuint * gl_shader_handle = ( GLuint * ) f[ ::e2::interface::e_renderresourcekey_shader_handle ];
    assert( gl_program_handle );
    assert( gl_shader_handle );
    return ::e2::render::gl::gl_helper::attach_shader( *gl_program_handle, *gl_shader_handle );
}

bool renderdevice_gl_impl::process_unbind_shader( renderdevice_gl_impl * context, ::e2::interface::i_renderpackage p ){
    ::e2::interface::i_package_filter f( { ::e2::interface::e_renderresourcekey_program_handle,
		                           ::e2::interface::e_renderresourcekey_shader_handle } );
    if( false == renderpackage_gl::unpack( &p, &f ) )
	return false;
    GLuint * gl_program_handle = ( GLuint * ) f[ ::e2::interface::e_renderresourcekey_program_handle ];
    GLuint * gl_shader_handle = ( GLuint * ) f[ ::e2::interface::e_renderresourcekey_shader_handle ];
    assert( gl_program_handle );
    assert( gl_shader_handle );
    return ::e2::render::gl::gl_helper::detach_shader( *gl_program_handle, *gl_shader_handle );
}

bool renderdevice_gl_impl::process_compute_program( renderdevice_gl_impl * context, ::e2::interface::i_renderpackage p ){
    ::e2::interface::i_package_filter f( { ::e2::interface::e_renderresourcekey_program_handle } );
    if( false == renderpackage_gl::unpack( &p, &f ) )
	return false;
    GLuint * gl_program_handle = ( GLuint * ) f[ ::e2::interface::e_renderresourcekey_program_handle ];
    assert( gl_program_handle );
    return ::e2::render::gl::gl_helper::link_program( *gl_program_handle );
}

bool renderdevice_gl_impl::process_bind_program( renderdevice_gl_impl * context, ::e2::interface::i_renderpackage p ){
    ::e2::interface::i_package_filter f( { ::e2::interface::e_renderresourcekey_program_handle } );
    if( false == renderpackage_gl::unpack( &p, &f ) )
	return false;
    GLuint * gl_program_handle = ( GLuint * ) f[ ::e2::interface::e_renderresourcekey_program_handle ];
    assert( gl_program_handle );
    return ::e2::render::gl::gl_helper::use_program( *gl_program_handle );
}

bool renderdevice_gl_impl::process_store_persistent( renderdevice_gl_impl * context, ::e2::interface::i_renderpackage p ){
    switch( p._resource_subtype ){
    case ::e2::interface::e_renderresource_subtype_persistent_vec_f:
    {
	::e2::interface::i_package_filter f( { ::e2::interface::e_renderresourcekey_program_handle,
		                               ::e2::interface::e_renderresourcekey_dim,
		                               ::e2::interface::e_renderresourcekey_name,
		                               ::e2::interface::e_renderresourcekey_arrayf } );
	if( false == renderpackage_gl::unpack( &p, &f ) )
	    return false;
	GLuint * gl_program_handle = ( GLuint * ) f[ ::e2::interface::e_renderresourcekey_program_handle ];
	uint64_t * dim = ( uint64_t * ) f[ ::e2::interface::e_renderresourcekey_dim ];
	char * name = ( char * ) f[ ::e2::interface::e_renderresourcekey_name ];
	float * vec = ( float * ) f[ ::e2::interface::e_renderresourcekey_arrayf ];
	assert( gl_program_handle );	    
	assert( dim );
	assert( name );
	assert( vec );
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
    case ::e2::interface::e_renderresource_subtype_persistent_mat_f:
    {
	::e2::interface::i_package_filter f( { ::e2::interface::e_renderresourcekey_program_handle,
		                               ::e2::interface::e_renderresourcekey_dim,
		                               ::e2::interface::e_renderresourcekey_name,
		                               ::e2::interface::e_renderresourcekey_arrayf } );
	if( false == renderpackage_gl::unpack( &p, &f ) )
	    return false;
	GLuint * gl_program_handle = ( GLuint * ) f[ ::e2::interface::e_renderresourcekey_program_handle ];
	uint64_t * dim = ( uint64_t * ) f[ ::e2::interface::e_renderresourcekey_dim ];
	char * name = ( char * ) f[ ::e2::interface::e_renderresourcekey_name ];
	float * vec = ( float * ) f[ ::e2::interface::e_renderresourcekey_arrayf ];
	assert( gl_program_handle );	    
	assert( dim );
	assert( name );
	assert( vec );
        switch( *dim ){
        case 2:
        {
            return ::e2::render::gl::gl_helper::set_uniform_matf2( *gl_program_handle, name, vec );
        }
        break;
        case 3:
        {
            return ::e2::render::gl::gl_helper::set_uniform_matf3( *gl_program_handle, name, vec );
        }
        break;
        case 4:
        {
            return ::e2::render::gl::gl_helper::set_uniform_matf4( *gl_program_handle, name, vec );
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

bool renderdevice_gl_impl::process_load_shader( renderdevice_gl_impl * context, ::e2::interface::i_renderpackage p ){
    ::e2::interface::i_package_filter f( { ::e2::interface::e_renderresourcekey_shader_handle,
		                           ::e2::interface::e_renderresourcekey_shader_source,
		                           ::e2::interface::e_renderresourcekey_shader_type } );
    if( false == renderpackage_gl::unpack( &p, &f ) )
	return false;
    GLuint * gl_shader_handle = ( GLuint * ) f[ ::e2::interface::e_renderresourcekey_shader_handle ];
    char * shader_source = ( char * ) f[ ::e2::interface::e_renderresourcekey_shader_source ];
    uint64_t * shader_type = ( uint64_t * ) f[ ::e2::interface::e_renderresourcekey_shader_type ];
    assert( gl_shader_handle );
    assert( shader_source );
    assert( shader_type );
    switch( p._resource_subtype ){
    case ::e2::interface::e_renderresource_subtype_shader_file:
    {
        return ::e2::render::gl::gl_helper::compile_shader_from_file( gl_shader_handle, shader_source, ( ::e2::render::gl::GLSLShaderType ) *shader_type );
    }
    break;
    case ::e2::interface::e_renderresource_subtype_shader_string:
    {
        return ::e2::render::gl::gl_helper::compile_shader_from_string( gl_shader_handle, shader_source, ( ::e2::render::gl::GLSLShaderType ) *shader_type );
    }
    break;
    default:
    {
    }
    }
    return false;
}

bool renderdevice_gl_impl::process_store_program( renderdevice_gl_impl * context, ::e2::interface::i_renderpackage p ){   
    switch( p._resource_subtype ){
    case ::e2::interface::e_renderresource_subtype_program_parameter:
    {
	::e2::interface::i_package_filter f( { ::e2::interface::e_renderresourcekey_program_handle,
		                               ::e2::interface::e_renderresourcekey_program_param_type,
		                               ::e2::interface::e_renderresourcekey_program_param_val } );
	if( false == renderpackage_gl::unpack( &p, &f ) )
	    return false;
	GLuint * gl_program_handle = ( GLuint * ) f[ ::e2::interface::e_renderresourcekey_program_handle ];
	GLuint * gl_parameter = ( GLuint * ) f[ ::e2::interface::e_renderresourcekey_program_param_type ];
	GLuint * gl_parameter_value = ( GLuint * ) f[ ::e2::interface::e_renderresourcekey_program_param_val ];
        assert( gl_program_handle );
        assert( gl_parameter );
        assert( gl_parameter_value );
        return ::e2::render::gl::gl_helper::set_program_parameter( *gl_program_handle, *gl_parameter, *gl_parameter_value );
    }
    break;
    default:
    {
    }
    }
    return false;
}

bool renderdevice_gl_impl::process_query_persistent( renderdevice_gl_impl *, ::e2::interface::i_renderpackage p ){
    ::e2::interface::i_package_filter f( { ::e2::interface::e_renderresourcekey_program_handle } );
    if( false == renderpackage_gl::unpack( &p, &f ) )
        return false;
    GLuint * gl_program_handle = ( GLuint * ) f[ ::e2::interface::e_renderresourcekey_program_handle ];
    assert( gl_program_handle );
    return ::e2::render::gl::gl_helper::print_active_uniforms( *gl_program_handle );
}
bool renderdevice_gl_impl::process_query_attrib( renderdevice_gl_impl *, ::e2::interface::i_renderpackage p ){
    ::e2::interface::i_package_filter f( { ::e2::interface::e_renderresourcekey_program_handle } );
    if( false == renderpackage_gl::unpack( &p, &f ) )
        return false;
    GLuint * gl_program_handle = ( GLuint * ) f[ ::e2::interface::e_renderresourcekey_program_handle ];
    assert( gl_program_handle );
    return ::e2::render::gl::gl_helper::print_active_attribs( *gl_program_handle );
}

} }
