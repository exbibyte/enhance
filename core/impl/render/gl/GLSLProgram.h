#ifndef E2_GLSLPROGRAM_HPP
#define E2_GLSLPROGRAM_HPP

#include "gl_includes.hpp"
#include "Mat.hpp"
#include "Vec.hpp"

#include<iostream>
#include<vector>
#include<string>
#include<map>
#include<utility>
using namespace std;

#include "gl_helper.hpp"
#include "GLAttribData.h"
#include "gl_texture.hpp"
#include "GLBufferInfo.h"

class GLBufferInfo;

class GLSLProgram{

public:
    GLSLProgram();
    ~GLSLProgram();
    GLuint GetHandle() const;
    bool IsLinked() const;
    void AttachShaders();
    bool Link();
    void Use();
    bool AddMapAttrib( string AttribName, GLAttribData<float> * AttribData );
    bool GetMapAttrib( string AttribName, GLAttribData<float> * & AttribData ) const;
    bool DeallocateMapAttrib( string AttribName );
    void BindMapAttrib(); //binds all attributes in map
    bool AddMapTexture( string TextureName, GLTexture * Texture );
    bool GetMapTexture( string TextureName, GLTexture * & Texture ) const;
    bool AddNewTexture( string TextureName, GLTexture::eTextureType TextureType, int iWidth, int iHeight, void * const Data, int iActiveTexture );
    bool SetBufferInfo( GLBufferInfo * BufferInfo );
    bool GetBufferInfo( string strName, GLBufferInfo * & BufferInfo );
    bool RemoveBufferInfo( string strName );
    bool SetCurrentBufferInfo( string strName );
    bool DrawCurrentBufferSegment();
private:
    GLuint _HandleProgram;
    bool _Linked;
    vector< GLuint > _vHandleShader;
    GLuint _VertexArrayObj; //holding handles to individual VBO associated with GLAttribData
    map< string, GLAttribData<float> * > _MapAttrib; //mapping of attribute name and attribute index for this shader program
    static unsigned int _mVertexArrayIndexCount;
    map< string, GLTexture * > _MapTexture; //texture storage
    map< string, GLBufferInfo * > _MapBufferInfo; // information of buffer segments
    GLBufferInfo * _CurrentBufferInfo;
    GLBufferInfoSequence * _CurrentBufferInfoSequence;
};

#endif
