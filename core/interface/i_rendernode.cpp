#include <cstdint>
#include <cstring>
#include <list>
#include <utility>

#include "i_rendernode.hpp"
#include "i_rendertaskpackager.hpp"
#include "i_renderpackage.hpp"
#include "buffer.hpp"

namespace e2 { namespace interface {

//init starts
bool i_rendernode_init_window::package( ::e2::memory::buffer * buf, ::e2::interface::i_renderpackage ** pkg, i_rendertaskpackager * pkger ){
    return pkger->process( buf, pkg, this );
}

bool i_rendernode_init_program::package( ::e2::memory::buffer * buf, ::e2::interface::i_renderpackage ** pkg, i_rendertaskpackager * pkger ){
    return pkger->process( buf, pkg, this );
}

bool i_rendernode_init_buffer::package( ::e2::memory::buffer * buf, ::e2::interface::i_renderpackage ** pkg, i_rendertaskpackager * pkger ){
    return pkger->process( buf, pkg, this );
}

bool i_rendernode_init_object::package( ::e2::memory::buffer * buf, ::e2::interface::i_renderpackage ** pkg, i_rendertaskpackager * pkger ){
    return pkger->process( buf, pkg, this );
}

//init ends
//deinit starts
bool i_rendernode_deinit_window::package( ::e2::memory::buffer * buf, ::e2::interface::i_renderpackage ** pkg, i_rendertaskpackager * pkger ){
    return pkger->process( buf, pkg, this );
}

bool i_rendernode_deinit_program::package( ::e2::memory::buffer * buf, ::e2::interface::i_renderpackage ** pkg, i_rendertaskpackager * pkger ){
    return pkger->process( buf, pkg, this );
}

bool i_rendernode_deinit_buffer::package( ::e2::memory::buffer * buf, ::e2::interface::i_renderpackage ** pkg, i_rendertaskpackager * pkger ){
    return pkger->process( buf, pkg, this );
}

//deinit ends
//query starts
bool i_rendernode_query_attrib::package( ::e2::memory::buffer * buf, ::e2::interface::i_renderpackage ** pkg, i_rendertaskpackager * pkger ){
    return pkger->process( buf, pkg, this );
}

bool i_rendernode_query_persistent::package( ::e2::memory::buffer * buf, ::e2::interface::i_renderpackage ** pkg, i_rendertaskpackager * pkger ){
    return pkger->process( buf, pkg, this );
}

//query ends
//load starts
bool i_rendernode_load_shader::package( ::e2::memory::buffer * buf, ::e2::interface::i_renderpackage ** pkg, i_rendertaskpackager * pkger ){
    return pkger->process( buf, pkg, this );
}
//load ends
//store starts
bool i_rendernode_store_buffer::package( ::e2::memory::buffer * buf, ::e2::interface::i_renderpackage ** pkg, i_rendertaskpackager * pkger ){
    return pkger->process( buf, pkg, this );
}

bool i_rendernode_store_defineformat_vertexattrib::package( ::e2::memory::buffer * buf, ::e2::interface::i_renderpackage ** pkg, i_rendertaskpackager * pkger ){
    return pkger->process( buf, pkg, this );
}
//store ends
//bind starts
bool i_rendernode_bind_shader::package( ::e2::memory::buffer * buf, ::e2::interface::i_renderpackage ** pkg, i_rendertaskpackager * pkger ){
    return pkger->process( buf, pkg, this );
}

bool i_rendernode_bind_program::package( ::e2::memory::buffer * buf, ::e2::interface::i_renderpackage ** pkg, i_rendertaskpackager * pkger ){
    return pkger->process( buf, pkg, this );
}

bool i_rendernode_bind_attrib::package( ::e2::memory::buffer * buf, ::e2::interface::i_renderpackage ** pkg, i_rendertaskpackager * pkger ){
    return pkger->process( buf, pkg, this );
}

bool i_rendernode_bind_buffer::package( ::e2::memory::buffer * buf, ::e2::interface::i_renderpackage ** pkg, i_rendertaskpackager * pkger ){
    return pkger->process( buf, pkg, this );
}

bool i_rendernode_bind_object::package( ::e2::memory::buffer * buf, ::e2::interface::i_renderpackage ** pkg, i_rendertaskpackager * pkger ){
    return pkger->process( buf, pkg, this );
}

//bind ends
//compute starts
bool i_rendernode_compute_program::package( ::e2::memory::buffer * buf, ::e2::interface::i_renderpackage ** pkg, i_rendertaskpackager * pkger ){
    return pkger->process( buf, pkg, this );
}

//compute ends
//enable starts
bool i_rendernode_enable_attrib::package( ::e2::memory::buffer * buf, ::e2::interface::i_renderpackage ** pkg, i_rendertaskpackager * pkger ){
    return pkger->process( buf, pkg, this );
}

//enable ends
//exec starts
bool i_rendernode_exec_drawbatch::package( ::e2::memory::buffer * buf, ::e2::interface::i_renderpackage ** pkg, i_rendertaskpackager * pkger ){
    return pkger->process( buf, pkg, this );
}

//exec ends
//window specific starts
bool i_rendernode_swap_window_buffer::package( ::e2::memory::buffer * buf, ::e2::interface::i_renderpackage ** pkg, i_rendertaskpackager * pkger ){
    return pkger->process( buf, pkg, this );
}

bool i_rendernode_clear_window_buffer_colour::package( ::e2::memory::buffer * buf, ::e2::interface::i_renderpackage ** pkg, i_rendertaskpackager * pkger ){
    return pkger->process( buf, pkg, this );
}

bool i_rendernode_clear_window_buffer_depth::package( ::e2::memory::buffer * buf, ::e2::interface::i_renderpackage ** pkg, i_rendertaskpackager * pkger ){
    return pkger->process( buf, pkg, this );
}

bool i_rendernode_disable_window_buffer_depth::package( ::e2::memory::buffer * buf, ::e2::interface::i_renderpackage ** pkg, i_rendertaskpackager * pkger ){
    return pkger->process( buf, pkg, this );
}

bool i_rendernode_enable_window_buffer_depth::package( ::e2::memory::buffer * buf, ::e2::interface::i_renderpackage ** pkg, i_rendertaskpackager * pkger ){
    return pkger->process( buf, pkg, this );
}
//window specific ends
} }
