#include<iostream>
#include<vector>
#include<string>
#include<map>
using namespace std;

#include "GLSLProgram.h"
#include "GLTexture.h"

unsigned int GLSLProgram::_mVertexArrayIndexCount = 0;

GLSLProgram::GLSLProgram(){
    _HandleProgram = glCreateProgram();
    _Linked = false;
    _LogString = "";
    _vHandleShader.clear();
    glGenVertexArrays( 1, &_VertexArrayObj );
}

GLSLProgram::~GLSLProgram(){
    map< string, GLAttribData<float> * >::iterator itAttrib = _MapAttrib.begin();
    while( itAttrib != _MapAttrib.end() ) {
        delete itAttrib->second;
        itAttrib->second = 0;
    }

    map< string, GLTexture * >::iterator itTexture = _MapTexture.begin();
    while( itTexture != _MapTexture.end() ) {
        delete itTexture->second;
        itTexture->second = 0;
    }
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
bool GLSLProgram::SetUniform( string Name, vec3 const & v ){
    return GLSetUniform( _HandleProgram, ( char const * ) Name.c_str(), v );
}
bool GLSLProgram::SetUniform( string Name, vec4 const & v ){
    return GLSetUniform( _HandleProgram, ( char const * ) Name.c_str(), v );
}
bool GLSLProgram::SetUniform( string Name, mat3 const & m ){
    return GLSetUniform( _HandleProgram, ( char const * ) Name.c_str(), m );
}
bool GLSLProgram::SetUniform( string Name, mat4 const & m ){
    return GLSetUniform( _HandleProgram, ( char const * ) Name.c_str(), m );
}
bool GLSLProgram::SetUniform( string Name, float val ){
    return GLSetUniform( _HandleProgram, ( char const * ) Name.c_str(), val );
}
bool GLSLProgram::SetUniform( string Name, int val ){
    return GLSetUniform( _HandleProgram, ( char const * ) Name.c_str(), val );
}
bool GLSLProgram::SetUniform( string Name, bool val ){
    return GLSetUniform( _HandleProgram, ( char const * ) Name.c_str(), val );
}
void GLSLProgram::PrintActiveUniforms() const{
    GLPrintActiveUniforms( _HandleProgram );
}
void GLSLProgram::PrintActiveAttribs() const{
    GLPrintActiveAttribs( _HandleProgram );
}
bool GLSLProgram::AddMapAttrib( string AttribName, GLAttribData<float> * AttribData ){
    bool bRet = true;
    _MapAttrib[ AttribName ] = AttribData;
    return bRet;
}
bool GLSLProgram::GetMapAttrib( string AttribName, GLAttribData<float> * & AttribData ) const {
    bool bRet;
    string key = AttribName;
    map< string, GLAttribData<float> * >::const_iterator it = _MapAttrib.find( key );
    if( it == _MapAttrib.end() ){
        AttribData = 0;
        bRet = false;
    }else{
        AttribData = it->second;
        bRet = true;
    }
    return bRet;
}
void GLSLProgram::BindMapAttrib(){

    for(auto & i : _MapAttrib ){
        int CurrentVertexArrayIndex = _mVertexArrayIndexCount++;
        i.second->SetVertexArrayIndex( _VertexArrayObj, CurrentVertexArrayIndex );
        BindAttribLocation( i.second->GetIndexAttrib(), i.first );
    }

}
void GLSLProgram::BindVertexArray()
{
    GLBindVertexArray( _VertexArrayObj ); 
}
void GLSLProgram::UnBindVertexArray()
{
    GLUnBindVertexArray();
}
bool GLSLProgram::AddMapTexture( string TextureName, GLTexture * Texture ) {
    bool bRet = true;
    _MapTexture[ TextureName ] = Texture;
    return bRet;
}
bool GLSLProgram::GetMapTexture( string TextureName, GLTexture * & Texture ) const {
    bool bRet;
    map< string, GLTexture * >::const_iterator it = _MapTexture.find( TextureName );
    if( it == _MapTexture.end() ){
        Texture = 0;
        bRet = false;
    }else{
        Texture = it->second;
        bRet = true;
    }
    return bRet;
}

bool GLSLProgram::AddNewTexture( string TextureName, GLTexture::eTextureType TextureType, int iWidth, int iHeight, void * const Data, int iActiveTexture ) {
    bool bRet = true;
    GLTexture * Texture = new GLTexture;
    bRet = Texture->SetTexture( TextureType, iWidth, iHeight, Data, iActiveTexture );
    if( bRet ) {
        bRet = AddMapTexture( TextureName, Texture );        
    }
    return bRet;
}
