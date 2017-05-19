#ifndef RENDERPAYLOAD_GL_HPP
#define RENDERPAYLOAD_GL_HPP

#include <cstdint>

#include "i_renderpayload.hpp"
#include "buffer.hpp"

namespace e2 { namespace render {

class renderpayload_gl {
public:
    static bool unpack( size_t count_unpack, ::e2::interface::i_renderpayload *, void ** );
};

} }

#endif



