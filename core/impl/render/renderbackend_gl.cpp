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
bool renderbackend_gl::clear_window_buffer_colour( ::e2::memory::buffer * buf, ::e2::interface::i_renderpackage ** pkg ){
    assert( ::e2::render::renderpackage_gl::pack( buf, pkg, nullptr, nullptr, 0 ) );
    (*pkg)->set_render_cmd_type( ::e2::interface::e_rendercmd_type_exec );
    (*pkg)->set_render_resource_type( ::e2::interface::e_renderresource_type_windowing );
    (*pkg)->_resource_subtype = ::e2::interface::e_renderresource_subtype_window_clear_colour;
    return true;
}
bool renderbackend_gl::clear_window_buffer_depth( ::e2::memory::buffer * buf, ::e2::interface::i_renderpackage ** pkg ){
    assert( ::e2::render::renderpackage_gl::pack( buf, pkg, nullptr, nullptr, 0 ) );
    (*pkg)->set_render_cmd_type( ::e2::interface::e_rendercmd_type_exec );
    (*pkg)->set_render_resource_type( ::e2::interface::e_renderresource_type_windowing );
    (*pkg)->_resource_subtype = ::e2::interface::e_renderresource_subtype_window_clear_depth;
    return true;
}
bool renderbackend_gl::disable_window_buffer_depth( ::e2::memory::buffer * buf, ::e2::interface::i_renderpackage ** pkg ){
    assert( ::e2::render::renderpackage_gl::pack( buf, pkg, nullptr, nullptr, 0 ) );
    (*pkg)->set_render_cmd_type( ::e2::interface::e_rendercmd_type_exec );
    (*pkg)->set_render_resource_type( ::e2::interface::e_renderresource_type_windowing );
    (*pkg)->_resource_subtype = ::e2::interface::e_renderresource_subtype_window_buffer_disable_depth;
    return true;
}
bool renderbackend_gl::enable_window_buffer_depth( ::e2::memory::buffer * buf, ::e2::interface::i_renderpackage ** pkg ){
    assert( ::e2::render::renderpackage_gl::pack( buf, pkg, nullptr, nullptr, 0 ) );
    (*pkg)->set_render_cmd_type( ::e2::interface::e_rendercmd_type_exec );
    (*pkg)->set_render_resource_type( ::e2::interface::e_renderresource_type_windowing );
    (*pkg)->_resource_subtype = ::e2::interface::e_renderresource_subtype_window_buffer_enable_depth;
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
    void ** p_data;
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
bool renderbackend_gl::query_attrib( ::e2::memory::buffer * buf, ::e2::interface::i_renderpackage ** pkg, uint64_t * program_handle ){
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
    (*pkg)->set_render_cmd_type( ::e2::interface::e_rendercmd_type_query );
    (*pkg)->set_render_resource_type( ::e2::interface::e_renderresource_type_attrib );
    return true;
}
bool renderbackend_gl::query_persistent( ::e2::memory::buffer * buf, ::e2::interface::i_renderpackage ** pkg, uint64_t * program_handle ){
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
    (*pkg)->set_render_cmd_type( ::e2::interface::e_rendercmd_type_query );
    (*pkg)->set_render_resource_type( ::e2::interface::e_renderresource_type_persistent );
    return true;
}
bool renderbackend_gl::bind_attrib( ::e2::memory::buffer * buf, ::e2::interface::i_renderpackage ** pkg, uint64_t * program_handle, uint64_t location_type, uint64_t * index, char * var_name ){
    size_t offset;
    void ** p_data;
    if( false == buf->buffer_get_next_available( &offset, &p_data, 3 ) ){
	assert( false && "mem buffer for loading vertex shader failed." );
    }
    p_data[0]= (void*) program_handle;
    p_data[1]= (void*) index;
    p_data[2]= (void*) var_name;
    uint64_t * key;
    if( false == buf->buffer_get_next_available( &offset, &key, 3 ) ){
	assert( false && "mem buffer for loading vertex shader failed." );
    }
    key[0] = ::e2::interface::e_renderresourcekey_program_handle;
    key[1] = ::e2::interface::e_renderresourcekey_attrib_index;
    key[2] = ::e2::interface::e_renderresourcekey_attrib_name;
    assert( ::e2::render::renderpackage_gl::pack( buf, pkg, key, (void**)p_data, 3 ) );
    (*pkg)->_resource_subtype = location_type; //expect e_renderresource_subtype_attrib_frag_location / e_renderresource_subtype_attrib_attrib_location
    (*pkg)->set_render_cmd_type( ::e2::interface::e_rendercmd_type_bind );
    (*pkg)->set_render_resource_type( ::e2::interface::e_renderresource_type_attrib );
    return true;    
}
bool renderbackend_gl::compute_program( ::e2::memory::buffer * buf, ::e2::interface::i_renderpackage ** pkg, uint64_t * program_handle ){
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
    (*pkg)->set_render_cmd_type( ::e2::interface::e_rendercmd_type_compute );
    (*pkg)->set_render_resource_type( ::e2::interface::e_renderresource_type_program );
    return true;
}
bool renderbackend_gl::init_buffer( ::e2::memory::buffer * buf, ::e2::interface::i_renderpackage ** pkg, uint64_t * num_buf, uint64_t * buffers ){
    size_t offset;
    void ** p_data;
    if( false == buf->buffer_get_next_available( &offset, &p_data, 2 ) ){
	assert( false && "mem buffer for loading vertex shader failed." );
    }
    p_data[0]= num_buf;
    p_data[1]= buffers;
    uint64_t * key;
    if( false == buf->buffer_get_next_available( &offset, &key, 2 ) ){
	assert( false && "mem buffer for loading vertex shader failed." );
    }
    key[0] = ::e2::interface::e_renderresourcekey_buffer_num;
    key[1] = ::e2::interface::e_renderresourcekey_buffer_handle;
    assert( ::e2::render::renderpackage_gl::pack( buf, pkg, key, (void**)p_data, 2 ) );
    (*pkg)->set_render_cmd_type( ::e2::interface::e_rendercmd_type_init );
    (*pkg)->set_render_resource_type( ::e2::interface::e_renderresource_type_buffer );
    return true;
}
bool renderbackend_gl::deinit_buffer( ::e2::memory::buffer * buf, ::e2::interface::i_renderpackage ** pkg, uint64_t * num_buf, uint64_t * buffers ){
    size_t offset;
    void ** p_data;
    if( false == buf->buffer_get_next_available( &offset, &p_data, 2 ) ){
	assert( false && "mem buffer for loading vertex shader failed." );
    }
    p_data[0]= num_buf;
    p_data[1]= buffers;
    uint64_t * key;
    if( false == buf->buffer_get_next_available( &offset, &key, 2 ) ){
	assert( false && "mem buffer for loading vertex shader failed." );
    }
    key[0] = ::e2::interface::e_renderresourcekey_buffer_num;
    key[1] = ::e2::interface::e_renderresourcekey_buffer_handle;
    assert( ::e2::render::renderpackage_gl::pack( buf, pkg, key, (void**)p_data, 2 ) );
    (*pkg)->set_render_cmd_type( ::e2::interface::e_rendercmd_type_deinit );
    (*pkg)->set_render_resource_type( ::e2::interface::e_renderresource_type_buffer );
    return true;
}
bool renderbackend_gl::bind_buffer( ::e2::memory::buffer * buf, ::e2::interface::i_renderpackage ** pkg, uint64_t * buffer_type, uint64_t * handle_buffers ){
    size_t offset;
    void ** p_data;
    if( false == buf->buffer_get_next_available( &offset, &p_data, 2 ) ){
	assert( false && "mem buffer for loading vertex shader failed." );
    }
    p_data[0]= buffer_type;
    p_data[1]= handle_buffers;
    uint64_t * key;
    if( false == buf->buffer_get_next_available( &offset, &key, 2 ) ){
	assert( false && "mem buffer for loading vertex shader failed." );
    }
    key[0] = ::e2::interface::e_renderresourcekey_buffer_type;
    key[1] = ::e2::interface::e_renderresourcekey_buffer_handle;
    assert( ::e2::render::renderpackage_gl::pack( buf, pkg, key, (void**)p_data, 2 ) );
    (*pkg)->set_render_cmd_type( ::e2::interface::e_rendercmd_type_bind );
    (*pkg)->set_render_resource_type( ::e2::interface::e_renderresource_type_buffer );
    return true;
}
bool renderbackend_gl::store_buffer( ::e2::memory::buffer * buf, ::e2::interface::i_renderpackage ** pkg, uint64_t * buffer_type, uint64_t * buffer_size, void * buffer_data, uint64_t * buffer_usage ){
    size_t offset;
    void ** p_data;
    if( false == buf->buffer_get_next_available( &offset, &p_data, 4 ) ){
	assert( false && "mem buffer for loading vertex shader failed." );
    }
    p_data[0]= buffer_type;
    p_data[1]= buffer_size;
    p_data[2]= buffer_data;
    p_data[3]= buffer_usage;
    uint64_t * key;
    if( false == buf->buffer_get_next_available( &offset, &key, 4 ) ){
	assert( false && "mem buffer for loading vertex shader failed." );
    }
    key[0] = ::e2::interface::e_renderresourcekey_buffer_type;
    key[1] = ::e2::interface::e_renderresourcekey_buffer_size;
    key[2] = ::e2::interface::e_renderresourcekey_buffer_data;
    key[3] = ::e2::interface::e_renderresourcekey_buffer_usage;
    assert( ::e2::render::renderpackage_gl::pack( buf, pkg, key, (void**)p_data, 4 ) );
    (*pkg)->set_render_cmd_type( ::e2::interface::e_rendercmd_type_store );
    (*pkg)->set_render_resource_type( ::e2::interface::e_renderresource_type_buffer );
    return true;
}
bool renderbackend_gl::init_object( ::e2::memory::buffer * buf, ::e2::interface::i_renderpackage ** pkg, uint64_t object_type, uint64_t * num_obj, uint64_t * handle_objects ){
    size_t offset;
    void ** p_data;
    if( false == buf->buffer_get_next_available( &offset, &p_data, 2 ) ){
	assert( false && "mem buffer for loading vertex shader failed." );
    }
    p_data[0]= num_obj;
    p_data[1]= handle_objects;
    uint64_t * key;
    if( false == buf->buffer_get_next_available( &offset, &key, 2 ) ){
	assert( false && "mem buffer for loading vertex shader failed." );
    }
    key[0] = ::e2::interface::e_renderresourcekey_va_num;
    key[1] = ::e2::interface::e_renderresourcekey_va_handle;
    assert( ::e2::render::renderpackage_gl::pack( buf, pkg, key, (void**)p_data, 2 ) );
    (*pkg)->_resource_subtype = object_type;
    (*pkg)->set_render_cmd_type( ::e2::interface::e_rendercmd_type_init );
    (*pkg)->set_render_resource_type( ::e2::interface::e_renderresource_type_object );
    return true;    
}
bool renderbackend_gl::bind_object( ::e2::memory::buffer * buf, ::e2::interface::i_renderpackage ** pkg, uint64_t object_type, uint64_t * handle_object ){
    size_t offset;
    void ** p_data;
    if( false == buf->buffer_get_next_available( &offset, &p_data, 1 ) ){
	assert( false && "mem buffer for loading vertex shader failed." );
    }
    p_data[0]= handle_object;
    uint64_t * key;
    if( false == buf->buffer_get_next_available( &offset, &key, 1 ) ){
	assert( false && "mem buffer for loading vertex shader failed." );
    }
    key[0] = ::e2::interface::e_renderresourcekey_va_handle;
    assert( ::e2::render::renderpackage_gl::pack( buf, pkg, key, (void**)p_data, 1 ) );
    (*pkg)->_resource_subtype = object_type;
    (*pkg)->set_render_cmd_type( ::e2::interface::e_rendercmd_type_bind );
    (*pkg)->set_render_resource_type( ::e2::interface::e_renderresource_type_object );
    return true;
}
bool renderbackend_gl::enable_attrib( ::e2::memory::buffer * buf, ::e2::interface::i_renderpackage ** pkg, uint64_t attrib_type, uint64_t * attrib_handle ){
    size_t offset;
    void ** p_data;
    if( false == buf->buffer_get_next_available( &offset, &p_data, 1 ) ){
	assert( false && "mem buffer for loading vertex shader failed." );
    }
    p_data[0]= attrib_handle;
    uint64_t * key;
    if( false == buf->buffer_get_next_available( &offset, &key, 1 ) ){
	assert( false && "mem buffer for loading vertex shader failed." );
    }
    key[0] = ::e2::interface::e_renderresourcekey_va_index;
    assert( ::e2::render::renderpackage_gl::pack( buf, pkg, key, (void**)p_data, 1 ) );
    (*pkg)->_resource_subtype = attrib_type;
    (*pkg)->set_render_cmd_type( ::e2::interface::e_rendercmd_type_enable );
    (*pkg)->set_render_resource_type( ::e2::interface::e_renderresource_type_attrib );
    return true;
}

bool renderbackend_gl::store_defineformat_vertexattrib( ::e2::memory::buffer * buf, ::e2::interface::i_renderpackage ** pkg, uint64_t * attrib_handle, uint64_t * attrib_size, uint64_t * attrib_data_type, bool * attrib_normalized, uint64_t * attrib_stride, void * attrib_pointer ){
    size_t offset;
    void ** p_data;
    if( false == buf->buffer_get_next_available( &offset, &p_data, 6 ) ){
	assert( false && "mem buffer for loading vertex shader failed." );
    }
    p_data[0]= attrib_handle;
    p_data[1]= attrib_size;
    p_data[2] = attrib_data_type;
    p_data[3]= attrib_normalized;
    p_data[4]= attrib_stride;
    p_data[5]= attrib_pointer;
    uint64_t * key;
    if( false == buf->buffer_get_next_available( &offset, &key, 6 ) ){
	assert( false && "mem buffer for loading vertex shader failed." );
    }
    key[0] = ::e2::interface::e_renderresourcekey_index;
    key[1] = ::e2::interface::e_renderresourcekey_size;
    key[2] = ::e2::interface::e_renderresourcekey_type;
    key[3] = ::e2::interface::e_renderresourcekey_normalized;
    key[4] = ::e2::interface::e_renderresourcekey_stride;
    key[5] = ::e2::interface::e_renderresourcekey_pointer;
    assert( ::e2::render::renderpackage_gl::pack( buf, pkg, key, (void**)p_data, 6 ) );
    (*pkg)->_resource_subtype = ::e2::interface::e_renderresource_subtype_vertex_attrib_data;
    (*pkg)->set_render_cmd_type( ::e2::interface::e_rendercmd_type_store );
    (*pkg)->set_render_resource_type( ::e2::interface::e_renderresource_type_defineformat );
    return true;
}

bool renderbackend_gl::exec_drawbatch( ::e2::memory::buffer * buf, ::e2::interface::i_renderpackage ** pkg, uint64_t * primitive_type , int64_t * data_offset, uint64_t * data_count ){
    size_t offset;
    void ** p_data;
    if( false == buf->buffer_get_next_available( &offset, &p_data, 3 ) ){
	assert( false && "mem buffer for loading vertex shader failed." );
    }
    p_data[0]= primitive_type;
    p_data[1]= data_offset;
    p_data[2]= data_count;
    uint64_t * key;
    if( false == buf->buffer_get_next_available( &offset, &key, 3 ) ){
	assert( false && "mem buffer for loading vertex shader failed." );
    }
    key[0] = ::e2::interface::e_renderresourcekey_primitive_type;
    key[1] = ::e2::interface::e_renderresourcekey_primitive_offset;
    key[2] = ::e2::interface::e_renderresourcekey_primitive_count;
    assert( ::e2::render::renderpackage_gl::pack( buf, pkg, key, (void**)p_data, 3 ) );
    (*pkg)->set_render_cmd_type( ::e2::interface::e_rendercmd_type_exec );
    (*pkg)->set_render_resource_type( ::e2::interface::e_renderresource_type_drawbatch );
    return true;
}

} }
