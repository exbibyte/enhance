#ifndef E2_GLHELPER_HPP
#define E2_GLHELPER_HPP

#include "gl_includes.hpp"

namespace e2 { namespace render { namespace gl {

enum class GLSLShaderType {
    VERTEX, FRAGMENT, GEOMETRY,TESS_CONTROL, TESS_EVALUATION
};

class render_helper_gl {
public:
    static bool compile_shader_from_string( GLuint * shader, char const * Source, GLSLShaderType type );
    static bool compile_shader_from_file( GLuint * shader, char const * fileName, GLSLShaderType type );
    static bool create_program( GLuint & program );
    static bool link_program( GLuint & program );
    static bool use_program( GLuint & program );
    static bool print_info();
    static bool check_error();

    static bool bind_attrib_location( GLuint Program, GLuint Loc, char const * Name );
    static bool bind_frag_data_location( GLuint Program, GLuint Loc, char const * Name );
    static bool set_uniform_vec3( GLuint Program, char const * Name, float * v );
    static bool set_uniform_vec4( GLuint Program, char const * Name, float * v );
    static bool set_uniform_mat3( GLuint Program, char const * Name, float * m );
    static bool set_uniform_mat4( GLuint Program, char const * Name, float * m );
    static bool set_uniform_single_f( GLuint Program, char const * Name, float val );
    static bool set_uniform_single_i( GLuint Program, char const * Name, int val );
    static bool set_uniform_single_b( GLuint Program, char const * Name, bool val );
    static bool print_active_uniforms( GLuint Program );
    static bool print_active_attribs( GLuint Program );

    static bool gen_textures( int Size, GLuint * TexNames );
    static bool set_texture( int GLTextureUnitOffset, GLuint TexName, unsigned char const * TexData, int Width, int Height );
    static bool bind_vertex_array( GLuint vbo );
    static bool unbind_vertex_array();

    static bool uniform_subroutinesuiv( GLenum ShaderType, GLsizei Count, GLuint const * Indices ); //loads the active subroutine uniforms
    static bool get_subroutine_uniform_location( GLuint Program, GLenum Shadertype, GLchar const * Name, GLint * ret ); //get subroutine uniform location in a program's shader stage
    static bool get_subroutine_index(GLuint program, GLenum shadertype, const GLchar * name, GLuint * ret ); //get index of a subroutine uniform in a program's shader stage
};

} } }

#endif
