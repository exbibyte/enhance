#include <cassert>
#include <iostream>
#include <cstdio>

#include "gl_includes.hpp"
#include "gl_helper.hpp"
#include "text_read_write.hpp"

namespace e2 { namespace render { namespace gl {

bool gl_helper::compile_shader_from_string( GLuint * shader, char const * Source, GLSLShaderType type ){
    bool ret;
    if( nullptr == shader ) return false;

    if( GLSLShaderType::VERTEX == type ){
        *shader = glCreateShader(GL_VERTEX_SHADER);
    }else if( GLSLShaderType::FRAGMENT == type ){
        *shader = glCreateShader(GL_FRAGMENT_SHADER);
    }
    else{
        return false;
    }

    if( 0 == *shader ){
	assert( false );
	return false;
    }
    char const * sourceTextConst = Source;
    glShaderSource( *shader, 1, &sourceTextConst, NULL);	
    check_error();

    glCompileShader( *shader );
    check_error();

    GLint result;
    glGetShaderiv( *shader, GL_COMPILE_STATUS, &result );
    check_error();

    if( GL_FALSE == result ){
	std::cerr<< "shader compilation failed!" <<std::endl;
        GLint logLen;
        glGetShaderiv( *shader, GL_INFO_LOG_LENGTH, &logLen );
        if( logLen > 0 ){
            char * log = (char *)malloc(logLen);
            GLsizei written;
            glGetShaderInfoLog( *shader, logLen, &written, log);
            std::cerr << "Shader log: \n" << log <<std::endl;
            free(log);
        }
        ret = false;
    }else{
        ret = true;
    }
    return ret;
}
bool gl_helper::compile_shader_from_file( GLuint * shader, char const * fileName, GLSLShaderType type ){
    bool ret;
    std::vector<char> sourceText;
    ret = ::e2::file::text_read_write::read( fileName, &sourceText );
    if( false == ret ){
	assert( false );
	return false;
    }
    char const * sourceTextConst = &sourceText[0];
    return compile_shader_from_string( shader, sourceTextConst, type );
}
bool gl_helper::create_program( GLuint * program_handle ){
    if( nullptr == program_handle ){
	assert( "gl program handle invalid." );
	return false;
    }
    *program_handle = glCreateProgram();
    if( 0 == *program_handle ){
	assert( "gl program creation failed.");
        return false;
    }
    return true;
}

bool gl_helper::delete_program( GLuint program_handle ){
    glDeleteProgram( program_handle );
    return check_error();
}

bool gl_helper::link_program( GLuint program_handle ){
    glLinkProgram( program_handle );
    check_error();
    GLint status;
    glGetProgramiv( program_handle, GL_LINK_STATUS, &status );
    if( GL_FALSE == status ){
        GLint logLen;
        glGetProgramiv( program_handle, GL_INFO_LOG_LENGTH, &logLen);
        if( logLen > 0 ){
            char * log = (char *)malloc(logLen);
            GLsizei written;
            glGetProgramInfoLog( program_handle, logLen, &written, log);
	    std::cerr<< "Program log: \n" << log <<std::endl;
            free(log);
        }
	assert( false && "gl link program failed." );
        return false;
    }
    return true;
}
bool gl_helper::use_program( GLuint program_handle ){
    glUseProgram( program_handle );
    return check_error();
}

bool gl_helper::print_info(){
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
    return check_error();
}

bool gl_helper::bind_attrib_location( GLuint program_handle, GLuint index, GLchar * attrib_name ){
    glBindAttribLocation( program_handle, index, attrib_name );
    return check_error();
}

bool gl_helper::define_vertex_attrib_data( GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, GLvoid * pointer ){
    glVertexAttribPointer( index, size, type, normalized, stride, pointer );
    return check_error();
}
	    
bool gl_helper::bind_frag_data_location( GLuint program_handle, GLuint color_number, char const * name ){
    glBindFragDataLocation( program_handle, color_number, name );
    return check_error();
}

bool gl_helper::set_uniform_i1( GLuint Program, char const * Name, int * v ){
    GLint location = glGetUniformLocation( Program, (GLchar const * ) Name );
    if( location >= 0 ){
        glUniform1iv( location, 1, v );
	return check_error();
    }
    else{
	assert( false );
        return false;
    }
}

bool gl_helper::set_uniform_i2( GLuint Program, char const * Name, int * v ){
    GLint location = glGetUniformLocation( Program, (GLchar const * ) Name );
    if( location >= 0 ){
        glUniform2iv( location, 1, v );
	return check_error();
    }
    else{
	assert( false );
        return false;
    }
}

bool gl_helper::set_uniform_i3( GLuint Program, char const * Name, int * v ){
    GLint location = glGetUniformLocation( Program, (GLchar const * ) Name );
    if( location >= 0 ){
        glUniform3iv( location, 1, v );
	return check_error();
    }
    else{
	assert( false );
        return false;
    }
}

bool gl_helper::set_uniform_i4( GLuint Program, char const * Name, int * v ){
    GLint location = glGetUniformLocation( Program, (GLchar const * ) Name );
    if( location >= 0 ){
        glUniform4iv( location, 1, v );
	return check_error();
    }
    else{
	assert( false );
        return false;
    }
}

bool gl_helper::set_uniform_f1( GLuint Program, char const * Name, float * v ){
    GLint location = glGetUniformLocation( Program, (GLchar const * ) Name );
    if( location >= 0 ){
        glUniform1fv( location, 1, v );
	return check_error();
    }
    else{
	assert( false );
        return false;
    }
}

bool gl_helper::set_uniform_f2( GLuint Program, char const * Name, float * v ){
    GLint location = glGetUniformLocation( Program, (GLchar const * ) Name );
    if( location >= 0 ){
        glUniform2fv( location, 1, v );
	return check_error();
    }
    else{
	assert( false );
        return false;
    }
}

bool gl_helper::set_uniform_f3( GLuint Program, char const * Name, float * v ){
    GLint location = glGetUniformLocation( Program, (GLchar const * ) Name );
    if( location >= 0 ){
        glUniform3fv( location, 1, v );
	return check_error();
    }
    else{
	assert( false );
        return false;
    }
}

bool gl_helper::set_uniform_f4( GLuint Program, char const * Name, float * v ){
    GLint location = glGetUniformLocation( Program, (GLchar const * ) Name );
    if( location >= 0 ){
        glUniform4fv( location, 1, v );
	return check_error();
    }
    else{
	assert( false );
        return false;
    }
}

bool gl_helper::set_uniform_matf2( GLuint Program, char const * Name, float * m ){
    GLint location = glGetUniformLocation( Program, (GLchar const * ) Name );
    if( location >= 0 ){
        glUniformMatrix2fv( location, 1, GL_FALSE, m );
	return check_error();
    }
    else{
	assert( false );
        return false;
    }
}

bool gl_helper::set_uniform_matf3( GLuint Program, char const * Name, float * m ){
    GLint location = glGetUniformLocation( Program, (GLchar const * ) Name );
    if( location >= 0 ){
        glUniformMatrix3fv( location, 1, GL_FALSE, m );
	return check_error();
    }
    else{
	assert( false );
        return false;
    }
}

bool gl_helper::set_uniform_matf4( GLuint Program, char const * Name, float * m ){
    GLint location = glGetUniformLocation( Program, (GLchar const * ) Name );
    if( location >= 0 ){
        glUniformMatrix4fv( location, 1, GL_FALSE, m );
        return true;
    }
    else{
        return false;
    }
}

bool gl_helper::print_active_uniforms( GLuint Program )
{
    GLint nUniforms, maxLen;
    GLenum err = glGetError();
    glGetProgramiv( Program, GL_ACTIVE_UNIFORM_MAX_LENGTH, &maxLen );
    err = glGetError();
    if( GL_NO_ERROR != err ){
	assert( 0 && "GL error" );
    }
    glGetProgramiv( Program, GL_ACTIVE_UNIFORMS, &nUniforms);
    GLchar * name = (GLchar *) malloc( maxLen );
    GLint size, location;
    GLsizei written;
    GLenum type;
    std::cout << "Number of uniforms: " << nUniforms << std::endl;
    printf(" Location | Name\n");
    printf("------------------------------------------------\n");
    for( int i = 0; i < nUniforms; ++i ) {
        glGetActiveUniform( Program, i, maxLen, &written,
                            &size, &type, name );
        location = glGetUniformLocation( Program, name);
        printf(" %-8d | %s\n", location, name);
    }
    free(name);
    return check_error();
}

bool gl_helper::print_active_attribs( GLuint Program )
{
    GLint maxLength, nAttribs;
    glGetProgramiv(Program, GL_ACTIVE_ATTRIBUTES, &nAttribs);
    glGetProgramiv(Program, GL_ACTIVE_ATTRIBUTE_MAX_LENGTH, &maxLength);
    GLchar * name = (GLchar *) malloc( maxLength );
    GLint written, size, location;
    GLenum type;
    printf(" Index | Name\n");
    printf("------------------------------------------------\n");
    for( int i = 0; i < nAttribs; i++ ) {
        glGetActiveAttrib( Program, i, maxLength, &written,
                           &size, &type, name );
        location = glGetAttribLocation(Program, name);
        printf(" %-5d | %s\n",location, name);
    }
    free(name);
    return check_error();
}

bool gl_helper::gen_textures( int Size, GLuint * TexNames )
{
    if( Size <= 0 ){
        return false;
    }
    else{
        glGenTextures( Size, TexNames );
	return check_error();
    }
}

bool gl_helper::set_texture( int GLTextureUnitOffset, GLuint TexName, unsigned char const * TexData, int Width, int Height )
{
    glBindTexture( GL_TEXTURE_2D, TexName );
    glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, Width,
                  Height, 0, GL_RGBA, GL_UNSIGNED_BYTE,
                  TexData );
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,
                    GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                    GL_LINEAR);
    return check_error();
}

bool gl_helper::bind_buffer( GLenum buffertype, GLuint bufferhandle ){
    glBindBuffer( buffertype, bufferhandle );
    return check_error();
}
	    
bool gl_helper::bind_vertex_array( GLuint vbo )
{
    glBindVertexArray( vbo );
    return check_error();
}
bool gl_helper::unbind_vertex_array()
{
    glBindVertexArray( 0 );
    return check_error();
}

bool gl_helper::uniform_subroutinesuiv( GLenum ShaderType, GLsizei Count, GLuint const * Indices )
{
    glUniformSubroutinesuiv( ShaderType, Count, Indices );
    return check_error();
}

bool gl_helper::get_subroutine_uniform_location( GLuint Program, GLenum Shadertype, GLchar const * Name, GLint * ret )
{
    *ret = glGetSubroutineUniformLocation( Program, Shadertype, Name );
    if( GL_INVALID_ENUM == *ret ||
	GL_INVALID_VALUE == *ret ){
	assert( false );
	return false;
    }
    return true;
}

bool gl_helper::get_subroutine_index(GLuint program, GLenum shadertype, const GLchar * name, GLuint * ret )
{
    *ret = glGetSubroutineIndex( program, shadertype, name );
    if( GL_INVALID_ENUM == *ret ||
	GL_INVALID_VALUE == *ret ){
	assert( false );
	return false;
    }
    return true;
}

bool gl_helper::check_error(){
    GLenum ret = glGetError();
    if( GL_NO_ERROR != ret ){	
	assert(false);
	return false;
    }
    return true;
}

bool gl_helper::enable_vertex_attrib_array( GLuint index ){
    glEnableVertexAttribArray( index );
    return check_error();
}

bool gl_helper::disable_vertex_attrib_array( GLuint index ){
    glDisableVertexAttribArray( index );
    return check_error();
}

bool gl_helper::generate_vertex_arrays( GLsizei n, GLuint * arrays ){
    glGenVertexArrays( n, arrays );
    return check_error();
}

bool gl_helper::delete_vertex_arrays( GLsizei n, GLuint * arrays ){
    glDeleteVertexArrays( n, arrays );
    return check_error();
}

bool gl_helper::store_buffer_data( GLenum target, GLsizeiptr size, GLvoid * data, GLenum usage ){
    glBufferData( target, size, data, usage );
    return check_error();
}

bool gl_helper::generate_buffers( GLsizei n, GLuint * buffers ){
    glGenBuffers( n, buffers );
    return check_error();
}

bool gl_helper::delete_buffers( GLsizei n, GLuint * buffers ){
    glDeleteBuffers( n, buffers );
    return check_error();
}

bool gl_helper::attach_shader( GLuint program_handle, GLuint shader_handle ){
    glAttachShader( program_handle, shader_handle );
    return check_error();
}
	    
bool gl_helper::detach_shader( GLuint program_handle, GLuint shader_handle ){
    glDetachShader( program_handle, shader_handle );
    return check_error();
}
	    
} } }
