#ifndef E2_I_RENDERRESOURCE_HPP
#define E2_I_RENDERRESOURCE_HPP

#include <cstdint>

namespace e2 { namespace interface {

enum e_renderresource_type : uint64_t { //list of primitive resources that can be manipulated
    windowing = 1,
    camera,
    attrib_vertex,
    attrib_normal,
    attrib_texture,
    transform_rotate,
    transform_translate,
};

enum e_renderresource_window : uint64_t {
    window_dim = 1,
    window_buf_swap,
};

enum e_renderresource_camera : uint64_t {
    camera_coord = 1,
    camera_dir,
    camera_up,
    camera_fov,
};

class i_renderresource {
public:
    size_t _id;
};

} }

#endif
