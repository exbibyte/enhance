#ifndef GLHELPER_H
#define GLHELPER_H

#include <stdio.h>
#include <stdlib.h>
#include <GL/glew.h>
#include <GL/glut.h>

#include <iostream>

#include "textfile.h"

using namespace std;

namespace GLSLShader {
    enum GLSLShaderType {
        VERTEX, FRAGMENT, GEOMETRY,TESS_CONTROL, TESS_EVALUATION
    };
};

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

#endif
