#ifndef E2_RENDERPACKAGE_GL_HPP
#define E2_RENDERPACKAGE_GL_HPP

#include <cstdint>

#include "i_renderpackage.hpp"
#include "buffer.hpp"

namespace e2 { namespace render {

class renderpackage_gl {
public:
    static bool pack( ::e2::memory::buffer * b, ::e2::interface::i_renderpackage ** package, uint64_t * renderresource_key, void ** renderresource_data, size_t count );
    static bool unpack( ::e2::interface::i_renderpackage * pkg, ::e2::interface::i_package_filter * filter );
};

} }

#endif
