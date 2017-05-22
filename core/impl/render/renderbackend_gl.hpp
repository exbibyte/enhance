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
};

} }

#endif
