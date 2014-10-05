#ifndef GLHELPER_H
#define GLHELPER_H

#include <stdio.h>
#include <stdlib.h>
#include <GL/glew.h>
#include <GL/glut.h>

bool GLCompileShader( GLuint & shader ){
    glCompileShader(shader);
    GLint result;
    glGetShaderiv( shader, GL_COMPILE_STATUS, &result );
    if( GL_FALSE == result ){
        fprintf( stderr, "shader compilation failed!\n" );
        GLint logLen;
        glGetShaderiv( shader, GL_INFO_LOG_LENGTH, &logLen );
        if( logLen > 0 ){
            char * log = (char *)malloc(logLen);
            GLsizei written;
            glGetShaderInfoLog( shader, logLen, &written, log);
            fprintf(stderr, "Shader log:\n%s", log);
            free(log);
        }
    }
}

bool GLCreateProgram( GLuint & program ){
    program = glCreateProgram();
    if( 0 == program ){
        fprintf(stderr, "Error creating program object.\n");
        exit(1);
    }
}

bool GLLinkProgram( GLuint & program ){
    glLinkProgram(program);
    GLint status;
    glGetProgramiv( program, GL_LINK_STATUS, &status );
    if( GL_FALSE == status ){
        fprintf( stderr, "Failed to link shader program!\n" );
        GLint logLen;
        glGetProgramiv( program, GL_INFO_LOG_LENGTH, &logLen);
        if( logLen > 0 ){
            char * log = (char *)malloc(logLen);
            GLsizei written;
            glGetProgramInfoLog( program, logLen, &written, log);
            fprintf(stderr, "Program log: \n%s", log);
            free(log);
        }
        return false;
    }else{
        glUseProgram(program);
        return true;
    }
}

#endif
