#ifndef GLSLPROGRAM_H
#define GLSLPROGRAM_H

#include "GLIncludes.hpp"
#include "Mat.hpp"
#include "Vec.hpp"

#include<iostream>
#include<vector>
#include<string>
#include<map>
#include<utility>
using namespace std;

#include "GLHelper.h"
#include "GLAttribData.h"
#include "GLTexture.h"
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
    bool CompileShaderFromFile( std::string FileName, GLSLShader::GLSLShaderType Type );
    bool CompileShaderFromString( std::string const & Source, GLSLShader::GLSLShaderType Type );
    void Use();
    string Log() const;
    void BindAttribLocation( GLuint Loc, std::string Name );
    void BindFragDataLocation( GLuint Loc, std::string Name );
    bool SetUniform( std::string Name, float x, float y, float z );
    bool SetUniformVec3( std::string Name, Vec const & v );
    bool SetUniformVec4( std::string Name, Vec const & v );
    bool SetUniformMat3( std::string Name, Mat const & m );
    bool SetUniformMat4( std::string Name, Mat const & m );
    bool SetUniform( std::string Name, float val );
    bool SetUniform( std::string Name, int val );
    bool SetUniform( std::string Name, bool val );
    void PrintActiveUniforms() const;
    void PrintActiveAttribs() const;
    bool AddMapAttrib( string AttribName, GLAttribData<float> * AttribData );
    bool GetMapAttrib( string AttribName, GLAttribData<float> * & AttribData ) const;
    bool DeallocateMapAttrib( string AttribName );
    void BindMapAttrib(); //binds all attributes in map
    void BindVertexArray();
    void UnBindVertexArray();
    bool AddMapTexture( string TextureName, GLTexture * Texture );
    bool GetMapTexture( string TextureName, GLTexture * & Texture ) const;
    bool AddNewTexture( string TextureName, GLTexture::eTextureType TextureType, int iWidth, int iHeight, void * const Data, int iActiveTexture );
    bool SetBufferInfo( GLBufferInfo * BufferInfo );
    bool GetBufferInfo( string strName, GLBufferInfo * & BufferInfo );
    bool SetBufferInfoSequence( GLBufferInfoSequence * BufferInfoSequence );
    bool GetBufferInfoSequence( string strName, GLBufferInfoSequence * & BufferInfoSequence );
    bool RemoveBufferInfo( string strName );
    bool SetCurrentBufferInfo( string strName );
    bool SetCurrentBufferInfoSequence( string strName );
    bool DrawCurrentBufferSegment();
    bool DrawCurrentBufferSequence( bool bIncrement );
private:
    GLuint _HandleProgram;
    bool _Linked;
    string _LogString;
    vector< GLuint > _vHandleShader;
    GLuint _VertexArrayObj; //holding handles to individual VBO associated with GLAttribData
    map< string, GLAttribData<float> * > _MapAttrib; //mapping of attribute name and attribute index for this shader program
    static unsigned int _mVertexArrayIndexCount;
    map< string, GLTexture * > _MapTexture; //texture storage
    map< string, GLBufferInfo * > _MapBufferInfo; // information of buffer segments
    map< string, GLBufferInfoSequence * > _MapBufferInfoSequence; // information of buffer sequences
    GLBufferInfo * _CurrentBufferInfo;
    GLBufferInfoSequence * _CurrentBufferInfoSequence;
};

#endif
