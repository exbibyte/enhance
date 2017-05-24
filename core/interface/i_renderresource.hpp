#ifndef E2_I_RENDERRESOURCE_HPP
#define E2_I_RENDERRESOURCE_HPP

#include <cstdint>
#include <cstring>

namespace e2 { namespace interface {

enum e_renderresourcekey : uint64_t {
    e_renderresourcekey_na = 1,
    e_renderresourcekey_wind_dimx,
    e_renderresourcekey_wind_dimy,
    e_renderresourcekey_primitive_type,
    e_renderresourcekey_primitive_offset,
    e_renderresourcekey_primitive_count,
    e_renderresourcekey_index,
    e_renderresourcekey_size,
    e_renderresourcekey_type,
    e_renderresourcekey_normalized,
    e_renderresourcekey_stride,
    e_renderresourcekey_pointer,
    e_renderresourcekey_buffer_handle,
    e_renderresourcekey_buffer_type,
    e_renderresourcekey_va_index,
    e_renderresourcekey_va_num,
    e_renderresourcekey_va_array,
    e_renderresourcekey_va_handle,
    e_renderresourcekey_buffer_size,
    e_renderresourcekey_buffer_data,
    e_renderresourcekey_buffer_usage,
    e_renderresourcekey_buffer_num,
    e_renderresourcekey_program_handle,
    e_renderresourcekey_attrib_index,
    e_renderresourcekey_attrib_name,
    e_renderresourcekey_frag_location,
    e_renderresourcekey_frag_name,
    e_renderresourcekey_shader_handle,
    e_renderresourcekey_dim,
    e_renderresourcekey_name,
    e_renderresourcekey_arrayf,
    e_renderresourcekey_shader_source,
    e_renderresourcekey_shader_type,
    e_renderresourcekey_program_param_type,
    e_renderresourcekey_program_param_val,
};

enum e_renderresource_primitives : uint64_t {
    e_renderresource_primitive_points = 1,
    e_renderresource_primitive_line_strip,
    e_renderresource_primitive_line_loop,
    e_renderresource_primitive_lines,
    e_renderresource_primitive_line_strip_adjacency,
    e_renderresource_primitive_lines_adjacency,
    e_renderresource_primitive_triangle_strip,
    e_renderresource_primitive_triangle_fan,
    e_renderresource_primitive_triangles,
    e_renderresource_primitive_triangle_strip_adjacency,
    e_renderresource_primitive_triangles_adjacency,
    e_renderresource_primitive_patches,
};
	
enum e_renderresource_buffer : uint64_t {
    e_renderresource_buffer_array_buffer = 1,
    e_renderresource_buffer_atomic_counter_buffer,
    e_renderresource_buffer_copy_read_buffer,
    e_renderresource_buffer_copy_write_buffer,
    e_renderresource_buffer_dispatch_indirect_buffer,
    e_renderresource_buffer_draw_indirect_buffer,
    e_renderresource_buffer_element_array_buffer,
    e_renderresource_buffer_pixel_pack_buffer,
    e_renderresource_buffer_pixel_unpack_buffer,
    e_renderresource_buffer_query_buffer,
    e_renderresource_buffer_shader_storage_buffer,
    e_renderresource_buffer_texture_buffer,
    e_renderresource_buffer_transform_feedback_buffer,
    e_renderresource_buffer_uniform_buffer,
};

enum e_renderresource_data_type : uint64_t {
    e_renderresource_data_type_float = 1,
};

} }

#endif
