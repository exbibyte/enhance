#ifndef E2_RENDERPACKAGE_GL_HPP
#define E2_RENDERPACKAGE_GL_HPP

#include <cstdint>

#include "i_renderpackage.hpp"
#include "buffer.hpp"

namespace e2 { namespace render {

class renderpackage_gl {
public:
    static bool unpack( ::e2::interface::i_renderpackage *, void ** );
};

} }

#endif
