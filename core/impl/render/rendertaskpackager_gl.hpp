#ifndef E2_RENDERTASKPACKAGER_GL_HPP
#define E2_RENDERTASKPACKAGER_GL_HPP

#include "i_rendernode.hpp"
#include "i_renderpackage.hpp"
#include "i_rendertaskpackager.hpp"
#include "buffer.hpp"

namespace e2 { namespace render {

class rendertaskpackager_gl : public ::e2::interface::i_rendertaskpackager {
public:
    bool process( ::e2::memory::buffer * buf, ::e2::interface::i_renderpackage ** pkg, ::e2::interface::i_rendernode * n ) override;

    bool process( ::e2::memory::buffer *, ::e2::interface::i_renderpackage ** pkg, ::e2::interface::i_rendernode_init_window * n ) override;
    bool process( ::e2::memory::buffer *, ::e2::interface::i_renderpackage ** pkg, ::e2::interface::i_rendernode_init_program * n ) override;
    bool process( ::e2::memory::buffer *, ::e2::interface::i_renderpackage ** pkg, ::e2::interface::i_rendernode_init_buffer * n ) override;
    bool process( ::e2::memory::buffer *, ::e2::interface::i_renderpackage ** pkg, ::e2::interface::i_rendernode_init_object * n ) override;

    bool process( ::e2::memory::buffer *, ::e2::interface::i_renderpackage ** pkg, ::e2::interface::i_rendernode_deinit_window * n ) override;
    bool process( ::e2::memory::buffer *, ::e2::interface::i_renderpackage ** pkg, ::e2::interface::i_rendernode_deinit_program * n ) override;
    bool process( ::e2::memory::buffer *, ::e2::interface::i_renderpackage ** pkg, ::e2::interface::i_rendernode_deinit_buffer * n ) override;

    bool process( ::e2::memory::buffer *, ::e2::interface::i_renderpackage ** pkg, ::e2::interface::i_rendernode_query_attrib * n ) override;
    bool process( ::e2::memory::buffer *, ::e2::interface::i_renderpackage ** pkg, ::e2::interface::i_rendernode_query_persistent * n ) override;
    
    bool process( ::e2::memory::buffer *, ::e2::interface::i_renderpackage ** pkg, ::e2::interface::i_rendernode_load_shader * n ) override;

    bool process( ::e2::memory::buffer *, ::e2::interface::i_renderpackage ** pkg, ::e2::interface::i_rendernode_store_buffer * n ) override;
    bool process( ::e2::memory::buffer *, ::e2::interface::i_renderpackage ** pkg, ::e2::interface::i_rendernode_store_defineformat_vertexattrib * n ) override;

    bool process( ::e2::memory::buffer *, ::e2::interface::i_renderpackage ** pkg, ::e2::interface::i_rendernode_bind_shader * n ) override;
    bool process( ::e2::memory::buffer *, ::e2::interface::i_renderpackage ** pkg, ::e2::interface::i_rendernode_bind_program * n ) override;
    bool process( ::e2::memory::buffer *, ::e2::interface::i_renderpackage ** pkg, ::e2::interface::i_rendernode_bind_attrib * n ) override;
    bool process( ::e2::memory::buffer *, ::e2::interface::i_renderpackage ** pkg, ::e2::interface::i_rendernode_bind_buffer * n ) override;
    bool process( ::e2::memory::buffer *, ::e2::interface::i_renderpackage ** pkg, ::e2::interface::i_rendernode_bind_object * n ) override;

    bool process( ::e2::memory::buffer *, ::e2::interface::i_renderpackage ** pkg, ::e2::interface::i_rendernode_compute_program * n ) override;

    bool process( ::e2::memory::buffer *, ::e2::interface::i_renderpackage ** pkg, ::e2::interface::i_rendernode_enable_attrib * n ) override;

    bool process( ::e2::memory::buffer *, ::e2::interface::i_renderpackage ** pkg, ::e2::interface::i_rendernode_exec_drawbatch * n ) override;

    bool process( ::e2::memory::buffer *, ::e2::interface::i_renderpackage ** pkg, ::e2::interface::i_rendernode_swap_window_buffer * n ) override;
    bool process( ::e2::memory::buffer *, ::e2::interface::i_renderpackage ** pkg, ::e2::interface::i_rendernode_clear_window_buffer_colour * n ) override;
    bool process( ::e2::memory::buffer *, ::e2::interface::i_renderpackage ** pkg , ::e2::interface::i_rendernode_clear_window_buffer_depth * n ) override;
    bool process( ::e2::memory::buffer *, ::e2::interface::i_renderpackage ** pkg, ::e2::interface::i_rendernode_disable_window_buffer_depth * n ) override;
    bool process( ::e2::memory::buffer *, ::e2::interface::i_renderpackage ** pkg, ::e2::interface::i_rendernode_enable_window_buffer_depth * n ) override;
};

} }

#endif
