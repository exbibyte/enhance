#ifndef E2_I_RENDERRESOURCE_HPP
#define E2_I_RENDERRESOURCE_HPP

#include <cstdint>
#include <cstring>

namespace e2 { namespace interface {

enum e_renderresource_type : uint64_t { //list of primitive resources that can be manipulated/executed
    windowing = 1,
    camera,
    attributes,
    buffer,
    binding,
    draw_batch,
    transform_rotate,
    transform_translate,
};

enum e_renderresource_window : uint64_t {
    window_dim = 1,
    window_buf_swap,
};

enum e_renderresource_camera : uint64_t {
    camera_coord = 1,
    camera_dir,
    camera_up,
    camera_fov,
};

enum e_renderresource_primitives : uint64_t {
    renderresource_primitive_points = 1,
    renderresource_primitive_line_strip,
    renderresource_primitive_line_loop,
    renderresource_primitive_lines,
    renderresource_primitive_line_strip_adjacency,
    renderresource_primitive_lines_adjacency,
    renderresource_primitive_triangle_strip,
    renderresource_primitive_triangle_fan,
    renderresource_primitive_triangles,
    renderresource_primitive_triangle_strip_adjacency,
    renderresource_primitive_triangles_adjacency,
    renderresource_primitive_patches,
};

enum e_renderresource_buffer : uint64_t {
    renderresource_buffer_array_buffer = 1,
    renderresource_buffer_atomic_counter_buffer,
    renderresource_buffer_copy_read_buffer,
    renderresource_buffer_copy_write_buffer,
    renderresource_buffer_dispatch_indirect_buffer,
    renderresource_buffer_draw_indirect_buffer,
    renderresource_buffer_element_array_buffer,
    renderresource_buffer_pixel_pack_buffer,
    renderresource_buffer_pixel_unpack_buffer,
    renderresource_buffer_query_buffer,
    renderresource_buffer_shader_storage_buffer,
    renderresource_buffer_texture_buffer,
    renderresource_buffer_transform_feedback_buffer,
    renderresource_buffer_uniform_buffer,
};

class i_renderresource {
public:
    size_t _id;
};

} }

#endif
