#ifndef E2_I_RENDERNODE_HPP
#define E2_I_RENDERNODE_HPP

#include <cstdint>
#include <cstring>
#include <list>
#include <utility>

#include "buffer.hpp"

namespace e2 { namespace interface {

class i_renderpackage;
class i_rendertaskpackager;
	
class i_rendernode {
public:
    uint64_t _handle;
    std::list< i_rendernode * > _inputs;
    std::list< i_rendernode * > _outputs;
    virtual bool package( ::e2::memory::buffer *, ::e2::interface::i_renderpackage ** pkg, ::e2::interface::i_rendertaskpackager * pkger ) = 0;
};

//init starts
class i_rendernode_init_window : public i_rendernode {
public:
    int _x;
    int _y;

    bool package( ::e2::memory::buffer * buf, ::e2::interface::i_renderpackage ** pkg, i_rendertaskpackager * pkger ) override;
};
class i_rendernode_init_program : public i_rendernode {
public:
    uint64_t * _program_handle;
    
    bool package( ::e2::memory::buffer * buf, ::e2::interface::i_renderpackage ** pkg, i_rendertaskpackager * pkger ) override;
};
class i_rendernode_init_buffer : public i_rendernode {
public:
    uint64_t * _num_buffers;
    uint64_t * _buffers;

    bool package( ::e2::memory::buffer * buf, ::e2::interface::i_renderpackage ** pkg, i_rendertaskpackager * pkger ) override;
};
class i_rendernode_init_object : public i_rendernode {
public:
    uint64_t _object_type;
    uint64_t * _num_objects;
    uint64_t * _handle_objects;

    bool package( ::e2::memory::buffer * buf, ::e2::interface::i_renderpackage ** pkg, i_rendertaskpackager * pkger ) override;
};

//init ends
//deinit starts
class i_rendernode_deinit_window : public i_rendernode {
public:
    bool package( ::e2::memory::buffer * buf, ::e2::interface::i_renderpackage ** pkg, i_rendertaskpackager * pkger ) override;
};
class i_rendernode_deinit_program : public i_rendernode {
public:
    uint64_t * _program_handle;
    bool package( ::e2::memory::buffer * buf, ::e2::interface::i_renderpackage ** pkg, i_rendertaskpackager * pkger ) override;
};
class i_rendernode_deinit_buffer : public i_rendernode {
public:
    uint64_t * _num_buffers;
    uint64_t * _buffers;
    bool package( ::e2::memory::buffer * buf, ::e2::interface::i_renderpackage ** pkg, i_rendertaskpackager * pkger ) override;
};
//deinit ends
//query starts
class i_rendernode_query_attrib : public i_rendernode {
public:
    uint64_t * _program_handle;

    bool package( ::e2::memory::buffer * buf, ::e2::interface::i_renderpackage ** pkg, i_rendertaskpackager * pkger ) override;
};
class i_rendernode_query_persistent : public i_rendernode {
public:
    uint64_t * _program_handle;

    bool package( ::e2::memory::buffer * buf, ::e2::interface::i_renderpackage ** pkg, i_rendertaskpackager * pkger ) override;
};
//query ends
//load starts
class i_rendernode_load_shader : public i_rendernode {
public:
    uint64_t * _shader_handle;
    uint64_t * _shader_type;
        char * _source;
      uint64_t _source_type;

    bool package( ::e2::memory::buffer * buf, ::e2::interface::i_renderpackage ** pkg, i_rendertaskpackager * pkger ) override;
};
//load ends
//store starts
class i_rendernode_store_buffer : public i_rendernode {
public:
    uint64_t * _buffer_type;
    uint64_t * _buffer_size;
        void * _buffer_data;
    uint64_t * _buffer_usage;

    bool package( ::e2::memory::buffer * buf, ::e2::interface::i_renderpackage ** pkg, i_rendertaskpackager * pkger ) override;
};
class i_rendernode_store_defineformat_vertexattrib : public i_rendernode {
public:
    uint64_t * _attrib_handle;
    uint64_t * _attrib_size;
    uint64_t * _attrib_data_type;
        bool * _attrib_normalized;
    uint64_t * _attrib_stride;
        void * _attrib_pointer;

    bool package( ::e2::memory::buffer * buf, ::e2::interface::i_renderpackage ** pkg, i_rendertaskpackager * pkger ) override;
};
//store ends
//bind starts
class i_rendernode_bind_shader : public i_rendernode {
public:
    uint64_t * _program_handle;
    uint64_t * _shader_handle;

    bool package( ::e2::memory::buffer * buf, ::e2::interface::i_renderpackage ** pkg, i_rendertaskpackager * pkger ) override;
};
class i_rendernode_bind_program : public i_rendernode {
public:
    uint64_t * _program_handle;

    bool package( ::e2::memory::buffer * buf, ::e2::interface::i_renderpackage ** pkg, i_rendertaskpackager * pkger ) override;
};
class i_rendernode_bind_attrib : public i_rendernode {
public:
    uint64_t * _program_handle;
    uint64_t _location_type;
    uint64_t * _index;
    char * _var_name;

    bool package( ::e2::memory::buffer * buf, ::e2::interface::i_renderpackage ** pkg, i_rendertaskpackager * pkger ) override;
};
class i_rendernode_bind_buffer : public i_rendernode {
public:
    uint64_t * _buffer_type;
    uint64_t * _buffer_handle;

    bool package( ::e2::memory::buffer * buf, ::e2::interface::i_renderpackage ** pkg, i_rendertaskpackager * pkger ) override;
};
class i_rendernode_bind_object : public i_rendernode {
public:
    uint64_t _object_type;
    uint64_t * _object_handle;

    bool package( ::e2::memory::buffer * buf, ::e2::interface::i_renderpackage ** pkg, i_rendertaskpackager * pkger ) override;
};
//bind ends
//compute starts
class i_rendernode_compute_program : public i_rendernode {
public:
    uint64_t * _program_handle;

    bool package( ::e2::memory::buffer * buf, ::e2::interface::i_renderpackage ** pkg, i_rendertaskpackager * pkger ) override;
};
//compute ends
//enable starts
class i_rendernode_enable_attrib : public i_rendernode {
public:
    uint64_t _attrib_type;
    uint64_t * _attrib_handle;

    bool package( ::e2::memory::buffer * buf, ::e2::interface::i_renderpackage ** pkg, i_rendertaskpackager * pkger ) override;
};
//enable ends
//exec starts
class i_rendernode_exec_drawbatch : public i_rendernode {
public:
    uint64_t * _primitive_type;
    int64_t * _offset;
    uint64_t * _count;

    bool package( ::e2::memory::buffer * buf, ::e2::interface::i_renderpackage ** pkg, i_rendertaskpackager * pkger ) override;
};
//exec ends
//window specific starts
class i_rendernode_swap_window_buffer : public i_rendernode {
public:
    bool package( ::e2::memory::buffer * buf, ::e2::interface::i_renderpackage ** pkg, i_rendertaskpackager * pkger ) override;
};
class i_rendernode_clear_window_buffer_colour : public i_rendernode {
public:
    bool package( ::e2::memory::buffer * buf, ::e2::interface::i_renderpackage ** pkg, i_rendertaskpackager * pkger ) override;
};
class i_rendernode_clear_window_buffer_depth : public i_rendernode {
public:
    bool package( ::e2::memory::buffer * buf, ::e2::interface::i_renderpackage ** pkg, i_rendertaskpackager * pkger ) override;
};
class i_rendernode_disable_window_buffer_depth : public i_rendernode {
public:
    bool package( ::e2::memory::buffer * buf, ::e2::interface::i_renderpackage ** pkg, i_rendertaskpackager * pkger ) override;
};
class i_rendernode_enable_window_buffer_depth : public i_rendernode {
public:
    bool package( ::e2::memory::buffer * buf, ::e2::interface::i_renderpackage ** pkg, i_rendertaskpackager * pkger ) override;
};
//window specific ends
} }
	
#endif
