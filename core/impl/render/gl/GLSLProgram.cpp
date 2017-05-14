#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <cassert>

#include "gl_includes.hpp"
#include "GLSLProgram.h"
#include "gl_texture.hpp"
#include "GLBufferInfo.h"
#include "Mat.hpp"
#include "Vec.hpp"

using namespace std;

unsigned int GLSLProgram::_mVertexArrayIndexCount = 0;

GLSLProgram::GLSLProgram(){
    _HandleProgram = glCreateProgram();
    _Linked = false;
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
void GLSLProgram::Use(){
    GLUseProgram( _HandleProgram );
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
bool GLSLProgram::DeallocateMapAttrib( string AttribName ){
    GLAttribData<float> * AttribData;
    if( !GetMapAttrib( AttribName, AttribData ) )
    {
	return false;
    }else{
	delete [] AttribData;
	_MapAttrib.erase( AttribName );
    }
    return true;
}
void GLSLProgram::BindMapAttrib(){
    for(auto & i : _MapAttrib ){
        int CurrentVertexArrayIndex = _mVertexArrayIndexCount++;
        i.second->SetVertexArrayIndex( _VertexArrayObj, CurrentVertexArrayIndex );
        BindAttribLocation( i.second->GetIndexAttrib(), i.first );
    }
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
    gl_texture * Texture = new GLTexture;
    bRet = Texture->set_texture( TextureType, iWidth, iHeight, Data, iActiveTexture );
    if( bRet ) {
        bRet = AddMapTexture( TextureName, Texture );        
    }
    return bRet;
}
bool GLSLProgram::SetBufferInfo( GLBufferInfo * BufferInfo ){
    if( !BufferInfo ){
	assert( 0 && "GLSLProgram::SetBufferInfo failed" );
	return false;
    }
    _MapBufferInfo[ BufferInfo->_Name ] = BufferInfo;
    return true;
}
bool GLSLProgram::GetBufferInfo( string strName, GLBufferInfo * & BufferInfo ){
    auto it = _MapBufferInfo.find( strName );
    if( _MapBufferInfo.end() == it ){
	return false;
    }
    BufferInfo = it->second;
    return true;
}
bool GLSLProgram::RemoveBufferInfo( string strName ){
    auto it = _MapBufferInfo.find( strName );
    if( _MapBufferInfo.end() == it ){
	return false;
    }
    _MapBufferInfo.erase( it );
    return true;
}
bool GLSLProgram::SetCurrentBufferInfo( string strName ){
    GLBufferInfo * BufferInfo;
    if( !GetBufferInfo( strName, BufferInfo ) ){
	return false;
    }
    _CurrentBufferInfo = BufferInfo;
    return true;
}
bool GLSLProgram::DrawCurrentBufferSegment(){
    if( !_CurrentBufferInfo ){
	return false;
    }
    glDrawArrays( GL_TRIANGLES, _CurrentBufferInfo->_Offset, _CurrentBufferInfo->_Length );
    return true;
}
