#ifndef E2_I_RENDERRESOURCE_HPP
#define E2_I_RENDERRESOURCE_HPP

#include "buffer_info.hpp"

namespace e2 { namespace interface {

enum e_renderresource_type { //list of primitive resources that can be manipulated
    windowing,
    camera,
    attrib_vertex,
    attrib_normal,
    attrib_texture,
    transform_rotate,
    transform_translate,
};
	
class i_renderresource {
public:
    size_t _id;
};

} }

#endif
