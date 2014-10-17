#include<iostream>
#include<vector>
#include<string>
#include<map>
using namespace std;

#include "GLSLProgram.h"

GLSLProgram::GLSLProgram(){
    _HandleProgram = glCreateProgram();
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
void GLSLProgram::AttachShaders(){
    for( auto i : _vHandleShader ){
        glAttachShader( _HandleProgram, i );
    }
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

void GLSLProgram::BindAttribLocation( GLuint Loc, string Name ){
    GLBindAttribLocation( _HandleProgram, Loc, ( char const * ) Name.c_str() );
}
void GLSLProgram::BindFragDataLocation( GLuint Loc, string Name ){
    GLBindFragDataLocation( _HandleProgram, Loc, ( char const * ) Name.c_str() );
}
void GLSLProgram::SetUniform( string Name, vec3 const & v ){
    GLSetUniform( _HandleProgram, ( char const * ) Name.c_str(), v );
}
void GLSLProgram::SetUniform( string Name, vec4 const & v ){
    GLSetUniform( _HandleProgram, ( char const * ) Name.c_str(), v );
}
void GLSLProgram::SetUniform( string Name, mat3 const & m ){
    GLSetUniform( _HandleProgram, ( char const * ) Name.c_str(), m );
}
void GLSLProgram::SetUniform( string Name, mat4 const & m ){
    GLSetUniform( _HandleProgram, ( char const * ) Name.c_str(), m );
}
void GLSLProgram::SetUniform( string Name, float val ){
    GLSetUniform( _HandleProgram, ( char const * ) Name.c_str(), val );
}
void GLSLProgram::SetUniform( string Name, int val ){
    GLSetUniform( _HandleProgram, ( char const * ) Name.c_str(), val );
}
void GLSLProgram::SetUniform( string Name, bool val ){
    GLSetUniform( _HandleProgram, ( char const * ) Name.c_str(), val );
}
void GLSLProgram::PrintActiveUniforms() const{
    GLPrintActiveUniforms( _HandleProgram );
}
void GLSLProgram::PrintActiveAttribs() const{
    GLPrintActiveAttribs( _HandleProgram );
}
void GLSLProgram::AddMapAttrib( string AttribName, GLAttribData<float> * AttribData ){
    _MapAttrib[ AttribName ] = AttribData;
}
void GLSLProgram::GetMapAttrib( string AttribName, GLAttribData<float> * & AttribData ) const {
    string key = AttribName;
    map< string, GLAttribData<float> * >::const_iterator it = _MapAttrib.find( key );
    if( it == _MapAttrib.end() ){
        AttribData = 0;
    }else{
        AttribData = it->second;
    }
}
void GLSLProgram::BindMapAttrib(){
    for(auto & i : _MapAttrib ){
        BindAttribLocation( i.second->GetIndexAttrib(), i.first );
    }
}
