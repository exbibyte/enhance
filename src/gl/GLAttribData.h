#ifndef GLATTRIBDATA_H
#define GLATTRIBDATA_H

#include <GL/glew.h>
#include <GL/glut.h>

template< typename DataType >
class GLAttribData { 

public:
    GLAttribData(){
        glGenBuffers( & _HandleBuffer );
        shaderAttribute = shaderAttributeGlobal;
        ++shaderAttributeGlobal;
    }
    void SetData( DataType * Data, int VertexSize, int Count ){
        _pData = Data;
        _VertexSize = VertexSize;
        _Size = Count;

        glBindBuffer( GL_ARRAY_BUFFER, _HandleBuffer );
        //populate buffer with data
        glBufferData( GL_ARRAY_BUFFER, Count * sizeof(DataType), Data, GL_STATIC_DRAW );

        glVertexAttribPointer( shaderAttribute, VertexSize, GL_FLOAT, GL_FALSE, 0, (GLubyte *)NULL );
        glEnableVertexAttribArray( shaderAttribute );

        glBindBuffer(GL_ARRAY_BUFFER, _HandleBuffer);
    }
    void BindAttrib() {
        glEnableVertexAttribArray( shaderAttribute );
        glBindBuffer(GL_ARRAY_BUFFER, _HandleBuffer);
    }
    int GetAttribIndex() const {
        return shaderAttribute;
    }
    int GetSize() const {

    }
    void GetData( DataType * & Data, int
private:
    GLuint _HandleBuffer;
    DataType * _pData;
    int _VertexSize;
    int _Size;
    static unsigned int shaderAttributeGlobal = 0;
    unsigned int shaderAttribute;
};

#endif
