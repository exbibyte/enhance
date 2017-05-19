#ifndef E2_RENDERDEVICE_GL_HPP
#define E2_RENDERDEVICE_GL_HPP

#include <cstring>
#include <map>

#include "gl_includes.hpp"
#include "i_renderdevice.hpp"
#include "i_renderresource.hpp"
#include "i_renderpackage.hpp"
#include "renderdevice_dispatch.hpp"
#include "buffer.hpp"

namespace e2 { namespace render {

class renderdevice_gl_impl {
public:
                                                       renderdevice_gl_impl();
                                                  bool renderdevice_process( ::e2::interface::i_renderpackage p );
         renderdevice_dispatch< renderdevice_gl_impl > _dispatch;

                                           static bool process_init_window_size( renderdevice_gl_impl *, ::e2::interface::i_renderpackage p );

                                           static bool process_init_window( renderdevice_gl_impl *, ::e2::interface::i_renderpackage p );
                                           static bool process_deinit_window( renderdevice_gl_impl *, ::e2::interface::i_renderpackage p );

                                           static bool process_exec_window( renderdevice_gl_impl *, ::e2::interface::i_renderpackage p );
                                           static bool process_exec_draw_batch( renderdevice_gl_impl *, ::e2::interface::i_renderpackage p );
                                           static bool process_store_buffer( renderdevice_gl_impl *, ::e2::interface::i_renderpackage p );
                                                GLuint _program;
                                          GLFWwindow * _window;
                       std::map< uint64_t, void * > _device_resources;
};

class renderdevice_gl : public ::e2::interface::i_renderdevice< renderdevice_gl_impl, ::e2::interface::i_renderpackage > {};

} }

#endif
