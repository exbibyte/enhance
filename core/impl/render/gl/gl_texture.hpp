#ifndef E2_GL_TEXTURE_HPP
#define E2_GL_TEXTURE_HPP

#include "gl_includes.hpp"

namespace e2 { namespace render { namespace gl {

class gl_texture {
public:
    enum eTextureType {
        DEPTH,
        RGB,
    };
    gl_texture();
    bool bind_fbo();
    bool unbind_fbo();
    bool get_texture_id( GLuint & TextureId );
    bool get_fbo_id( GLuint & FboId );
    bool get_active_texture( int & iActiveTexture );
    bool set_texture_from_file( char const * strTexturePath, int iActiveTexture );
    bool set_texture( eTextureType TextureType, int iWidth, int iHeight, void * const Data, int iActiveTexture );
private:
    bool set_texture_rgb( int iWidth, int iHeight, void * const Data, int iActiveTexture );
    bool set_texture_depth( int iWidth, int iHeight, int iActiveTexture );
    GLuint _tex;
    GLuint _fbo;
    int _active_texture;
};

} } }

#endif
