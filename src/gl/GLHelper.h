#ifndef GLHELPER_H
#define GLHELPER_H

#include <stdio.h>
#include <stdlib.h>
#include <GL/glew.h>
#include <GL/glut.h>

#include <iostream>
using namespace std;

#define GLM_FORCE_RADIANS

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
using glm::mat3;
using glm::mat4;
using glm::vec3;
using glm::vec4;

#include "textfile.h"

namespace GLSLShader {
    enum GLSLShaderType {
        VERTEX, FRAGMENT, GEOMETRY,TESS_CONTROL, TESS_EVALUATION
    };
};

bool GLCompileShaderFromString( GLuint & shader, char const * Source, GLSLShader::GLSLShaderType type );
bool GLCompileShaderFromFile( GLuint & shader, char const * fileName, GLSLShader::GLSLShaderType type );
bool GLCreateProgram( GLuint & program );
bool GLLinkProgram( GLuint & program );
bool GLLinkUseProgram( GLuint & program );
void GLUseProgram( GLuint & program );
void GLPrintInfo();

void GLBindAttribLocation( GLuint Program, GLuint Loc, char const * Name );
void GLBindFragDataLocation( GLuint Program, GLuint Loc, char const * Name );
void GLSetUniform( GLuint Program, char const * Name, vec3 const & v );
void GLSetUniform( GLuint Program, char const * Name, vec4 const & v );
void GLSetUniform( GLuint Program, char const * Name, mat3 const & m );
void GLSetUniform( GLuint Program, char const * Name, mat4 const & m );
void GLSetUniform( GLuint Program, char const * Name, float val );
void GLSetUniform( GLuint Program, char const * Name, int val );
void GLSetUniform( GLuint Program, char const * Name, bool val );
void GLPrintActiveUniforms( GLuint Program );
void GLPrintActiveAttribs( GLuint Program );

///////////

bool GLCompileShaderFromString( GLuint & shader, char const * Source, GLSLShader::GLSLShaderType type ){
    bool bRet;

    if( GLSLShader::VERTEX == type ){
        shader = glCreateShader(GL_VERTEX_SHADER);
    }else if( GLSLShader::FRAGMENT == type ){
        shader = glCreateShader(GL_FRAGMENT_SHADER);
    }

    char const * sourceTextConst = Source;
    glShaderSource( shader, 1, &sourceTextConst, NULL);	
    
    glCompileShader(shader);
    GLint result;
    glGetShaderiv( shader, GL_COMPILE_STATUS, &result );
    if( GL_FALSE == result ){
        cerr<< "shader compilation failed!" <<endl;
        GLint logLen;
        glGetShaderiv( shader, GL_INFO_LOG_LENGTH, &logLen );
        if( logLen > 0 ){
            char * log = (char *)malloc(logLen);
            GLsizei written;
            glGetShaderInfoLog( shader, logLen, &written, log);
            cerr << "Shader log: \n" << log <<endl;
            free(log);
        }
        bRet = false;
    }else{
        bRet = true;
    }
    return bRet;
}

bool GLCompileShaderFromFile( GLuint & shader, char const * fileName, GLSLShader::GLSLShaderType type ){
    bool bRet;
    char * sourceText = 0;
    sourceText = textFileRead( fileName );
    char const * sourceTextConst = sourceText;
    bRet = GLCompileShaderFromString( shader, sourceTextConst, type );
    free(sourceText);
    return bRet;
}

bool GLCreateProgram( GLuint & program ){
    program = glCreateProgram();
    if( 0 == program ){
        cerr<< "Error creating program object" <<endl;
        return false;
    }else{
        return true;
    }
}

bool GLLinkProgram( GLuint & program ){
    glLinkProgram(program);
    GLint status;
    glGetProgramiv( program, GL_LINK_STATUS, &status );
    if( GL_FALSE == status ){
        cerr << "Failed to link shader program!" <<endl;
        GLint logLen;
        glGetProgramiv( program, GL_INFO_LOG_LENGTH, &logLen);
        if( logLen > 0 ){
            char * log = (char *)malloc(logLen);
            GLsizei written;
            glGetProgramInfoLog( program, logLen, &written, log);
            cerr<< "Program log: \n" << log <<endl;
            free(log);
        }
        return false;
    }else{
        return true;
    }
}
bool GLLinkUseProgram( GLuint & program ){
    if( true == GLLinkProgram( program ) ){
        glUseProgram(program);
        return true;
    }else{
        return false;
    }
}

void GLUseProgram( GLuint & program ){
    glUseProgram(program);
}

void GLPrintInfo(){
    const char * p_glversion = (const char * ) glGetString(GL_VERSION);
    cout << "GL Version: " << p_glversion <<endl;
}

void GLBindAttribLocation( GLuint Program, GLuint Loc, char const * Name ){
    glBindAttribLocation( Program, Loc, (GLchar const * ) Name );
}

void GLBindFragDataLocation( GLuint Program, GLuint Loc, char const * Name ){
    glBindFragDataLocation( Program, Loc, (GLchar const * ) Name );
}

void GLSetUniform( GLuint Program, char const * Name, vec3 const & v ){
    GLuint location = glGetUniformLocation( Program, (GLchar const * ) Name );
    if( location >= 0 )
    {
        glUniform3fv( location, 1, &v[0]);
    }
}

void GLSetUniform( GLuint Program, char const * Name, vec4 const & v ){
    GLuint location = glGetUniformLocation( Program, (GLchar const * ) Name );
    if( location >= 0 )
    {
        glUniform4fv( location, 1, &v[0]);
    }
}
void GLSetUniform( GLuint Program, char const * Name, mat3 const & m ){
    GLuint location = glGetUniformLocation( Program, (GLchar const * ) Name );
    if( location >= 0 )
    {
        glUniformMatrix3fv( location, 1, GL_FALSE, &m[0][0]);
    }
}
void GLSetUniform( GLuint Program, char const * Name, mat4 const & m ){
    GLuint location = glGetUniformLocation( Program, (GLchar const * ) Name );
    if( location >= 0 )
    {
        glUniformMatrix4fv( location, 1, GL_FALSE, &m[0][0]);
    }
}

void GLSetUniform( GLuint Program, char const * Name, float val ){
    GLuint location = glGetUniformLocation( Program, (GLchar const * ) Name );
    if( location >= 0 )
    {
        glUniform1f( location, val );
    }
}
void GLSetUniform( GLuint Program, char const * Name, int val ){
    GLuint location = glGetUniformLocation( Program, (GLchar const * ) Name );
    if( location >= 0 )
    {
        glUniform1i( location, val );
    }
}
void GLSetUniform( GLuint Program, char const * Name, bool val ){
    
    int ValInt;
    if(val){
        ValInt = 1;
    }else{
        ValInt = 0;
    }

    GLuint location = glGetUniformLocation( Program, (GLchar const * ) Name );
    if( location >= 0 )
    {
        glUniform1i( location, ValInt );
    }
}

void GLPrintActiveUniforms( GLuint Program ){}
void GLPrintActiveAttribs( GLuint Program ){}

#endif
