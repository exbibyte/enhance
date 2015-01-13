#ifndef EN_OBJRENDER_H
#define EN_OBJRENDER_H

#include "GLTexture.h"
#include "GLAttribData.h"
#include "DualQuat.h"
#include "glm.h"

//class representing a renderable object with vertex, normal, texture, orientation properties
class enObjRender {
public:
    bool SetGLTexture( GLTexture * );
    bool SetGLAttribVertex( GLAttribData * , int iStart, int iLen );
    bool SetGLAttribNormal( GLAttribData * , int iStart, int lLen);
    bool SetOrientation( DualQuat const & );
    bool GetOrientationMatrix( glm::mat4 & );
    bool GetTransformedVertex( double * Data, int iCount );
    bool GetTransformedNormal( double * Data, int iCount );
private:
    GLAttribData * _AttribVertex;
    GLAttribData * _AttribNormal;
    GLTexture * _Texture;
    DualQuat _Orientation;
    int iAttribOffsetVertex; //attribute data offset for this obj
    int iAttribOffsetNormal;
    int iAttribLength; //attribute data length for this obj
};

#endif
