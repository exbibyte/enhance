#ifndef GLATTRIBDATA_H
#define GLATTRIBDATA_H

/* #ifdef MAC_OS */
/* #include <GLUT/glut.h> */
/* #else */
/* #include <GL/glut.h> */
/* #endif */

#include <list>

#include "GLIncludes.hpp"

#include <algorithm>

#include "GLHelper.h"

template< typename DataType >
class GLAttribData { 

public:
    GLAttribData(){
        glGenBuffers( 1, &_HandleBuffer );
    }
    ~GLAttribData(){
    }
    void SetVertexArrayIndex( GLuint vao, GLuint Index ){
        _indexVertexAttrib = Index;
        _hVAO = vao;
    }
    void SetData( DataType * Data, int VertexSize, int Count ){
        _VertexSize = VertexSize;
        _Size = Count;

        GLBindVertexArray( _hVAO );

        BindVertexBuf();
        //populate buffer with data
        glBufferData( GL_ARRAY_BUFFER, (unsigned int )Count * sizeof(DataType), Data, GL_STATIC_DRAW );

        //associate and bind vbo to vao index
        EnableVertexArray();
        glVertexAttribPointer( _indexVertexAttrib, VertexSize, GL_FLOAT, GL_FALSE, 0, (GLubyte *)NULL );

        UnBindVertexBuf();

        GLUnBindVertexArray();
    }    
    void EnableVertexArray()
    {
        glEnableVertexAttribArray( _indexVertexAttrib );
    }
    void DisableVertexArray()
    {
        glDisableVertexAttribArray( _indexVertexAttrib );
    }
    void BindVertexBuf() {
        glBindBuffer(GL_ARRAY_BUFFER, _HandleBuffer);
    }
    void UnBindVertexBuf() {
        glBindBuffer( GL_ARRAY_BUFFER, 0 );
    }
    GLuint GetIndexAttrib() const {
        return _indexVertexAttrib;
    }
private:
    GLuint _HandleBuffer; //vbo size of 1
    int _VertexSize;
    int _Size;
    GLuint _indexVertexAttrib;
    GLuint _hVAO;
};

#endif
