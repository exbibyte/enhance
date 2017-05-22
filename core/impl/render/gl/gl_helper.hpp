#ifndef E2_GLHELPER_HPP
#define E2_GLHELPER_HPP

#include "gl_includes.hpp"

namespace e2 { namespace render { namespace gl {

enum GLSLShaderType {
    VERTEX = 1, FRAGMENT, GEOMETRY,TESS_CONTROL, TESS_EVALUATION
};

class gl_helper {
public:
    static bool compile_shader_from_string( GLuint * shader, char const * Source, GLSLShaderType type );
    static bool compile_shader_from_file( GLuint * shader, char const * fileName, GLSLShaderType type );

    static bool create_program( GLuint * program_handle );
    static bool delete_program( GLuint program_handle );
    static bool attach_shader( GLuint program_handle, GLuint shader_handle );
    static bool detach_shader( GLuint program_handle, GLuint shader_handle );
    static bool link_program( GLuint program );
    static bool use_program( GLuint program );
    static bool set_program_parameter( GLuint program_handle, GLenum parameter, GLint value );
    static bool print_info();
    static bool check_error();

    static bool bind_attrib_location( GLuint program_handle, GLuint index, GLchar * attrib_name );
    static bool define_vertex_attrib_data( GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, GLvoid * pointer );
    static bool enable_vertex_attrib_array( GLuint index );
    static bool disable_vertex_attrib_array( GLuint index );
    static bool generate_vertex_arrays( GLsizei n, GLuint * arrays );
    static bool delete_vertex_arrays( GLsizei n, GLuint * arrays );
    static bool store_buffer_data( GLenum target, GLsizeiptr size, GLvoid * data, GLenum usage );
    static bool generate_buffers( GLsizei n, GLuint * buffers );
    static bool delete_buffers( GLsizei n, GLuint * buffers );
    static bool bind_frag_data_location( GLuint Program, GLuint Loc, char const * Name );

    static bool set_uniform_i1( GLuint Program, char const * Name, int * v );
    static bool set_uniform_i2( GLuint Program, char const * Name, int * v );
    static bool set_uniform_i3( GLuint Program, char const * Name, int * v );
    static bool set_uniform_i4( GLuint Program, char const * Name, int * v );
    static bool set_uniform_f1( GLuint Program, char const * Name, float * v );
    static bool set_uniform_f2( GLuint Program, char const * Name, float * v );
    static bool set_uniform_f3( GLuint Program, char const * Name, float * v );
    static bool set_uniform_f4( GLuint Program, char const * Name, float * v );
    static bool set_uniform_matf2( GLuint Program, char const * Name, float * m );
    static bool set_uniform_matf3( GLuint Program, char const * Name, float * m );
    static bool set_uniform_matf4( GLuint Program, char const * Name, float * m );
    static bool print_active_uniforms( GLuint Program );
    static bool print_active_attribs( GLuint Program );

    static bool gen_textures( int Size, GLuint * TexNames );
    static bool set_texture( int GLTextureUnitOffset, GLuint TexName, unsigned char const * TexData, int Width, int Height );
    static bool bind_buffer( GLenum buffertype, GLuint bufferhandle );
    static bool bind_vertex_array( GLuint vbo );
    static bool unbind_vertex_array();

    static bool uniform_subroutinesuiv( GLenum ShaderType, GLsizei Count, GLuint const * Indices ); //loads the active subroutine uniforms
    static bool get_subroutine_uniform_location( GLuint Program, GLenum Shadertype, GLchar const * Name, GLint * ret ); //get subroutine uniform location in a program's shader stage
    static bool get_subroutine_index(GLuint program, GLenum shadertype, const GLchar * name, GLuint * ret ); //get index of a subroutine uniform in a program's shader stage
};

} } }

#endif
