#ifndef GLATTRIBDATA_H
#define GLATTRIBDATA_H

#include <GL/glew.h>
#include <GL/glut.h>

template< typename DataType >
class GLAttribData { 

public:
    GLAttribData(){
        glGenBuffers( 1, &_HandleBuffer );
        IndexVertexAttrib = IndexVertexAttribGlobal;
        ++IndexVertexAttribGlobal;
    }
    void SetData( DataType * Data, int VertexSize, int Count ){
        _pData = Data;
        _VertexSize = VertexSize;
        _Size = Count;

        glBindBuffer( GL_ARRAY_BUFFER, _HandleBuffer );
        //populate buffer with data
        glBufferData( GL_ARRAY_BUFFER, Count * sizeof(DataType), Data, GL_STATIC_DRAW );

        glEnableVertexAttribArray( IndexVertexAttrib );

        glVertexAttribPointer( IndexVertexAttrib, VertexSize, GL_FLOAT, GL_FALSE, 0, (GLubyte *)NULL );
    }
    void BindAttrib() {
        glEnableVertexAttribArray( IndexVertexAttrib );
        glBindBuffer(GL_ARRAY_BUFFER, _HandleBuffer);
    }
    void UnBindAttrib() {
        glDisableVertexAttribArray( IndexVertexAttrib );
        glBindBuffer( GL_ARRAY_BUFFER, 0 );
    }
    int GetIndexAttrib() const {
        return IndexVertexAttrib;
    }
    void GetData( DataType * & Data, int & VertexSize, int & DataCount ) const {
        Data = _pData;
        VertexSize = _VertexSize;
        DataCount = _Size;
    }
    void Draw(){
        glDrawArrays( GL_TRIANGLES, 0, (sizeof(_pData)/3)/sizeof(DataType) );
    }
private:
    GLuint _HandleBuffer;
    DataType * _pData;
    int _VertexSize;
    int _Size;
    static unsigned int IndexVertexAttribGlobal;
    unsigned int IndexVertexAttrib;
};

template< typename DataType >
unsigned int GLAttribData< DataType >::IndexVertexAttribGlobal = 0;

#endif
