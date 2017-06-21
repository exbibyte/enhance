#ifndef E2_I_RENDERDEVICE_HPP
#define E2_I_RENDERDEVICE_HPP

#include <utility>
#include <functional>
#include <cstdint>

#include "i_rendernode.hpp"

namespace e2 { namespace interface {

class i_renderdevice {
public:
    // virtual bool renderdevice_process_impl( ::e2::interface::i_rendernode * n ) = 0;
                                           virtual bool renderdevice_process_impl( ::e2::interface::i_rendernode * n ){ return false; }
                                           virtual bool renderdevice_process_impl( ::e2::interface::i_rendernode_init_window * ){ return false; }
                                           virtual bool renderdevice_process_impl( ::e2::interface::i_rendernode_init_program * ){ return false; }
                                           virtual bool renderdevice_process_impl( ::e2::interface::i_rendernode_init_object_vertex_array * ){ return false; }
                                           virtual bool renderdevice_process_impl( ::e2::interface::i_rendernode_init_buffer * ){ return false; }
    
                                           virtual bool renderdevice_process_impl( ::e2::interface::i_rendernode_deinit_window * ){ return false; }
                                           virtual bool renderdevice_process_impl( ::e2::interface::i_rendernode_deinit_object_vertex_array * ){ return false; }
                                           virtual bool renderdevice_process_impl( ::e2::interface::i_rendernode_deinit_buffer * ){ return false; }
                                           virtual bool renderdevice_process_impl( ::e2::interface::i_rendernode_deinit_program * ){ return false; }
    
                                           virtual bool renderdevice_process_impl( ::e2::interface::i_rendernode_swap_window_buffer * ){ return false; }
                                           virtual bool renderdevice_process_impl( ::e2::interface::i_rendernode_clear_window_buffer_colour * ){ return false; }
                                           virtual bool renderdevice_process_impl( ::e2::interface::i_rendernode_clear_window_buffer_depth * ){ return false; }
                                           virtual bool renderdevice_process_impl( ::e2::interface::i_rendernode_disable_window_buffer_depth * ){ return false; }
                                           virtual bool renderdevice_process_impl( ::e2::interface::i_rendernode_enable_window_buffer_depth * ){ return false; }
    
                                           virtual bool renderdevice_process_impl( ::e2::interface::i_rendernode_exec_drawbatch * ){ return false; }
    
                                           virtual bool renderdevice_process_impl( ::e2::interface::i_rendernode_store_defineformat_vertexattrib * ){ return false; }
                                           virtual bool renderdevice_process_impl( ::e2::interface::i_rendernode_store_buffer * ){ return false; }
                                           virtual bool renderdevice_process_impl( ::e2::interface::i_rendernode_store_persistent_vecf * ){ return false; }
                                           virtual bool renderdevice_process_impl( ::e2::interface::i_rendernode_store_persistent_matf * ){ return false; }
                                           // virtual bool process_store_window( renderdevice_gl *, ::e2::interface::i_renderpackage p ){ return false; }
    
                                           virtual bool renderdevice_process_impl( ::e2::interface::i_rendernode_enable_attrib_vertex_array * ){ return false; }
    
                                           virtual bool renderdevice_process_impl( ::e2::interface::i_rendernode_disable_attrib_vertex_array * ){ return false; }

                                           virtual bool renderdevice_process_impl( ::e2::interface::i_rendernode_compute_program * ){ return false; }
    
                                           virtual bool renderdevice_process_impl( ::e2::interface::i_rendernode_bind_attrib_attrib_loc * ){ return false; }
                                           virtual bool renderdevice_process_impl( ::e2::interface::i_rendernode_bind_attrib_frag_loc * ){ return false; }
                                           virtual bool renderdevice_process_impl( ::e2::interface::i_rendernode_bind_shader * ){ return false; }
                                           virtual bool renderdevice_process_impl( ::e2::interface::i_rendernode_bind_object_vertex_array * ){ return false; }
                                           virtual bool renderdevice_process_impl( ::e2::interface::i_rendernode_bind_buffer * ){ return false; }
                                           virtual bool renderdevice_process_impl( ::e2::interface::i_rendernode_bind_program * ){ return false; }
    
                                           virtual bool renderdevice_process_impl( ::e2::interface::i_rendernode_unbind_shader * ){ return false; }

                                           virtual bool renderdevice_process_impl( ::e2::interface::i_rendernode_load_shader * ){ return false; }
                                           virtual bool renderdevice_process_impl( ::e2::interface::i_rendernode_store_program_param * ){ return false; }

                                           virtual bool renderdevice_process_impl( ::e2::interface::i_rendernode_query_persistent * ){ return false; }
                                           virtual bool renderdevice_process_impl( ::e2::interface::i_rendernode_query_attrib * ){ return false; }
};

} }

#endif
