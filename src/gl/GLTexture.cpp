#include "GLTexture.h"

GLTexture::GLTexture() : _Tex(-1), _Fbo(-1) {
    
}

bool GLTexture::GetTextureId( GLuint & TextureId ) {
    if( _Tex < 0 ) {
        return false;
    } else {
        TextureId = _Tex;
        return true;
    }
}

bool GLTexture::GetFboId( GLuint & FboId ) {
    if( _Fbo < 0 ) {
        return false;
    } else {
        FboId = _Fbo;
        return true;
    }
}

bool GLTexture::SetTexture( eTextureType TextureType, int iWidth, int iHeight, void * const Data, int iActiveTexture ) {
    bool bRet = true;

    GLint max_textures = 0;
    glGetIntegerv(GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS, &max_textures);
    if( iActiveTexture >= max_textures - 1 ) {
        bRet = false;
        return bRet;
    }

    switch( TextureType ) {
    case DEPTH:
    {
        bRet = SetTextureDepth( iWidth, iHeight, iActiveTexture );
    }
    break;
    case RGBA:
    {
        bRet = SetTextureRgba( iWidth, iHeight, Data, iActiveTexture );
    }
    break;
    default:
        bRet = false;
    }
    return bRet;
}
bool GLTexture::SetTextureRgba( int iWidth, int iHeight, void * const Data, int iActiveTexture ) {
    bool bRet = true;
    
    return bRet;
}

bool GLTexture::SetTextureDepth( int iWidth, int iHeight, int iActiveTexture ) {
    bool bRet = true;

    GLfloat border[]={ 1.0f,0.0f,0.0f,0.0f };
    //The shadow maptexture
    glGenTextures( 1, &_Tex );
    glBindTexture( GL_TEXTURE_2D, _Tex );
    glTexImage2D( GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, iWidth, iHeight, 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER );
    glTexParameterfv( GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, border );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LESS );
    //Assign the shadow map to texture channel 0
    glActiveTexture( GL_TEXTURE0 + iActiveTexture );
    glBindTexture( GL_TEXTURE_2D, _Tex );
    //Create and set up the FBO
    glGenFramebuffers( 1, &_Fbo );
    glBindFramebuffer( GL_FRAMEBUFFER, _Fbo );
    glFramebufferTexture2D( GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, _Tex, 0 );
    GLenum drawBuffers[] = {GL_NONE};
    glDrawBuffers( 1, drawBuffers );
    // Revert to the default framebuffer for now
    glBindFramebuffer( GL_FRAMEBUFFER, 0 );

    return bRet;
}

bool GLTexture::BindFbo() {
    glBindFramebuffer( GL_FRAMEBUFFER, _Fbo );
}

bool GLTexture::UnbindFbo() {
    // Revert to the default framebuffer for now
    glBindFramebuffer( GL_FRAMEBUFFER, 0 );
}
