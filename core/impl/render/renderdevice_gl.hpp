#ifndef E2_RENDERDEVICE_GL_HPP
#define E2_RENDERDEVICE_GL_HPP

#include <cstring>

#include "i_renderdevice.hpp"
#include "i_renderresource.hpp"
#include "i_renderpackage.hpp"

namespace e2 { namespace render {
	
class renderdevice_gl_impl {
public:
                renderdevice_gl_impl();
           bool renderdevice_process( ::e2::interface::i_renderpackage p );
    // renderdevice_dispatch _dispatch;

    static bool process_init_window_size( ::e2::interface::i_renderpackage p );
};

class renderdevice_gl : public ::e2::interface::i_renderdevice< renderdevice_gl_impl, ::e2::interface::i_renderpackage > {};

} }

#endif
