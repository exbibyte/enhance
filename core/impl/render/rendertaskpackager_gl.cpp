#include <cassert>

#include "renderpackage_gl.hpp"
#include "rendertaskpackager_gl.hpp"
#include "buffer.hpp"

namespace e2 { namespace render {

bool rendertaskpackager_gl::process( ::e2::memory::buffer * buf, ::e2::interface::i_renderpackage ** pkg, ::e2::interface::i_rendernode * n ){
    return n->package( buf, pkg, this );
}

bool rendertaskpackager_gl::process( ::e2::memory::buffer * buf, ::e2::interface::i_renderpackage ** pkg, ::e2::interface::i_rendernode_init_window * n ){
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
	*data++ = n->_x;
	*data++ = n->_y;
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
bool rendertaskpackager_gl::process( ::e2::memory::buffer * buf, ::e2::interface::i_renderpackage ** pkg, ::e2::interface::i_rendernode_swap_window_buffer * n ){
    assert( ::e2::render::renderpackage_gl::pack( buf, pkg, nullptr, nullptr, 0 ) );
    (*pkg)->set_render_cmd_type( ::e2::interface::e_rendercmd_type_exec );
    (*pkg)->set_render_resource_type( ::e2::interface::e_renderresource_type_windowing );
    (*pkg)->_resource_subtype = ::e2::interface::e_renderresource_subtype_window_buf_swap;
    return true;
}
bool rendertaskpackager_gl::process( ::e2::memory::buffer * buf, ::e2::interface::i_renderpackage ** pkg, ::e2::interface::i_rendernode_clear_window_buffer_colour * n ){
    assert( ::e2::render::renderpackage_gl::pack( buf, pkg, nullptr, nullptr, 0 ) );
    (*pkg)->set_render_cmd_type( ::e2::interface::e_rendercmd_type_exec );
    (*pkg)->set_render_resource_type( ::e2::interface::e_renderresource_type_windowing );
    (*pkg)->_resource_subtype = ::e2::interface::e_renderresource_subtype_window_clear_colour;
    return true;
}
bool rendertaskpackager_gl::process( ::e2::memory::buffer * buf, ::e2::interface::i_renderpackage ** pkg, ::e2::interface::i_rendernode_clear_window_buffer_depth * n ){
    assert( ::e2::render::renderpackage_gl::pack( buf, pkg, nullptr, nullptr, 0 ) );
    (*pkg)->set_render_cmd_type( ::e2::interface::e_rendercmd_type_exec );
    (*pkg)->set_render_resource_type( ::e2::interface::e_renderresource_type_windowing );
    (*pkg)->_resource_subtype = ::e2::interface::e_renderresource_subtype_window_clear_depth;
    return true;
}
bool rendertaskpackager_gl::process( ::e2::memory::buffer * buf, ::e2::interface::i_renderpackage ** pkg, ::e2::interface::i_rendernode_disable_window_buffer_depth * n ){
    assert( ::e2::render::renderpackage_gl::pack( buf, pkg, nullptr, nullptr, 0 ) );
    (*pkg)->set_render_cmd_type( ::e2::interface::e_rendercmd_type_exec );
    (*pkg)->set_render_resource_type( ::e2::interface::e_renderresource_type_windowing );
    (*pkg)->_resource_subtype = ::e2::interface::e_renderresource_subtype_window_buffer_disable_depth;
    return true;
}
bool rendertaskpackager_gl::process( ::e2::memory::buffer * buf, ::e2::interface::i_renderpackage ** pkg, ::e2::interface::i_rendernode_enable_window_buffer_depth * n ){
    assert( ::e2::render::renderpackage_gl::pack( buf, pkg, nullptr, nullptr, 0 ) );
    (*pkg)->set_render_cmd_type( ::e2::interface::e_rendercmd_type_exec );
    (*pkg)->set_render_resource_type( ::e2::interface::e_renderresource_type_windowing );
    (*pkg)->_resource_subtype = ::e2::interface::e_renderresource_subtype_window_buffer_enable_depth;
    return true;
}
bool rendertaskpackager_gl::process( ::e2::memory::buffer * buf, ::e2::interface::i_renderpackage ** pkg, ::e2::interface::i_rendernode_deinit_window * n ){
    assert( ::e2::render::renderpackage_gl::pack( buf, pkg, nullptr, nullptr, 0 ) );
    (*pkg)->set_render_cmd_type( ::e2::interface::e_rendercmd_type_deinit );
    (*pkg)->set_render_resource_type( ::e2::interface::e_renderresource_type_windowing );
    (*pkg)->_resource_subtype = ::e2::interface::e_renderresource_subtype_na;
    return true;
}
bool rendertaskpackager_gl::process( ::e2::memory::buffer * buf, ::e2::interface::i_renderpackage ** pkg, ::e2::interface::i_rendernode_init_program * n ){
    size_t offset;
    uint64_t * key;
    if( false == buf->buffer_get_next_available( &offset, &key, 1 ) ){
	assert( false && "mem buffer for init window failed." );
    }
    key[0] = ::e2::interface::e_renderresourcekey_program_handle;
    assert( ::e2::render::renderpackage_gl::pack( buf, pkg, key, (void**)& n->_program_handle, 1 ) );
    (*pkg)->set_render_cmd_type( ::e2::interface::e_rendercmd_type_init );
    (*pkg)->set_render_resource_type( ::e2::interface::e_renderresource_type_program );
    return true;
}
bool rendertaskpackager_gl::process( ::e2::memory::buffer * buf, ::e2::interface::i_renderpackage ** pkg, ::e2::interface::i_rendernode_deinit_program * n ){
    size_t offset;
    uint64_t * key;
    if( false == buf->buffer_get_next_available( &offset, &key, 1 ) ){
	assert( false && "mem buffer for init window failed." );
    }
    uint64_t ** p_h;
    if( false == buf->buffer_get_next_available( &offset, &p_h, 1 ) ){
	assert( false && "mem buffer failed." );
    }
    p_h[0] = n->_program_handle;
    key[0] = ::e2::interface::e_renderresourcekey_program_handle;
    assert( ::e2::render::renderpackage_gl::pack( buf, pkg, key, (void**)p_h, 1 ) );
    (*pkg)->set_render_cmd_type( ::e2::interface::e_rendercmd_type_deinit );
    (*pkg)->set_render_resource_type( ::e2::interface::e_renderresource_type_program );
    return true;
}
bool rendertaskpackager_gl::process( ::e2::memory::buffer * buf, ::e2::interface::i_renderpackage ** pkg, ::e2::interface::i_rendernode_load_shader * n ){
    size_t offset;
    uint64_t ** p_shader_handle;
    if( false == buf->buffer_get_next_available( &offset, &p_shader_handle, 1 ) ){
	assert( false && "mem buffer for loading vertex shader failed." );
    }
    p_shader_handle[0] = n->_shader_handle;
    uint64_t ** p_shader_type;
    if( false == buf->buffer_get_next_available( &offset, &p_shader_type, 1 ) ){
	assert( false && "mem buffer for loading vertex shader failed." );
    }
    p_shader_type[0] = n->_shader_type;
    char ** p_shader_source;
    if( false == buf->buffer_get_next_available( &offset, &p_shader_source, 1 ) ){
	assert( false && "mem buffer for loading vertex shader failed." );
    }
    p_shader_source[0] = n->_source;
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
    (*pkg)->_resource_subtype = n->_source_type;
    return true;
}
bool rendertaskpackager_gl::process( ::e2::memory::buffer * buf, ::e2::interface::i_renderpackage ** pkg, ::e2::interface::i_rendernode_bind_shader * n ){
    size_t offset;
    uint64_t ** p_data;
    if( false == buf->buffer_get_next_available( &offset, &p_data, 2 ) ){
	assert( false && "mem buffer for loading vertex shader failed." );
    }
    p_data[0]= n->_program_handle;
    p_data[1] = n->_shader_handle;
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
bool rendertaskpackager_gl::process( ::e2::memory::buffer * buf, ::e2::interface::i_renderpackage ** pkg, ::e2::interface::i_rendernode_bind_program * n ){
    size_t offset;
    void ** p_data;
    if( false == buf->buffer_get_next_available( &offset, &p_data, 1 ) ){
	assert( false && "mem buffer for loading vertex shader failed." );
    }
    p_data[0]= n->_program_handle;
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
bool rendertaskpackager_gl::process( ::e2::memory::buffer * buf, ::e2::interface::i_renderpackage ** pkg, ::e2::interface::i_rendernode_query_attrib * n ){
    size_t offset;
    uint64_t ** p_data;
    if( false == buf->buffer_get_next_available( &offset, &p_data, 1 ) ){
	assert( false && "mem buffer for loading vertex shader failed." );
    }
    p_data[0]= n->_program_handle;
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
bool rendertaskpackager_gl::process( ::e2::memory::buffer * buf, ::e2::interface::i_renderpackage ** pkg, ::e2::interface::i_rendernode_query_persistent * n){
    size_t offset;
    uint64_t ** p_data;
    if( false == buf->buffer_get_next_available( &offset, &p_data, 1 ) ){
	assert( false && "mem buffer for loading vertex shader failed." );
    }
    p_data[0]= n->_program_handle;
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
bool rendertaskpackager_gl::process( ::e2::memory::buffer * buf, ::e2::interface::i_renderpackage ** pkg, ::e2::interface::i_rendernode_bind_attrib * n ){
    size_t offset;
    void ** p_data;
    if( false == buf->buffer_get_next_available( &offset, &p_data, 3 ) ){
	assert( false && "mem buffer for loading vertex shader failed." );
    }
    p_data[0]= (void*) n->_program_handle;
    p_data[1]= (void*) n->_index;
    p_data[2]= (void*) n->_var_name;
    uint64_t * key;
    if( false == buf->buffer_get_next_available( &offset, &key, 3 ) ){
	assert( false && "mem buffer for loading vertex shader failed." );
    }
    key[0] = ::e2::interface::e_renderresourcekey_program_handle;
    key[1] = ::e2::interface::e_renderresourcekey_attrib_index;
    key[2] = ::e2::interface::e_renderresourcekey_attrib_name;
    assert( ::e2::render::renderpackage_gl::pack( buf, pkg, key, (void**)p_data, 3 ) );
    (*pkg)->_resource_subtype = n->_location_type; //expect e_renderresource_subtype_attrib_frag_location / e_renderresource_subtype_attrib_attrib_location
    (*pkg)->set_render_cmd_type( ::e2::interface::e_rendercmd_type_bind );
    (*pkg)->set_render_resource_type( ::e2::interface::e_renderresource_type_attrib );
    return true;    
}
bool rendertaskpackager_gl::process( ::e2::memory::buffer * buf, ::e2::interface::i_renderpackage ** pkg, ::e2::interface::i_rendernode_compute_program * n ){
    size_t offset;
    uint64_t ** p_data;
    if( false == buf->buffer_get_next_available( &offset, &p_data, 1 ) ){
	assert( false && "mem buffer for loading vertex shader failed." );
    }
    p_data[0]= n->_program_handle;
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
bool rendertaskpackager_gl::process( ::e2::memory::buffer * buf, ::e2::interface::i_renderpackage ** pkg, ::e2::interface::i_rendernode_init_buffer * n ){
    size_t offset;
    void ** p_data;
    if( false == buf->buffer_get_next_available( &offset, &p_data, 2 ) ){
	assert( false && "mem buffer for loading vertex shader failed." );
    }
    p_data[0]= n->_num_buffers;
    p_data[1]= n->_buffers;
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
bool rendertaskpackager_gl::process( ::e2::memory::buffer * buf, ::e2::interface::i_renderpackage ** pkg, ::e2::interface::i_rendernode_deinit_buffer * n ){
    size_t offset;
    void ** p_data;
    if( false == buf->buffer_get_next_available( &offset, &p_data, 2 ) ){
	assert( false && "mem buffer for loading vertex shader failed." );
    }
    p_data[0]= n->_num_buffers;
    p_data[1]= n->_buffers;
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
bool rendertaskpackager_gl::process( ::e2::memory::buffer * buf, ::e2::interface::i_renderpackage ** pkg, ::e2::interface::i_rendernode_bind_buffer * n ){
    size_t offset;
    void ** p_data;
    if( false == buf->buffer_get_next_available( &offset, &p_data, 2 ) ){
	assert( false && "mem buffer for loading vertex shader failed." );
    }
    p_data[0]= n->_buffer_type;
    p_data[1]= n->_buffer_handle;
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
bool rendertaskpackager_gl::process( ::e2::memory::buffer * buf, ::e2::interface::i_renderpackage ** pkg, ::e2::interface::i_rendernode_store_buffer * n ){
    size_t offset;
    void ** p_data;
    if( false == buf->buffer_get_next_available( &offset, &p_data, 4 ) ){
	assert( false && "mem buffer for loading vertex shader failed." );
    }
    p_data[0]= n->_buffer_type;
    p_data[1]= n->_buffer_size;
    p_data[2]= n->_buffer_data;
    p_data[3]= n->_buffer_usage;
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
bool rendertaskpackager_gl::process( ::e2::memory::buffer * buf, ::e2::interface::i_renderpackage ** pkg, ::e2::interface::i_rendernode_init_object * n ){
    size_t offset;
    void ** p_data;
    if( false == buf->buffer_get_next_available( &offset, &p_data, 2 ) ){
	assert( false && "mem buffer for loading vertex shader failed." );
    }
    p_data[0]= n->_num_objects;
    p_data[1]= n->_handle_objects;
    uint64_t * key;
    if( false == buf->buffer_get_next_available( &offset, &key, 2 ) ){
	assert( false && "mem buffer for loading vertex shader failed." );
    }
    key[0] = ::e2::interface::e_renderresourcekey_va_num;
    key[1] = ::e2::interface::e_renderresourcekey_va_handle;
    assert( ::e2::render::renderpackage_gl::pack( buf, pkg, key, (void**)p_data, 2 ) );

    (*pkg)->_resource_subtype = n->_object_type;
    (*pkg)->set_render_cmd_type( ::e2::interface::e_rendercmd_type_init );
    (*pkg)->set_render_resource_type( ::e2::interface::e_renderresource_type_object );
    return true;    
}
bool rendertaskpackager_gl::process( ::e2::memory::buffer * buf, ::e2::interface::i_renderpackage ** pkg, ::e2::interface::i_rendernode_bind_object * n ){
    size_t offset;
    void ** p_data;
    if( false == buf->buffer_get_next_available( &offset, &p_data, 1 ) ){
	assert( false && "mem buffer for loading vertex shader failed." );
    }
    p_data[0]= n->_object_handle;
    uint64_t * key;
    if( false == buf->buffer_get_next_available( &offset, &key, 1 ) ){
	assert( false && "mem buffer for loading vertex shader failed." );
    }
    key[0] = ::e2::interface::e_renderresourcekey_va_handle;
    assert( ::e2::render::renderpackage_gl::pack( buf, pkg, key, (void**)p_data, 1 ) );
    (*pkg)->_resource_subtype = n->_object_type;
    (*pkg)->set_render_cmd_type( ::e2::interface::e_rendercmd_type_bind );
    (*pkg)->set_render_resource_type( ::e2::interface::e_renderresource_type_object );
    return true;
}
bool rendertaskpackager_gl::process( ::e2::memory::buffer * buf, ::e2::interface::i_renderpackage ** pkg, ::e2::interface::i_rendernode_enable_attrib * n ){
    size_t offset;
    void ** p_data;
    if( false == buf->buffer_get_next_available( &offset, &p_data, 1 ) ){
	assert( false && "mem buffer for loading vertex shader failed." );
    }
    p_data[0]= n->_attrib_handle;
    uint64_t * key;
    if( false == buf->buffer_get_next_available( &offset, &key, 1 ) ){
	assert( false && "mem buffer for loading vertex shader failed." );
    }
    key[0] = ::e2::interface::e_renderresourcekey_va_index;
    assert( ::e2::render::renderpackage_gl::pack( buf, pkg, key, (void**)p_data, 1 ) );
    (*pkg)->_resource_subtype = n->_attrib_type;
    (*pkg)->set_render_cmd_type( ::e2::interface::e_rendercmd_type_enable );
    (*pkg)->set_render_resource_type( ::e2::interface::e_renderresource_type_attrib );
    return true;
}

bool rendertaskpackager_gl::process( ::e2::memory::buffer * buf, ::e2::interface::i_renderpackage ** pkg, ::e2::interface::i_rendernode_store_defineformat_vertexattrib * n ){
    size_t offset;
    void ** p_data;
    if( false == buf->buffer_get_next_available( &offset, &p_data, 6 ) ){
	assert( false && "mem buffer for loading vertex shader failed." );
    }
    p_data[0]= n->_attrib_handle;
    p_data[1]= n->_attrib_size;
    p_data[2] = n->_attrib_data_type;
    p_data[3]= n->_attrib_normalized;
    p_data[4]= n->_attrib_stride;
    p_data[5]= n->_attrib_pointer;
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

bool rendertaskpackager_gl::process( ::e2::memory::buffer * buf, ::e2::interface::i_renderpackage ** pkg, ::e2::interface::i_rendernode_exec_drawbatch * n ){
    size_t offset;
    void ** p_data;
    if( false == buf->buffer_get_next_available( &offset, &p_data, 3 ) ){
	assert( false && "mem buffer for loading vertex shader failed." );
    }

    p_data[0]= n->_primitive_type;
    p_data[1]= n->_offset;
    p_data[2]= n->_count;
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
