#ifndef GLSLPROGRAM_H
#define GLSLPROGRAM_H

#include<iostream>
#include<vector>
#include<string>
#include<map>
#include<utility>
using namespace std;

#include <GLFW/glfw3.h>

/* #include <GL/glew.h> */
/* #ifdef MAC_OS */
/* #include <GLUT/glut.h> */
/* #else */
/* #include <GL/glut.h> */
/* #endif */

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
using glm::mat3;
using glm::mat4;
using glm::vec3;
using glm::vec4;

#include "GLHelper.h"
#include "GLAttribData.h"
#include "GLTexture.h"

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
    bool SetUniform( std::string Name, vec3 const & v );
    bool SetUniform( std::string Name, vec4 const & v );
    bool SetUniform( std::string Name, mat3 const & m );
    bool SetUniform( std::string Name, mat4 const & m );
    bool SetUniform( std::string Name, float val );
    bool SetUniform( std::string Name, int val );
    bool SetUniform( std::string Name, bool val );
    void PrintActiveUniforms() const;
    void PrintActiveAttribs() const;
    bool AddMapAttrib( string AttribName, GLAttribData<float> * AttribData );
    bool GetMapAttrib( string AttribName, GLAttribData<float> * & AttribData ) const;
    void BindMapAttrib(); //binds all attributes in map
    void BindVertexArray();
    void UnBindVertexArray();
    bool AddMapTexture( string TextureName, GLTexture * Texture );
    bool GetMapTexture( string TextureName, GLTexture * & Texture ) const;
    bool AddNewTexture( string TextureName, GLTexture::eTextureType TextureType, int iWidth, int iHeight, void * const Data, int iActiveTexture );
private:
    GLuint _HandleProgram;
    bool _Linked;
    string _LogString;
    vector< GLuint > _vHandleShader;
    GLuint _VertexArrayObj; //holding handles to individual VBO associated with GLAttribData
    map< string, GLAttribData<float> * > _MapAttrib; //mapping of attribute name and attribute index for this shader program
    static unsigned int _mVertexArrayIndexCount;
    map< string, GLTexture * > _MapTexture; //texture storage
};

#include "GLSLProgram.cpp"

#endif
