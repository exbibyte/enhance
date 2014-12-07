#ifndef GLATTRIBDATA_H
#define GLATTRIBDATA_H

#include <GL/glew.h>
#include <GL/glut.h>

#include <algorithm>

#include "GLHelper.h"

template< typename DataType >
class GLAttribData { 

public:
    GLAttribData(){
        glGenBuffers( 1, &_HandleBuffer );
    }
    ~GLAttribData(){
        if( _pData ) {
            delete [] _pData;
            _pData = 0;
        }
    }
    void SetVertexArrayIndex( GLuint vao, GLuint Index ){
        IndexVertexAttrib = Index;
        _hVAO = vao;
    }
    void SetData( DataType * Data, int VertexSize, int Count ){
        _pData = new DataType[ Count ];
        std::copy ( Data, Data + Count, _pData );
        _VertexSize = VertexSize;
        _Size = Count;

        GLBindVertexArray( _hVAO );

        BindVertexBuf();
        //populate buffer with data
        glBufferData( GL_ARRAY_BUFFER, (unsigned int )Count * sizeof(DataType), Data, GL_STATIC_DRAW );

        //associate and bind vbo to vao index
        EnableVertexArray();
        glVertexAttribPointer( IndexVertexAttrib, VertexSize, GL_FLOAT, GL_FALSE, 0, (GLubyte *)NULL );

        UnBindVertexBuf();

        GLUnBindVertexArray();
    }    
    void EnableVertexArray()
    {
        glEnableVertexAttribArray( IndexVertexAttrib );
    }
    void DisableVertexArray()
    {
        glDisableVertexAttribArray( IndexVertexAttrib );
    }
    void BindVertexBuf() {
        glBindBuffer(GL_ARRAY_BUFFER, _HandleBuffer);
    }
    void UnBindVertexBuf() {
        glBindBuffer( GL_ARRAY_BUFFER, 0 );
    }
    GLuint GetIndexAttrib() const {
        return IndexVertexAttrib;
    }
    void GetData( DataType * & Data, int & VertexSize, int & DataCount ) const {
        Data = _pData;
        VertexSize = _VertexSize;
        DataCount = _Size;
    }

private:
    GLuint _HandleBuffer; //vbo size of 1
    DataType * _pData;
    int _VertexSize;
    int _Size;
    GLuint IndexVertexAttrib;
    GLuint _hVAO;
};

#endif
