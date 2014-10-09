#ifndef GLSLPROGRAM_H
#define GLSLPROGRAM_H

#include<iostream>
using namespace std;

#include <GL/glew.h>
#include <GL/glut.h>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
using glm::mat4;
using glm::vec3;

#include "GLHelper.h"

class GLSLProgram{

public:
GLSLProgram();
GLuint GetHandle() const;
bool IsLinked() const;
bool Link();
bool CompileShaderFromFile( char const * FileName, GLSLShader::GLSLShaderType Type );
bool CompileShaderFromString( string const & Source, GLSLShader::GLSLShaderType Type );
void Use();
string Log() const;
void BindAttribLocation( GLuint Loc, char const * Name );
void BindFragDataLocation( GLuint Loc, char const * Name );
void SetUniform( char const * Name, float x, float y, float z );
void SetUniform( char const * Name, vec3 const & v );
void SetUniform( char const * Name, vec4 const & v );
void SetUniform( char const * Name, mat3 const & m );
void SetUniform( char const * Name, mat4 const & m );
void SetUniform( char const * Name, float val );
void SetUniform( char const * Name, int val );
void SetUniform( char const * Name, bool val );
void PrintActiveUniforms() const;
void PrintActiveAttribs() const;

private:
GLuint _Handle;
bool _Linked;
string _LogString;
};

#endif
