#ifndef E2_RENDERDEVICE_GL_HPP
#define E2_RENDERDEVICE_GL_HPP

#include <map>
#include <iostream>
#include <cstdio>
#include <cassert>
#include <cstring>

#include "i_renderdevice.hpp"
#include "i_rendernode.hpp"
#include "gl_includes.hpp"

namespace e2 { namespace render {

class renderdevice_gl : public ::e2::interface::i_renderdevice {
public:
                                           bool renderdevice_process_impl( ::e2::interface::i_rendernode_init_window * ) override;
                                           bool renderdevice_process_impl( ::e2::interface::i_rendernode_init_program * ) override;
                                           bool renderdevice_process_impl( ::e2::interface::i_rendernode_init_object_vertex_array * ) override;
                                           bool renderdevice_process_impl( ::e2::interface::i_rendernode_init_buffer * ) override;
    
                                           bool renderdevice_process_impl( ::e2::interface::i_rendernode_deinit_window * ) override;
                                           bool renderdevice_process_impl( ::e2::interface::i_rendernode_deinit_object_vertex_array * ) override;
                                           bool renderdevice_process_impl( ::e2::interface::i_rendernode_deinit_buffer * ) override;
                                           bool renderdevice_process_impl( ::e2::interface::i_rendernode_deinit_program * ) override;
    
                                           bool renderdevice_process_impl( ::e2::interface::i_rendernode_swap_window_buffer * ) override;
                                           bool renderdevice_process_impl( ::e2::interface::i_rendernode_clear_window_buffer_colour * ) override;
                                           bool renderdevice_process_impl( ::e2::interface::i_rendernode_clear_window_buffer_depth * ) override;
                                           bool renderdevice_process_impl( ::e2::interface::i_rendernode_disable_window_buffer_depth * ) override;
                                           bool renderdevice_process_impl( ::e2::interface::i_rendernode_enable_window_buffer_depth * ) override;
    
                                           bool renderdevice_process_impl( ::e2::interface::i_rendernode_exec_drawbatch * ) override;
    
                                           bool renderdevice_process_impl( ::e2::interface::i_rendernode_store_defineformat_vertexattrib * ) override;
                                           bool renderdevice_process_impl( ::e2::interface::i_rendernode_store_buffer * ) override;
                                           bool renderdevice_process_impl( ::e2::interface::i_rendernode_store_persistent_vecf * ) override;
                                           bool renderdevice_process_impl( ::e2::interface::i_rendernode_store_persistent_matf * ) override;
                                           // bool process_store_window( renderdevice_gl *, ::e2::interface::i_renderpackage p );
    
                                           bool renderdevice_process_impl( ::e2::interface::i_rendernode_enable_attrib_vertex_array * ) override;
    
                                           bool renderdevice_process_impl( ::e2::interface::i_rendernode_disable_attrib_vertex_array * ) override;

                                           bool renderdevice_process_impl( ::e2::interface::i_rendernode_compute_program * ) override;
    
                                           bool renderdevice_process_impl( ::e2::interface::i_rendernode_bind_attrib_attrib_loc * ) override;
                                           bool renderdevice_process_impl( ::e2::interface::i_rendernode_bind_attrib_frag_loc * ) override;
                                           bool renderdevice_process_impl( ::e2::interface::i_rendernode_bind_shader * ) override;
                                           bool renderdevice_process_impl( ::e2::interface::i_rendernode_bind_object_vertex_array * ) override;
                                           bool renderdevice_process_impl( ::e2::interface::i_rendernode_bind_buffer * ) override;
                                           bool renderdevice_process_impl( ::e2::interface::i_rendernode_bind_program * ) override;
    
                                           bool renderdevice_process_impl( ::e2::interface::i_rendernode_unbind_shader * ) override;

                                           bool renderdevice_process_impl( ::e2::interface::i_rendernode_load_shader * ) override;
                                           bool renderdevice_process_impl( ::e2::interface::i_rendernode_store_program_param * ) override;

                                           bool renderdevice_process_impl( ::e2::interface::i_rendernode_query_persistent * ) override;
                                           bool renderdevice_process_impl( ::e2::interface::i_rendernode_query_attrib * ) override;

                                                GLuint _program;
                                          GLFWwindow * _window;
};

} }

#endif
