#ifndef E2_I_RENDERTASKPACKAGER_HPP
#define E2_I_RENDERTASKPACKAGER_HPP

#include <cstdint>
#include <cstring>

namespace e2 { namespace interface {

class i_rendertaskpackager {
public:
    virtual bool process( ::e2::memory::buffer *, ::e2::interface::i_renderpackage ** pkg, ::e2::interface::i_rendernode_init_window * n ) = 0;
    virtual bool process( ::e2::memory::buffer *, ::e2::interface::i_renderpackage ** pkg, ::e2::interface::i_rendernode_init_program * n ) = 0;
    virtual bool process( ::e2::memory::buffer *, ::e2::interface::i_renderpackage ** pkg, ::e2::interface::i_rendernode_init_buffer * n ) = 0;
    virtual bool process( ::e2::memory::buffer *, ::e2::interface::i_renderpackage ** pkg, ::e2::interface::i_rendernode_init_object * n ) = 0;

    virtual bool process( ::e2::memory::buffer *, ::e2::interface::i_renderpackage ** pkg, ::e2::interface::i_rendernode_deinit_window * n ) = 0;
    virtual bool process( ::e2::memory::buffer *, ::e2::interface::i_renderpackage ** pkg, ::e2::interface::i_rendernode_deinit_program * n ) = 0;
    virtual bool process( ::e2::memory::buffer *, ::e2::interface::i_renderpackage ** pkg, ::e2::interface::i_rendernode_deinit_buffer * n ) = 0;

    virtual bool process( ::e2::memory::buffer *, ::e2::interface::i_renderpackage ** pkg, ::e2::interface::i_rendernode_query_attrib * n ) = 0;
    virtual bool process( ::e2::memory::buffer *, ::e2::interface::i_renderpackage ** pkg, ::e2::interface::i_rendernode_query_persistent * n ) = 0;
    
    virtual bool process( ::e2::memory::buffer *, ::e2::interface::i_renderpackage ** pkg, ::e2::interface::i_rendernode_load_shader * n ) = 0;

    virtual bool process( ::e2::memory::buffer *, ::e2::interface::i_renderpackage ** pkg, ::e2::interface::i_rendernode_store_buffer * n ) = 0;
    virtual bool process( ::e2::memory::buffer *, ::e2::interface::i_renderpackage ** pkg, ::e2::interface::i_rendernode_store_defineformat_vertexattrib * n ) = 0;

    virtual bool process( ::e2::memory::buffer *, ::e2::interface::i_renderpackage ** pkg, ::e2::interface::i_rendernode_bind_shader * n ) = 0;
    virtual bool process( ::e2::memory::buffer *, ::e2::interface::i_renderpackage ** pkg, ::e2::interface::i_rendernode_bind_program * n ) = 0;
    virtual bool process( ::e2::memory::buffer *, ::e2::interface::i_renderpackage ** pkg, ::e2::interface::i_rendernode_bind_attrib * n ) = 0;
    virtual bool process( ::e2::memory::buffer *, ::e2::interface::i_renderpackage ** pkg, ::e2::interface::i_rendernode_bind_buffer * n ) = 0;
    virtual bool process( ::e2::memory::buffer *, ::e2::interface::i_renderpackage ** pkg, ::e2::interface::i_rendernode_bind_object * n ) = 0;

    virtual bool process( ::e2::memory::buffer *, ::e2::interface::i_renderpackage ** pkg, ::e2::interface::i_rendernode_compute_program * n ) = 0;

    virtual bool process( ::e2::memory::buffer *, ::e2::interface::i_renderpackage ** pkg, ::e2::interface::i_rendernode_enable_attrib * n ) = 0;

    virtual bool process( ::e2::memory::buffer *, ::e2::interface::i_renderpackage ** pkg, ::e2::interface::i_rendernode_exec_drawbatch * n ) = 0;

    virtual bool process( ::e2::memory::buffer *, ::e2::interface::i_renderpackage ** pkg, ::e2::interface::i_rendernode_swap_window_buffer * n ) = 0;
    virtual bool process( ::e2::memory::buffer *, ::e2::interface::i_renderpackage ** pkg, ::e2::interface::i_rendernode_clear_window_buffer_colour * n ) = 0;
    virtual bool process( ::e2::memory::buffer *, ::e2::interface::i_renderpackage ** pkg , ::e2::interface::i_rendernode_clear_window_buffer_depth * n ) = 0;
    virtual bool process( ::e2::memory::buffer *, ::e2::interface::i_renderpackage ** pkg, ::e2::interface::i_rendernode_disable_window_buffer_depth * n ) = 0;
    virtual bool process( ::e2::memory::buffer *, ::e2::interface::i_renderpackage ** pkg, ::e2::interface::i_rendernode_enable_window_buffer_depth * n ) = 0;
};

} }

#endif
