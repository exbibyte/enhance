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

class renderdevice_gl : public ::e2::interface::i_renderdevice {
public:
                                                       renderdevice_gl();
                                                  bool renderdevice_process( ::e2::interface::i_renderpackage p ) override;
         renderdevice_dispatch< renderdevice_gl > _dispatch;

                                           static bool process_init_window_size( renderdevice_gl *, ::e2::interface::i_renderpackage p );
                                           static bool process_init_window( renderdevice_gl *, ::e2::interface::i_renderpackage p );
                                           static bool process_init_program( renderdevice_gl *, ::e2::interface::i_renderpackage p );
                                           static bool process_init_object( renderdevice_gl *, ::e2::interface::i_renderpackage p );
                                           static bool process_init_buffer( renderdevice_gl *, ::e2::interface::i_renderpackage p );
    
                                           static bool process_deinit_window( renderdevice_gl *, ::e2::interface::i_renderpackage p );
                                           static bool process_deinit_object( renderdevice_gl *, ::e2::interface::i_renderpackage p );
                                           static bool process_deinit_buffer( renderdevice_gl *, ::e2::interface::i_renderpackage p );
                                           static bool process_deinit_program( renderdevice_gl *, ::e2::interface::i_renderpackage p );
    
                                           static bool process_exec_window( renderdevice_gl *, ::e2::interface::i_renderpackage p );
                                           static bool process_exec_drawbatch( renderdevice_gl *, ::e2::interface::i_renderpackage p );
    
                                           static bool process_store_defineformat( renderdevice_gl *, ::e2::interface::i_renderpackage p );
                                           static bool process_store_buffer( renderdevice_gl *, ::e2::interface::i_renderpackage p );
                                           static bool process_store_persistent( renderdevice_gl *, ::e2::interface::i_renderpackage p );
                                           static bool process_store_window( renderdevice_gl *, ::e2::interface::i_renderpackage p );
    
                                           static bool process_enable_attrib( renderdevice_gl *, ::e2::interface::i_renderpackage p );
    
                                           static bool process_disable_attrib( renderdevice_gl *, ::e2::interface::i_renderpackage p );

                                           static bool process_compute_program( renderdevice_gl *, ::e2::interface::i_renderpackage p );
    
                                           static bool process_bind_attrib( renderdevice_gl *, ::e2::interface::i_renderpackage p );
                                           static bool process_bind_shader( renderdevice_gl *, ::e2::interface::i_renderpackage p );
                                           static bool process_bind_object( renderdevice_gl *, ::e2::interface::i_renderpackage p );
                                           static bool process_bind_buffer( renderdevice_gl *, ::e2::interface::i_renderpackage p );
                                           static bool process_bind_program( renderdevice_gl *, ::e2::interface::i_renderpackage p );
    
                                           static bool process_unbind_shader( renderdevice_gl *, ::e2::interface::i_renderpackage p );

                                           static bool process_load_shader( renderdevice_gl *, ::e2::interface::i_renderpackage p );
                                           static bool process_store_program( renderdevice_gl *, ::e2::interface::i_renderpackage p );

                                           static bool process_query_persistent( renderdevice_gl *, ::e2::interface::i_renderpackage p );
                                           static bool process_query_attrib( renderdevice_gl *, ::e2::interface::i_renderpackage p );

                                                GLuint _program;
                                          GLFWwindow * _window;
};

} }

#endif
