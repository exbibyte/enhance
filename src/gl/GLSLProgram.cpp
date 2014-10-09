#include "GLSLProgram.h"
#include "GLHelper.h"

GLSLProgram::GLSLProgram(){
    _Handle = 0;
    _Linked = false;
    _LogString = "";
}

GLuint GLSLProgram::GetHandle() const{
    return _Handle;
}

bool GLSLProgram::IsLinked() const{
    return _Linked;
}
bool GLSLProgram::Link(){
    return GLLinkProgram( _Handle );
}

bool GLSLProgram::CompileShaderFromFile( char const * FileName, GLSLShader::GLSLShaderType Type ){
    return GLCompileShaderFromFile( _Handle, FileName, Type );
}

bool GLSLProgram::CompileShaderFromString( string const & Source, GLSLShader::GLSLShaderType Type ){

}

void GLSLProgram::Use(){
    GLUseProgram( _Handle );
}

string GLSLProgram::Log() const{

}

void GLSLProgram::BindAttribLocation( GLuint Loc, char const * Name ){}
void GLSLProgram::BindFragDataLocation( GLuint Loc, char const * Name ){}
void GLSLProgram::SetUniform( char const * Name, float x, float y, float z ){}
void GLSLProgram::SetUniform( char const * Name, vec3 const & v ){}
void GLSLProgram::SetUniform( char const * Name, vec4 const & v ){}
void GLSLProgram::SetUniform( char const * Name, mat3 const & m ){}
void GLSLProgram::SetUniform( char const * Name, mat4 const & m ){}
void GLSLProgram::SetUniform( char const * Name, float val ){}
void GLSLProgram::SetUniform( char const * Name, int val ){}
void GLSLProgram::SetUniform( char const * Name, bool val ){}
void GLSLProgram::PrintActiveUniforms() const{}
void GLSLProgram::PrintActiveAttribs() const{}
