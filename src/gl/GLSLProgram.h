#ifndef GLSLPROGRAM_H
#define GLSLPROGRAM_H

#include<iostream>
#include<vector>
#include<string>
#include<map>
#include<utility>
using namespace std;

#include <GL/glew.h>
#include <GL/glut.h>

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

class GLSLProgram{

public:
    GLSLProgram();
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
    void SetUniform( std::string Name, float x, float y, float z );
    void SetUniform( std::string Name, vec3 const & v );
    void SetUniform( std::string Name, vec4 const & v );
    void SetUniform( std::string Name, mat3 const & m );
    void SetUniform( std::string Name, mat4 const & m );
    void SetUniform( std::string Name, float val );
    void SetUniform( std::string Name, int val );
    void SetUniform( std::string Name, bool val );
    void PrintActiveUniforms() const;
    void PrintActiveAttribs() const;
    void AddMapAttrib( string AttribName, GLAttribData<float> * AttribData );
    void GetMapAttrib( string AttribName, GLAttribData<float> * & AttribData ) const;
    void BindMapAttrib(); //binds all attributes in map
private:
    GLuint _HandleProgram;
    bool _Linked;
    string _LogString;
    vector<GLuint> _vHandleShader;
    map< string, GLAttribData<float> * > _MapAttrib; //mapping of attribute name and attribute index for this shader program
};

#include "GLSLProgram.cpp"

#endif
