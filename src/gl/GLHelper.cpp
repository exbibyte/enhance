#include "GLHelper.h"

#include <GLFW/glfw3.h>
#include <OpenGL/gl3.h>

///////////
bool GLCompileShaderFromString( GLuint & shader, char const * Source, GLSLShader::GLSLShaderType type ){
    bool bRet;

    if( GLSLShader::VERTEX == type ){
        shader = glCreateShader(GL_VERTEX_SHADER);
    }else if( GLSLShader::FRAGMENT == type ){
        shader = glCreateShader(GL_FRAGMENT_SHADER);
    }
    else{
        return false;
    }
    char const * sourceTextConst = Source;
    glShaderSource( shader, 1, &sourceTextConst, NULL);	
    
    glCompileShader(shader);
    GLint result;
    glGetShaderiv( shader, GL_COMPILE_STATUS, &result );
    if( GL_FALSE == result ){
        cerr<< "shader compilation failed!" <<endl;
        GLint logLen;
        glGetShaderiv( shader, GL_INFO_LOG_LENGTH, &logLen );
        if( logLen > 0 ){
            char * log = (char *)malloc(logLen);
            GLsizei written;
            glGetShaderInfoLog( shader, logLen, &written, log);
            cerr << "Shader log: \n" << log <<endl;
            free(log);
        }
        bRet = false;
    }else{
        bRet = true;
    }
    return bRet;
}

bool GLCompileShaderFromFile( GLuint & shader, char const * fileName, GLSLShader::GLSLShaderType type ){
    bool bRet;
    char * sourceText = 0;
    sourceText = textFileRead( fileName );
    char const * sourceTextConst = sourceText;
    bRet = GLCompileShaderFromString( shader, sourceTextConst, type );
    free(sourceText);
    return bRet;
}

bool GLCreateProgram( GLuint & program ){
    program = glCreateProgram();
    if( 0 == program ){
        cerr<< "Error creating program object" <<endl;
        return false;
    }else{
        return true;
    }
}

bool GLLinkProgram( GLuint & program ){
    glLinkProgram(program);
    GLint status;
    glGetProgramiv( program, GL_LINK_STATUS, &status );
    if( GL_FALSE == status ){
        cerr << "Failed to link shader program!" <<endl;
        GLint logLen;
        glGetProgramiv( program, GL_INFO_LOG_LENGTH, &logLen);
        if( logLen > 0 ){
            char * log = (char *)malloc(logLen);
            GLsizei written;
            glGetProgramInfoLog( program, logLen, &written, log);
            cerr<< "Program log: \n" << log <<endl;
            free(log);
        }
        return false;
    }else{
        return true;
    }
}
bool GLLinkUseProgram( GLuint & program ){
    if( true == GLLinkProgram( program ) ){
        glUseProgram(program);
        return true;
    }else{
        return false;
    }
}

void GLUseProgram( GLuint & program ){
    glUseProgram(program);
}

void GLPrintInfo(){
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

void GLBindAttribLocation( GLuint Program, GLuint Loc, char const * Name ){
    glBindAttribLocation( Program, Loc, (GLchar const * ) Name );
}

void GLBindFragDataLocation( GLuint Program, GLuint Loc, char const * Name ){
    glBindFragDataLocation( Program, Loc, (GLchar const * ) Name );
}

bool GLSetUniform( GLuint Program, char const * Name, vec3 const & v ){
    GLint location = glGetUniformLocation( Program, (GLchar const * ) Name );
    if( location >= 0 ){
        glUniform3fv( location, 1, &v[0]);
        return true;
    }
    else{
        return false;
    }
}

bool GLSetUniform( GLuint Program, char const * Name, vec4 const & v ){
    GLint location = glGetUniformLocation( Program, (GLchar const * ) Name );
    if( location >= 0 ){
        glUniform4fv( location, 1, &v[0]);
        return true;
    }
    else{
        return false;
    }
}
bool GLSetUniform( GLuint Program, char const * Name, mat3 const & m ){
    GLint location = glGetUniformLocation( Program, (GLchar const * ) Name );
    if( location >= 0 ){
        glUniformMatrix3fv( location, 1, GL_FALSE, &m[0][0]);
        return true;
    }
    else{
        return false;
    }
}
bool GLSetUniform( GLuint Program, char const * Name, mat4 const & m ){
    GLint location = glGetUniformLocation( Program, (GLchar const * ) Name );
    if( location >= 0 ){
        glUniformMatrix4fv( location, 1, GL_FALSE, &m[0][0]);
        return true;
    }
    else{
        return false;
    }
}

bool GLSetUniform( GLuint Program, char const * Name, float val ){
    GLint location = glGetUniformLocation( Program, (GLchar const * ) Name );
    if( location >= 0 ){
        glUniform1f( location, val );
        return true;
    }
    else{
        return false;
    }
}
bool GLSetUniform( GLuint Program, char const * Name, int val ){
    GLint location = glGetUniformLocation( Program, (GLchar const * ) Name );
    if( location >= 0 ){
        glUniform1i( location, val );
        return true;
    }
    else{
        return false;
    }
}
bool GLSetUniform( GLuint Program, char const * Name, bool val ){
    
    int ValInt;
    if(val){
        ValInt = 1;
    }else{
        ValInt = 0;
    }

    GLint location = glGetUniformLocation( Program, (GLchar const * ) Name );
    if( location >= 0 ){
        glUniform1i( location, ValInt );
        return true;
    }
    else{
        return false;
    }
}

void GLPrintActiveUniforms( GLuint Program )
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
    cout << "Number of uniforms: " << nUniforms << endl;
    printf(" Location | Name\n");
    printf("------------------------------------------------\n");
    for( int i = 0; i < nUniforms; ++i ) {
        glGetActiveUniform( Program, i, maxLen, &written,
                            &size, &type, name );
        location = glGetUniformLocation( Program, name);
        printf(" %-8d | %s\n", location, name);
    }
    free(name);
}

void GLPrintActiveAttribs( GLuint Program )
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
}

bool GLGenTextures( int Size, GLuint * TexNames )
{
    if( Size <= 0 ){
        return false;
    }
    else{
        glGenTextures( Size, TexNames );
        return true;
    }
}

bool GLSetTexture( int GLTextureUnitOffset, GLuint TexName, unsigned char const * TexData, int Width, int Height )
{
    glBindTexture( GL_TEXTURE_2D, TexName );
    glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, Width,
                  Height, 0, GL_RGBA, GL_UNSIGNED_BYTE,
                  TexData );
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,
                    GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                    GL_LINEAR);
    return true;
}

void GLBindVertexArray( GLuint vbo )
{
    glBindVertexArray( vbo ); 
}
void GLUnBindVertexArray()
{
    glBindVertexArray( 0 ); 
}

void GLUniformSubroutinesuiv( GLenum ShaderType, GLsizei Count, GLuint const * Indices )
{
    glUniformSubroutinesuiv( ShaderType, Count, Indices );
}

GLint GLGetSubroutineUniformLocation( GLuint Program, GLenum Shadertype, GLchar const * Name )
{
    return glGetSubroutineUniformLocation( Program, Shadertype, Name );
}

GLuint GLGetSubroutineIndex(GLuint program, GLenum shadertype, const GLchar * name )
{
    return glGetSubroutineIndex( program, shadertype, name );
}
