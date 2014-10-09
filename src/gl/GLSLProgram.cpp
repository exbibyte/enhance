#include<iostream>
#include<vector>
#include<string>
using namespace std;

#include "GLSLProgram.h"
#include "GLHelper.h"

GLSLProgram::GLSLProgram(){
    _HandleProgram = 0;
    _Linked = false;
    _LogString = "";
    _vHandleShader.clear();
}

GLuint GLSLProgram::GetHandle() const{
    return _HandleProgram;
}

bool GLSLProgram::IsLinked() const{
    return _Linked;
}
bool GLSLProgram::Link(){
    return GLLinkProgram( _HandleProgram );
}

bool GLSLProgram::CompileShaderFromFile( string FileName, GLSLShader::GLSLShaderType Type ){
    GLuint Shader;
    char const * FileNameConst = FileName.c_str();
    bool bRet = GLCompileShaderFromFile( Shader, FileNameConst, Type );
    if( bRet ){
        _vHandleShader.push_back( Shader );
    }
    return bRet;
}

bool GLSLProgram::CompileShaderFromString( string const & Source, GLSLShader::GLSLShaderType Type ){
    GLuint Shader;
    char const * SourceConst = Source.c_str();
    bool bRet = GLCompileShaderFromString( Shader, SourceConst, Type );
    if( bRet ){
        _vHandleShader.push_back( Shader );
    }
    return bRet;
}

void GLSLProgram::Use(){
    GLUseProgram( _HandleProgram );
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
