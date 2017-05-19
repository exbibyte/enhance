#ifndef RENDERMAP_GL_HPP
#define RENDERMAP_GL_HPP

#include <unordered_map>
#include <cstdint>

#include "gl_includes.hpp"

namespace e2 { namespace render {

class rendermap_gl {
public:
    static std::unordered_map< uint64_t, GLenum > _map_render_primitives;
    static std::unordered_map< uint64_t, GLenum > _map_render_buffer;
};

} }

#endif
