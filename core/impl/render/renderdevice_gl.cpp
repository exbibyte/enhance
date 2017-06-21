#include <map>
#include <iostream>
#include <cstdio>
#include <cassert>
#include <cstring>
#include <vector>

#include "i_rendernode.hpp"

#include "gl_includes.hpp"
#include "gl_helper.hpp"
#include "renderdevice_gl.hpp"
#include "rendermap_gl.hpp"

#define debug_info

namespace e2 { namespace render {

bool renderdevice_gl::renderdevice_process_impl( ::e2::interface::i_rendernode_init_window * p ){
#ifdef debug_info
    std::cout << "process init window" << std::endl;
#endif
    if( false == glfwInit() ){
        assert( false && "glfw init failed." );
        return false;
    }
    {
        int dimx = ( int ) p->_x;
	int dimy = ( int ) p->_y;
        glfwWindowHint( GLFW_CONTEXT_VERSION_MAJOR, 3 );
        glfwWindowHint( GLFW_CONTEXT_VERSION_MINOR, 3 );
        glfwWindowHint( GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE );
        _window = glfwCreateWindow( dimx, dimy, "renderdevice_gl window", nullptr, nullptr );
        if( false == _window ){
            assert( false && "window creation failed." );
            return false;
        }

        glfwMakeContextCurrent( _window );

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

bool renderdevice_gl::renderdevice_process_impl( ::e2::interface::i_rendernode_init_object_vertex_array * p ){
    GLsizei * gl_size_va_names = ( GLsizei * ) p->_num_objects;
    assert( gl_size_va_names );
    std::vector< GLuint > gl_va_array( *gl_size_va_names );
    assert( &gl_va_array[0] );
    assert( ::e2::render::gl::gl_helper::generate_vertex_arrays( *gl_size_va_names, &gl_va_array[0] ) );
    for( size_t i = 0; i < *gl_size_va_names; ++i ){
	p->_handle_objects[i] = gl_va_array[i];
    }
    return true;
}

bool renderdevice_gl::renderdevice_process_impl( ::e2::interface::i_rendernode_deinit_window * p ){
#ifdef debug_info
    std::cout << "process deinit window" << std::endl;
#endif
    if( nullptr != _window ){
        glfwDestroyWindow( _window );
        _window = nullptr;
    }
    glfwTerminate();
    return true;
}

bool renderdevice_gl::renderdevice_process_impl( ::e2::interface::i_rendernode_swap_window_buffer * p ){
        assert( _window );
#ifdef debug_info
        std::cout << "process window buf swap" << std::endl;
#endif
        glfwMakeContextCurrent( _window );
        glfwSwapBuffers( _window );
}
    
bool renderdevice_gl::renderdevice_process_impl( ::e2::interface::i_rendernode_clear_window_buffer_colour * p ){
    glClear( GL_COLOR_BUFFER_BIT );
    return true;
}

bool renderdevice_gl::renderdevice_process_impl( ::e2::interface::i_rendernode_clear_window_buffer_depth * p ){
    glClear( GL_DEPTH_BUFFER_BIT );
    return true;
}
bool renderdevice_gl::renderdevice_process_impl( ::e2::interface::i_rendernode_disable_window_buffer_depth * p ){
    glDisable( GL_DEPTH_TEST );
}
bool renderdevice_gl::renderdevice_process_impl( ::e2::interface::i_rendernode_enable_window_buffer_depth * p ){
    glEnable( GL_DEPTH_TEST );
}

bool renderdevice_gl::renderdevice_process_impl( ::e2::interface::i_rendernode_exec_drawbatch * p ){
    uint64_t * primitive_type = ( uint64_t * ) p->_primitive_type;
    int * offset = ( int * ) p->_offset;
    int * count = ( int * ) p->_count;
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

bool renderdevice_gl::renderdevice_process_impl( ::e2::interface::i_rendernode_store_defineformat_vertexattrib * p ){
    uint64_t * va_index = ( uint64_t * ) p->_attrib_handle;
    uint64_t * va_size = ( uint64_t * ) p->_attrib_size;
    uint64_t * va_type = ( uint64_t * ) p->_attrib_data_type;
    bool * va_normalized = ( bool * ) p->_attrib_normalized;
    uint64_t * va_stride = ( uint64_t * ) p->_attrib_stride;
    void * va_pointer = ( void * ) p->_attrib_pointer;
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

bool renderdevice_gl::renderdevice_process_impl( ::e2::interface::i_rendernode_bind_object_vertex_array * p ){
    GLuint * gl_vertex_array_handle = ( GLuint * ) p->_object_handle;
    assert( gl_vertex_array_handle );
    return ::e2::render::gl::gl_helper::bind_vertex_array( *gl_vertex_array_handle );
}

bool renderdevice_gl::renderdevice_process_impl( ::e2::interface::i_rendernode_bind_buffer * p ){
    uint64_t * buffer_type = ( uint64_t * ) p->_buffer_type;
    uint64_t * buffer_handle = ( uint64_t * ) p->_buffer_handle;
    assert( buffer_type );
    assert( buffer_handle );
    GLuint gl_buffer_handle = *buffer_handle;
    auto it = rendermap_gl::_map_render_bind_buffer.find( *buffer_type );
    if( rendermap_gl::_map_render_bind_buffer.end() == it ){
        assert( false && "bind buffer type invalid." );
        return false;
    }
    GLenum gl_buffer_type = it->second;
    // GLenum gl_buffer_type = GL_ARRAY_BUFFER;
    return ::e2::render::gl::gl_helper::bind_buffer( gl_buffer_type, gl_buffer_handle );
}

bool renderdevice_gl::renderdevice_process_impl( ::e2::interface::i_rendernode_enable_attrib_vertex_array * p ){
    uint64_t * va_index = ( uint64_t * ) p->_attrib_handle;
    assert( va_index );
    GLuint gl_va_index = (GLuint) *va_index;
    return ::e2::render::gl::gl_helper::enable_vertex_attrib_array( gl_va_index );
}

bool renderdevice_gl::renderdevice_process_impl( ::e2::interface::i_rendernode_disable_attrib_vertex_array * p ){;
    GLuint * gl_va_index = ( GLuint * ) p->_attrib_handle;
    assert( gl_va_index );
    return ::e2::render::gl::gl_helper::disable_vertex_attrib_array( *gl_va_index );
}

bool renderdevice_gl::renderdevice_process_impl( ::e2::interface::i_rendernode_deinit_object_vertex_array * p ){
    GLsizei * gl_size_va_names = ( GLsizei * ) p->_num_objects;
    assert( gl_size_va_names );
    std::vector< GLuint > gl_va_array( *gl_size_va_names );
    for( size_t i = 0; i < *gl_size_va_names; ++i ){
	gl_va_array[i] = p->_objects[i];
    }
    assert( &gl_va_array[0] );
    return ::e2::render::gl::gl_helper::delete_vertex_arrays( *gl_size_va_names, &gl_va_array[0] );
}

bool renderdevice_gl::renderdevice_process_impl( ::e2::interface::i_rendernode_store_buffer * p ){
    uint64_t * buffer_type = ( uint64_t * ) p->_buffer_type;
    GLsizeiptr * gl_bytes_size = ( GLsizeiptr * ) p->_buffer_size;
    GLvoid * gl_data = ( GLvoid * ) p->_buffer_data;
    GLenum * gl_usage = ( GLenum * ) p->_buffer_usage;
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

bool renderdevice_gl::renderdevice_process_impl( ::e2::interface::i_rendernode_init_buffer * p ){
    uint64_t * num_buffers = ( uint64_t * ) p->_num_buffers;
    uint64_t * buffers = ( uint64_t * ) p->_buffers;
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

bool renderdevice_gl::renderdevice_process_impl( ::e2::interface::i_rendernode_deinit_buffer * p ){
    uint64_t * num_buffers = ( uint64_t * ) p->_num_buffers;
    uint64_t * buffers = ( uint64_t * ) p->_buffers;
    assert( num_buffers );
    assert( buffers );
    GLsizei gl_num_buffers = ( GLsizei ) *num_buffers;
    std::vector< GLuint > gl_buffers( *num_buffers );
    for( size_t i = 0; i < gl_buffers.size(); ++i ){
	gl_buffers[i] = buffers[i];
    }
    return ::e2::render::gl::gl_helper::delete_buffers( gl_num_buffers, &gl_buffers[0] );
}

bool renderdevice_gl::renderdevice_process_impl( ::e2::interface::i_rendernode_bind_attrib_attrib_loc * p ){
    GLuint * gl_program_handle = ( GLuint * ) p->_program_handle;
    GLuint * gl_attrib_index = ( GLuint * ) p->_index;
    GLchar * gl_attrib_name = ( GLchar * ) p->_var_name;
    assert( gl_program_handle );
    assert( gl_attrib_index );
    assert( gl_attrib_name );
    return ::e2::render::gl::gl_helper::bind_attrib_location( *gl_program_handle, *gl_attrib_index, gl_attrib_name );
}

bool renderdevice_gl::renderdevice_process_impl( ::e2::interface::i_rendernode_bind_attrib_frag_loc * p ){
    GLuint * gl_program_handle = ( GLuint * ) p->_program_handle;
    GLuint * gl_frag_location = ( GLuint * ) p->_index;
    char * gl_frag_name = ( char * ) p->_var_name;
    assert( gl_program_handle );
    assert( gl_frag_location );
    assert( gl_frag_name );
    return ::e2::render::gl::gl_helper::bind_frag_data_location( *gl_program_handle, *gl_frag_location, gl_frag_name );
}

bool renderdevice_gl::renderdevice_process_impl( ::e2::interface::i_rendernode_init_program * p ){
    GLuint * gl_program_handle = ( GLuint * ) p->_program_handle;
    assert( gl_program_handle );
    return ::e2::render::gl::gl_helper::create_program( gl_program_handle );
}

bool renderdevice_gl::renderdevice_process_impl( ::e2::interface::i_rendernode_deinit_program * p ){
    GLuint * gl_program_handle = ( GLuint * ) p->_program_handle;
    assert( gl_program_handle );
    return ::e2::render::gl::gl_helper::delete_program( *gl_program_handle );
}

bool renderdevice_gl::renderdevice_process_impl( ::e2::interface::i_rendernode_bind_shader * p ){
    GLuint * gl_program_handle = ( GLuint * ) p->_program_handle;
    GLuint * gl_shader_handle = ( GLuint * ) p->_shader_handle;
    assert( gl_program_handle );
    assert( gl_shader_handle );
    return ::e2::render::gl::gl_helper::attach_shader( *gl_program_handle, *gl_shader_handle );
}

bool renderdevice_gl::renderdevice_process_impl( ::e2::interface::i_rendernode_unbind_shader * p ){
    GLuint * gl_program_handle = ( GLuint * ) p->_program_handle;
    GLuint * gl_shader_handle = ( GLuint * ) p->_shader_handle;
    assert( gl_program_handle );
    assert( gl_shader_handle );
    return ::e2::render::gl::gl_helper::detach_shader( *gl_program_handle, *gl_shader_handle );
}

bool renderdevice_gl::renderdevice_process_impl( ::e2::interface::i_rendernode_compute_program * p ){
    GLuint * gl_program_handle = ( GLuint * ) p->_program_handle;
    assert( gl_program_handle );
    return ::e2::render::gl::gl_helper::link_program( *gl_program_handle );
}

bool renderdevice_gl::renderdevice_process_impl( ::e2::interface::i_rendernode_bind_program * p ){
    GLuint * gl_program_handle = ( GLuint * ) p->_program_handle;
    assert( gl_program_handle );
    return ::e2::render::gl::gl_helper::use_program( *gl_program_handle );
}

bool renderdevice_gl::renderdevice_process_impl( ::e2::interface::i_rendernode_store_persistent_vecf * p ){
    GLuint * gl_program_handle = ( GLuint * ) p->_program_handle;
    uint64_t * dim = ( uint64_t * ) p->_dim;
    char * name = ( char * ) p->_name;
    float * vec = ( float * ) p->_vec;
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
    default:
    break;
    }
    return false;
}
bool renderdevice_gl::renderdevice_process_impl( ::e2::interface::i_rendernode_store_persistent_matf * p ){
    GLuint * gl_program_handle = ( GLuint * ) p->_program_handle;
    uint64_t * dim = ( uint64_t * ) p->_dim;
    char * name = ( char * ) p->_name;
    float * vec = ( float * ) p->_mat;
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
    default:
	break;
    }
    return false;
}

bool renderdevice_gl::renderdevice_process_impl( ::e2::interface::i_rendernode_load_shader * p ){
    GLuint * gl_shader_handle = ( GLuint * ) p->_shader_handle;
    char * shader_source = ( char * ) p->_source;
    uint64_t * shader_type = ( uint64_t * ) p->_shader_type;
    assert( gl_shader_handle );
    assert( shader_source );
    assert( shader_type );
    switch( p->_source_type ){
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
	break;
    }
    return false;
}

bool renderdevice_gl::renderdevice_process_impl( ::e2::interface::i_rendernode_store_program_param * p ){
    GLuint * gl_program_handle = ( GLuint * ) p->_program_handle;
    GLuint * gl_parameter = ( GLuint * ) p->_parameter;
    GLuint * gl_parameter_value = ( GLuint * ) p->_parameter_value;
    assert( gl_program_handle );
    assert( gl_parameter );
    assert( gl_parameter_value );
    return ::e2::render::gl::gl_helper::set_program_parameter( *gl_program_handle, *gl_parameter, *gl_parameter_value );
}

bool renderdevice_gl::renderdevice_process_impl( ::e2::interface::i_rendernode_query_persistent * p ){
    GLuint * gl_program_handle = ( GLuint * ) p->_program_handle;
    assert( gl_program_handle );
    return ::e2::render::gl::gl_helper::print_active_uniforms( *gl_program_handle );
}
bool renderdevice_gl::renderdevice_process_impl( ::e2::interface::i_rendernode_query_attrib * p ){
    GLuint * gl_program_handle = ( GLuint * ) p->_program_handle;
    assert( gl_program_handle );
    return ::e2::render::gl::gl_helper::print_active_attribs( *gl_program_handle );
}

} }
