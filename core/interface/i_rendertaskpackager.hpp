#ifndef E2_I_RENDERTASKPACKAGER_HPP
#define E2_I_RENDERTASKPACKAGER_HPP

#include <cstdint>
#include <cstring>

#include "i_renderpackage.hpp"
#include "i_rendernode.hpp"

namespace e2 { namespace interface {

/**
Responsibilities:
 1. allocating i_renderpackage and its derivative using provided mem_manager and,
 2. set relevant data based on i_rendernode and its derivatives,
 3. return a pointer to the initialized i_renderpackage.
Notes: 
 1. specific render device shall derive from this interface and implement appropriate derivatives of i_renderpackage and set appropriate data.
**/
template< class mem_manager >
class i_rendertaskpackager {
public:
    virtual bool process( mem_manager *, ::e2::interface::i_renderpackage ** pkg, ::e2::interface::i_rendernode * n ) = 0;
    
    virtual bool process( mem_manager *, ::e2::interface::i_renderpackage ** pkg, ::e2::interface::i_rendernode_init_window * n ) = 0;
    virtual bool process( mem_manager *, ::e2::interface::i_renderpackage ** pkg, ::e2::interface::i_rendernode_init_program * n ) = 0;
    virtual bool process( mem_manager *, ::e2::interface::i_renderpackage ** pkg, ::e2::interface::i_rendernode_init_buffer * n ) = 0;
    virtual bool process( mem_manager *, ::e2::interface::i_renderpackage ** pkg, ::e2::interface::i_rendernode_init_object * n ) = 0;

    virtual bool process( mem_manager *, ::e2::interface::i_renderpackage ** pkg, ::e2::interface::i_rendernode_deinit_window * n ) = 0;
    virtual bool process( mem_manager *, ::e2::interface::i_renderpackage ** pkg, ::e2::interface::i_rendernode_deinit_program * n ) = 0;
    virtual bool process( mem_manager *, ::e2::interface::i_renderpackage ** pkg, ::e2::interface::i_rendernode_deinit_buffer * n ) = 0;

    virtual bool process( mem_manager *, ::e2::interface::i_renderpackage ** pkg, ::e2::interface::i_rendernode_query_attrib * n ) = 0;
    virtual bool process( mem_manager *, ::e2::interface::i_renderpackage ** pkg, ::e2::interface::i_rendernode_query_persistent * n ) = 0;
    
    virtual bool process( mem_manager *, ::e2::interface::i_renderpackage ** pkg, ::e2::interface::i_rendernode_load_shader * n ) = 0;

    virtual bool process( mem_manager *, ::e2::interface::i_renderpackage ** pkg, ::e2::interface::i_rendernode_store_buffer * n ) = 0;
    virtual bool process( mem_manager *, ::e2::interface::i_renderpackage ** pkg, ::e2::interface::i_rendernode_store_defineformat_vertexattrib * n ) = 0;

    virtual bool process( mem_manager *, ::e2::interface::i_renderpackage ** pkg, ::e2::interface::i_rendernode_bind_shader * n ) = 0;
    virtual bool process( mem_manager *, ::e2::interface::i_renderpackage ** pkg, ::e2::interface::i_rendernode_bind_program * n ) = 0;
    virtual bool process( mem_manager *, ::e2::interface::i_renderpackage ** pkg, ::e2::interface::i_rendernode_bind_attrib * n ) = 0;
    virtual bool process( mem_manager *, ::e2::interface::i_renderpackage ** pkg, ::e2::interface::i_rendernode_bind_buffer * n ) = 0;
    virtual bool process( mem_manager *, ::e2::interface::i_renderpackage ** pkg, ::e2::interface::i_rendernode_bind_object * n ) = 0;

    virtual bool process( mem_manager *, ::e2::interface::i_renderpackage ** pkg, ::e2::interface::i_rendernode_compute_program * n ) = 0;

    virtual bool process( mem_manager *, ::e2::interface::i_renderpackage ** pkg, ::e2::interface::i_rendernode_enable_attrib * n ) = 0;

    virtual bool process( mem_manager *, ::e2::interface::i_renderpackage ** pkg, ::e2::interface::i_rendernode_exec_drawbatch * n ) = 0;

    virtual bool process( mem_manager *, ::e2::interface::i_renderpackage ** pkg, ::e2::interface::i_rendernode_swap_window_buffer * n ) = 0;
    virtual bool process( mem_manager *, ::e2::interface::i_renderpackage ** pkg, ::e2::interface::i_rendernode_clear_window_buffer_colour * n ) = 0;
    virtual bool process( mem_manager *, ::e2::interface::i_renderpackage ** pkg , ::e2::interface::i_rendernode_clear_window_buffer_depth * n ) = 0;
    virtual bool process( mem_manager *, ::e2::interface::i_renderpackage ** pkg, ::e2::interface::i_rendernode_disable_window_buffer_depth * n ) = 0;
    virtual bool process( mem_manager *, ::e2::interface::i_renderpackage ** pkg, ::e2::interface::i_rendernode_enable_window_buffer_depth * n ) = 0;
};

} }

#endif
