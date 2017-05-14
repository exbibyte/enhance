#include <cassert>

#include "gl_includes.hpp"
#include "gl_texture.hpp"
#include "ppm.hpp"

namespace e2 { namespace render { namespace gl {

gl_texture::gl_texture() : _tex(-1), _fbo(-1), _active_texture(-1) {
    
}

bool gl_texture::get_texture_id( GLuint & TextureId ) {
    TextureId = _tex;
    return true;
}

bool gl_texture::get_fbo_id( GLuint & FboId ) {
    FboId = _fbo;
    return true;
}

bool gl_texture::get_active_texture( int & iactivetexture ) {
    if( _active_texture >= 0 ) {
        iactivetexture = _active_texture;
        return true;
    } else {
        return false;
    }
}

bool gl_texture::set_texture_from_file( char const * strTexturePath, int _active_texture )
{
    bool ret;
    int width, height;
    std::vector<GLubyte> img_data;
    ret = ::e2::file::ppm::read( strTexturePath, width, height, &img_data );
    if( false == ret )
    {
	assert( false && "ppm read unsuccessful." );
	return false;
    }
    return set_texture( RGB, width, height, &img_data[0], _active_texture );
}

bool gl_texture::set_texture( eTextureType TextureType, int iWidth, int iHeight, void * const Data, int _active_texture ) {
    bool bRet = true;

    GLint max_textures = 0;
    glGetIntegerv(GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS, &max_textures);
    if( _active_texture >= max_textures - 1 ) {
        bRet = false;
        return bRet;
    }

    switch( TextureType ) {
    case DEPTH:
    {
        bRet = set_texture_depth( iWidth, iHeight, _active_texture );
    }
    break;
    case RGB:
    {
        bRet = set_texture_rgb( iWidth, iHeight, Data, _active_texture );
    }
    break;
    default:
        bRet = false;
    }
    return bRet;
}
bool gl_texture::set_texture_rgb( int iWidth, int iHeight, void * const Data, int active_texture ) {
    bool bRet = true;

    glGenTextures( 1, &_tex );
    //Assign the shadow map to designated texture channel
    glActiveTexture( GL_TEXTURE0 + _active_texture );    
    //bind texture to current texture holder
    glBindTexture(GL_TEXTURE_2D, _tex );

    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR );

    //pass texture to openGL
    glTexImage2D( GL_TEXTURE_2D, 0, GL_RGB, iWidth, iHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, Data );

    //let opengl automatically build mipmap
    glGenerateMipmap( GL_TEXTURE_2D );    

    _active_texture = active_texture;
    
    return bRet;
}

bool gl_texture::set_texture_depth( int iWidth, int iHeight, int active_texture ) {
    bool bRet = true;

    GLfloat border[]={ 1.0f,0.0f,0.0f,0.0f };

    //The shadow maptexture
    glGenTextures( 1, &_tex );
    //Assign the shadow map to designated texture channel
    glActiveTexture( GL_TEXTURE0 + _active_texture );
    glBindTexture( GL_TEXTURE_2D, _tex );
    
    // glTexImage2D( GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, iWidth, iHeight, 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_BYTE, NULL);
    glTexImage2D( GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT24, iWidth, iHeight, 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER );
    glTexParameterfv( GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, border );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LESS );
    
    //Create and set up the FBO
    glGenFramebuffers( 1, &_fbo );
    glBindFramebuffer( GL_FRAMEBUFFER, _fbo );
    glFramebufferTexture2D( GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, _tex, 0 );
    GLenum drawBuffers[] = {GL_NONE};
    glDrawBuffers( 1, drawBuffers );
    // Revert to the default framebuffer for now
    glBindFramebuffer( GL_FRAMEBUFFER, 0 );

    _active_texture = active_texture;
    
    return bRet;
}

bool gl_texture::bind_fbo(){
    glBindFramebuffer( GL_FRAMEBUFFER, _fbo );
    return true;
}

bool gl_texture::unbind_fbo(){
    // Revert to the default framebuffer for now
    glBindFramebuffer( GL_FRAMEBUFFER, 0 );
    return true;
}

} } }
