#ifndef GLTEXTURE_H
#define GLTEXTURE_H

/* #include <GL/glew.h> */

/* #ifdef MAC_OS */
/* #include <GLUT/glut.h> */
/* #else */
/* #include <GL/glut.h> */
/* #endif */

#include "GLIncludes.hpp"

#include <string>
using namespace std;

class GLTexture {
public:
    enum eTextureType {
        DEPTH,
        RGB,
    };
    GLTexture();
    bool BindFbo();
    bool UnbindFbo();
    bool GetTextureId( GLuint & TextureId );
    bool GetFboId( GLuint & FboId );
    bool GetActiveTexture( int & iActiveTexture );
    bool SetTextureFromFile( string strTexturePath, int iActiveTexture );
    bool SetTexture( eTextureType TextureType, int iWidth, int iHeight, void * const Data, int iActiveTexture );
private:
    bool SetTextureRgb( int iWidth, int iHeight, void * const Data, int iActiveTexture );
    bool SetTextureDepth( int iWidth, int iHeight, int iActiveTexture );
    GLuint _Tex;
    GLuint _Fbo;
    int _iActiveTexture;
};

#endif
