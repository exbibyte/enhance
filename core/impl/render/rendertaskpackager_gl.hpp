#ifndef E2_RENDERTASKPACKAGER_GL_HPP
#define E2_RENDERTASKPACKAGER_GL_HPP

#include "i_rendernode.hpp"
#include "i_renderpackage.hpp"
#include "buffer.hpp"

namespace e2 { namespace render {

class rendertaskpackager_gl {
public:
    bool init_window( ::e2::memory::buffer *, ::e2::interface::i_renderpackage ** pkg, ::e2::interface::i_rendernode_init_window * n );
    bool init_program( ::e2::memory::buffer *, ::e2::interface::i_renderpackage ** pkg, ::e2::interface::i_rendernode_init_program * n );
    bool init_buffer( ::e2::memory::buffer *, ::e2::interface::i_renderpackage ** pkg, ::e2::interface::i_rendernode_init_buffer * n );
    bool init_object( ::e2::memory::buffer *, ::e2::interface::i_renderpackage ** pkg, ::e2::interface::i_rendernode_init_object * n );

    bool deinit_window( ::e2::memory::buffer *, ::e2::interface::i_renderpackage ** pkg, ::e2::interface::i_rendernode_deinit_window * n );
    bool deinit_program( ::e2::memory::buffer *, ::e2::interface::i_renderpackage ** pkg, ::e2::interface::i_rendernode_deinit_program * n );
    bool deinit_buffer( ::e2::memory::buffer *, ::e2::interface::i_renderpackage ** pkg, ::e2::interface::i_rendernode_deinit_buffer * n );

    bool query_attrib( ::e2::memory::buffer *, ::e2::interface::i_renderpackage ** pkg, ::e2::interface::i_rendernode_query_attrib * n );
    bool query_persistent( ::e2::memory::buffer *, ::e2::interface::i_renderpackage ** pkg, ::e2::interface::i_rendernode_query_persistent * n );
    
    bool load_shader( ::e2::memory::buffer *, ::e2::interface::i_renderpackage ** pkg, ::e2::interface::i_rendernode_load_shader * n );

    bool store_buffer( ::e2::memory::buffer *, ::e2::interface::i_renderpackage ** pkg, ::e2::interface::i_rendernode_store_buffer * n );
    bool store_defineformat_vertexattrib( ::e2::memory::buffer *, ::e2::interface::i_renderpackage ** pkg, ::e2::interface::i_rendernode_store_defineformat_vertexattrib * n );

    bool bind_shader( ::e2::memory::buffer *, ::e2::interface::i_renderpackage ** pkg, ::e2::interface::i_rendernode_bind_shader * n );
    bool bind_program( ::e2::memory::buffer *, ::e2::interface::i_renderpackage ** pkg, ::e2::interface::i_rendernode_bind_program * n );
    bool bind_attrib( ::e2::memory::buffer *, ::e2::interface::i_renderpackage ** pkg, ::e2::interface::i_rendernode_bind_attrib * n );
    bool bind_buffer( ::e2::memory::buffer *, ::e2::interface::i_renderpackage ** pkg, ::e2::interface::i_rendernode_bind_buffer * n );
    bool bind_object( ::e2::memory::buffer *, ::e2::interface::i_renderpackage ** pkg, ::e2::interface::i_rendernode_bind_object * n );

    bool compute_program( ::e2::memory::buffer *, ::e2::interface::i_renderpackage ** pkg, ::e2::interface::i_rendernode_compute_program * n );

    bool enable_attrib( ::e2::memory::buffer *, ::e2::interface::i_renderpackage ** pkg, ::e2::interface::i_rendernode_enable_attrib * n );

    bool exec_drawbatch( ::e2::memory::buffer *, ::e2::interface::i_renderpackage ** pkg, ::e2::interface::i_rendernode_exec_drawbatch * n );

    bool swap_window_buffer( ::e2::memory::buffer *, ::e2::interface::i_renderpackage ** pkg, ::e2::interface::i_rendernode_swap_window_buffer * n );
    bool clear_window_buffer_colour( ::e2::memory::buffer *, ::e2::interface::i_renderpackage ** pkg, ::e2::interface::i_rendernode_clear_window_buffer_colour * n );
    bool clear_window_buffer_depth( ::e2::memory::buffer *, ::e2::interface::i_renderpackage ** pkg , ::e2::interface::i_rendernode_clear_window_buffer_depth * n );
    bool disable_window_buffer_depth( ::e2::memory::buffer *, ::e2::interface::i_renderpackage ** pkg, ::e2::interface::i_rendernode_disable_window_buffer_depth * n );
    bool enable_window_buffer_depth( ::e2::memory::buffer *, ::e2::interface::i_renderpackage ** pkg, ::e2::interface::i_rendernode_enable_window_buffer_depth * n );
};

} }

#endif
