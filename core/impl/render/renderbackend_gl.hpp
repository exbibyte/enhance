#ifndef RENDERBACKEND_GL_HPP
#define RENDERBACKEND_GL_HPP

#include "i_renderpackage.hpp"
#include "buffer.hpp"

namespace e2 { namespace render {

class renderbackend_gl {
public:
    bool process(){ return false; };
    bool init_window( ::e2::memory::buffer *, ::e2::interface::i_renderpackage ** pkg, int x, int y );
    bool swap_window_buffer( ::e2::memory::buffer *, ::e2::interface::i_renderpackage ** pkg );
    bool deinit_window( ::e2::memory::buffer *, ::e2::interface::i_renderpackage ** pkg );
    bool init_program( ::e2::memory::buffer *, ::e2::interface::i_renderpackage ** pkg, uint64_t * program_handle );
    bool deinit_program( ::e2::memory::buffer *, ::e2::interface::i_renderpackage ** pkg, uint64_t * program_handle );
    bool load_shader( ::e2::memory::buffer *, ::e2::interface::i_renderpackage ** pkg, uint64_t * shader_handle, uint64_t * shader_type, char * source, uint64_t source_type );
    bool bind_shader( ::e2::memory::buffer *, ::e2::interface::i_renderpackage ** pkg, uint64_t * program_handle, uint64_t * shader_handle );
    bool bind_program( ::e2::memory::buffer *, ::e2::interface::i_renderpackage ** pkg, uint64_t * program_handle );
    bool query_attrib( ::e2::memory::buffer *, ::e2::interface::i_renderpackage ** pkg, uint64_t * program_handle );
    bool query_persistent( ::e2::memory::buffer *, ::e2::interface::i_renderpackage ** pkg, uint64_t * program_handle );
    bool bind_attrib( ::e2::memory::buffer *, ::e2::interface::i_renderpackage ** pkg, uint64_t * program_handle, uint64_t location_type, uint64_t * index, char * var_name );
    bool compute_program( ::e2::memory::buffer *, ::e2::interface::i_renderpackage ** pkg, uint64_t * program_handle );
    bool init_buffer( ::e2::memory::buffer *, ::e2::interface::i_renderpackage ** pkg, uint64_t * num_buf, uint64_t * buffers );
    bool bind_buffer( ::e2::memory::buffer *, ::e2::interface::i_renderpackage ** pkg, uint64_t * buffer_type, uint64_t * buffers_handle );
    bool store_buffer( ::e2::memory::buffer *, ::e2::interface::i_renderpackage ** pkg, uint64_t * buffer_type, uint64_t * buffer_size, void * data, uint64_t * buffer_usage );
    bool init_object( ::e2::memory::buffer *, ::e2::interface::i_renderpackage ** pkg, uint64_t obj_type, uint64_t * num_obj, uint64_t * handle_objects );
    bool bind_object( ::e2::memory::buffer *, ::e2::interface::i_renderpackage ** pkg, uint64_t obj_type, uint64_t * handle_objects );
    bool enable_attrib( ::e2::memory::buffer *, ::e2::interface::i_renderpackage ** pkg, uint64_t attrib_type, uint64_t * attrib_handle );
    bool store_defineformat_vertexattrib( ::e2::memory::buffer *, ::e2::interface::i_renderpackage ** pkg, uint64_t * attrib_handle, uint64_t * attrib_size, uint64_t * attrib_data_type, bool * normalized, uint64_t * stride, void * pointer );
    bool exec_drawbatch( ::e2::memory::buffer *, ::e2::interface::i_renderpackage ** pkg, uint64_t *primitive_type , int64_t * offset, uint64_t * count );
};

} }

#endif
