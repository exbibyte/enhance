#include <cassert>

#include "renderpackage_gl.hpp"
#include "renderbackend_gl.hpp"
#include "buffer.hpp"

namespace e2 { namespace render {

bool renderbackend_gl::init_window( ::e2::memory::buffer * buf, ::e2::interface::i_renderpackage ** pkg, int x, int y ){
    //set window size
    void ** data_ptr;
    {
	int * data;
	size_t offset;
	if( false == buf->buffer_get_next_available( &offset, &data, 2 ) ){
	    assert( false && "mem buffer for init window failed." );
	    return false;
	}
	int * data_start = data;
	*data++ = x;
	*data++ = y;
	if( false == buf->buffer_get_next_available( &offset, &data_ptr, 2 ) ){
	    assert( false && "mem buffer for init window failed." );
	    return false;
	}
	data_ptr[0] = data_start;
	data_ptr[1] = &data_start[1];
	uint64_t * key;
	if( false == buf->buffer_get_next_available( &offset, &key, 2 ) ){
	    assert( false && "mem buffer for init window failed." );
	    return false;
	}
	key[0] = ::e2::interface::e_renderresourcekey_wind_dimx;
	key[1] = ::e2::interface::e_renderresourcekey_wind_dimy;
	assert( ::e2::render::renderpackage_gl::pack( buf, pkg, key, (void**)data_ptr, 2 ) );
	(*pkg)->set_render_cmd_type( ::e2::interface::e_rendercmd_type_init );
	(*pkg)->set_render_resource_type( ::e2::interface::e_renderresource_type_windowing );
	(*pkg)->_resource_subtype = ::e2::interface::e_renderresource_subtype_na;
    }
    return true;
}
bool renderbackend_gl::swap_window_buffer( ::e2::memory::buffer * buf, ::e2::interface::i_renderpackage ** pkg ){
    assert( ::e2::render::renderpackage_gl::pack( buf, pkg, nullptr, nullptr, 0 ) );
    (*pkg)->set_render_cmd_type( ::e2::interface::e_rendercmd_type_exec );
    (*pkg)->set_render_resource_type( ::e2::interface::e_renderresource_type_windowing );
    (*pkg)->_resource_subtype = ::e2::interface::e_renderresource_subtype_window_buf_swap;
    return true;
}
bool renderbackend_gl::deinit_window( ::e2::memory::buffer * buf, ::e2::interface::i_renderpackage ** pkg ){
    assert( ::e2::render::renderpackage_gl::pack( buf, pkg, nullptr, nullptr, 0 ) );
    (*pkg)->set_render_cmd_type( ::e2::interface::e_rendercmd_type_deinit );
    (*pkg)->set_render_resource_type( ::e2::interface::e_renderresource_type_windowing );
    (*pkg)->_resource_subtype = ::e2::interface::e_renderresource_subtype_na;
    return true;
}
bool renderbackend_gl::init_program( ::e2::memory::buffer * buf, ::e2::interface::i_renderpackage ** pkg, uint64_t * program_handle ){
    size_t offset;
    uint64_t * key;
    if( false == buf->buffer_get_next_available( &offset, &key, 1 ) ){
	assert( false && "mem buffer for init window failed." );
    }
    key[0] = ::e2::interface::e_renderresourcekey_program_handle;
    assert( ::e2::render::renderpackage_gl::pack( buf, pkg, key, (void**)&program_handle, 1 ) );
    (*pkg)->set_render_cmd_type( ::e2::interface::e_rendercmd_type_init );
    (*pkg)->set_render_resource_type( ::e2::interface::e_renderresource_type_program );
    return true;
}
bool renderbackend_gl::deinit_program( ::e2::memory::buffer * buf, ::e2::interface::i_renderpackage ** pkg, uint64_t * program_handle ){
    size_t offset;
    uint64_t * key;
    if( false == buf->buffer_get_next_available( &offset, &key, 1 ) ){
	assert( false && "mem buffer for init window failed." );
    }
    uint64_t ** p_h;
    if( false == buf->buffer_get_next_available( &offset, &p_h, 1 ) ){
	assert( false && "mem buffer failed." );
    }
    p_h[0] = program_handle;
    key[0] = ::e2::interface::e_renderresourcekey_program_handle;
    assert( ::e2::render::renderpackage_gl::pack( buf, pkg, key, (void**)p_h, 1 ) );
    (*pkg)->set_render_cmd_type( ::e2::interface::e_rendercmd_type_deinit );
    (*pkg)->set_render_resource_type( ::e2::interface::e_renderresource_type_program );
    return true;
}
bool renderbackend_gl::load_shader( ::e2::memory::buffer * buf, ::e2::interface::i_renderpackage ** pkg, uint64_t * shader_handle, uint64_t * shader_type, char * source, uint64_t source_type ){
    size_t offset;
    uint64_t ** p_shader_handle;
    if( false == buf->buffer_get_next_available( &offset, &p_shader_handle, 1 ) ){
	assert( false && "mem buffer for loading vertex shader failed." );
    }
    p_shader_handle[0] = shader_handle;
    uint64_t ** p_shader_type;
    if( false == buf->buffer_get_next_available( &offset, &p_shader_type, 1 ) ){
	assert( false && "mem buffer for loading vertex shader failed." );
    }
    p_shader_type[0] = shader_type;
    char ** p_shader_source;
    if( false == buf->buffer_get_next_available( &offset, &p_shader_source, 1 ) ){
	assert( false && "mem buffer for loading vertex shader failed." );
    }
    p_shader_source[0] = source;
    uint64_t * key;
    if( false == buf->buffer_get_next_available( &offset, &key, 3 ) ){
	assert( false && "mem buffer for loading vertex shader failed." );
    }
    key[0] = ::e2::interface::e_renderresourcekey_shader_handle;
    key[1] = ::e2::interface::e_renderresourcekey_shader_type;
    key[2] = ::e2::interface::e_renderresourcekey_shader_source;
    assert( ::e2::render::renderpackage_gl::pack( buf, pkg, key, (void**)p_shader_handle, 3 ) );
    (*pkg)->set_render_cmd_type( ::e2::interface::e_rendercmd_type_load );
    (*pkg)->set_render_resource_type( ::e2::interface::e_renderresource_type_shader );
    (*pkg)->_resource_subtype = source_type;
    return true;
}
bool renderbackend_gl::bind_shader( ::e2::memory::buffer * buf, ::e2::interface::i_renderpackage ** pkg, uint64_t * program_handle, uint64_t * shader_handle ){
    size_t offset;
    uint64_t ** p_data;
    if( false == buf->buffer_get_next_available( &offset, &p_data, 2 ) ){
	assert( false && "mem buffer for loading vertex shader failed." );
    }
    p_data[0]= program_handle;
    p_data[1] = shader_handle;
    uint64_t * key;
    if( false == buf->buffer_get_next_available( &offset, &key, 2 ) ){
	assert( false && "mem buffer for loading vertex shader failed." );
    }
    key[0] = ::e2::interface::e_renderresourcekey_program_handle;
    key[1] = ::e2::interface::e_renderresourcekey_shader_handle;
    assert( ::e2::render::renderpackage_gl::pack( buf, pkg, key, (void**)p_data, 2 ) );
    (*pkg)->set_render_cmd_type( ::e2::interface::e_rendercmd_type_bind );
    (*pkg)->set_render_resource_type( ::e2::interface::e_renderresource_type_shader );
    return true;
}
bool renderbackend_gl::bind_program( ::e2::memory::buffer * buf, ::e2::interface::i_renderpackage ** pkg, uint64_t * program_handle ){
    size_t offset;
    uint64_t ** p_data;
    if( false == buf->buffer_get_next_available( &offset, &p_data, 1 ) ){
	assert( false && "mem buffer for loading vertex shader failed." );
    }
    p_data[0]= program_handle;
    uint64_t * key;
    if( false == buf->buffer_get_next_available( &offset, &key, 1 ) ){
	assert( false && "mem buffer for loading vertex shader failed." );
    }
    key[0] = ::e2::interface::e_renderresourcekey_program_handle;
    assert( ::e2::render::renderpackage_gl::pack( buf, pkg, key, (void**)p_data, 1 ) );
    (*pkg)->set_render_cmd_type( ::e2::interface::e_rendercmd_type_bind );
    (*pkg)->set_render_resource_type( ::e2::interface::e_renderresource_type_program );
    return true;
}

} }
