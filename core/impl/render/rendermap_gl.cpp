#include <cstdint>
#include <unordered_map>

#include "i_renderresource.hpp"
#include "gl_includes.hpp"
#include "rendermap_gl.hpp"

namespace e2 { namespace render {

std::unordered_map< uint64_t, GLenum > rendermap_gl::_map_render_primitives = {
    { ::e2::interface::e_renderresource_primitive_points, GL_POINTS },
    { ::e2::interface::e_renderresource_primitive_line_strip, GL_LINE_STRIP },
    { ::e2::interface::e_renderresource_primitive_line_loop, GL_LINE_LOOP },
    { ::e2::interface::e_renderresource_primitive_lines, GL_LINES },
    { ::e2::interface::e_renderresource_primitive_line_strip_adjacency, GL_LINE_STRIP_ADJACENCY },
    { ::e2::interface::e_renderresource_primitive_lines_adjacency, GL_LINES_ADJACENCY },
    { ::e2::interface::e_renderresource_primitive_triangle_strip, GL_TRIANGLE_STRIP },
    { ::e2::interface::e_renderresource_primitive_triangle_fan, GL_TRIANGLE_FAN },
    { ::e2::interface::e_renderresource_primitive_triangles, GL_TRIANGLES },
    { ::e2::interface::e_renderresource_primitive_triangle_strip_adjacency, GL_TRIANGLE_STRIP_ADJACENCY },
    { ::e2::interface::e_renderresource_primitive_triangles_adjacency, GL_TRIANGLES_ADJACENCY },
    { ::e2::interface::e_renderresource_primitive_patches, GL_PATCHES },
};
std::unordered_map< uint64_t, GLenum > rendermap_gl::_map_render_bind_buffer = {
    { ::e2::interface::e_renderresource_buffer_array_buffer, GL_ARRAY_BUFFER },
    { ::e2::interface::e_renderresource_buffer_atomic_counter_buffer, GL_ATOMIC_COUNTER_BUFFER },
    { ::e2::interface::e_renderresource_buffer_copy_read_buffer, GL_COPY_READ_BUFFER },
    { ::e2::interface::e_renderresource_buffer_copy_write_buffer, GL_COPY_WRITE_BUFFER },
    { ::e2::interface::e_renderresource_buffer_dispatch_indirect_buffer, GL_DISPATCH_INDIRECT_BUFFER },
    { ::e2::interface::e_renderresource_buffer_draw_indirect_buffer, GL_DRAW_INDIRECT_BUFFER },
    { ::e2::interface::e_renderresource_buffer_element_array_buffer, GL_ELEMENT_ARRAY_BUFFER },
    { ::e2::interface::e_renderresource_buffer_pixel_pack_buffer, GL_PIXEL_PACK_BUFFER },
    { ::e2::interface::e_renderresource_buffer_pixel_unpack_buffer, GL_PIXEL_UNPACK_BUFFER },
    { ::e2::interface::e_renderresource_buffer_query_buffer, GL_QUERY_BUFFER },
    { ::e2::interface::e_renderresource_buffer_shader_storage_buffer, GL_SHADER_STORAGE_BUFFER },
    { ::e2::interface::e_renderresource_buffer_texture_buffer, GL_TEXTURE_BUFFER },
    { ::e2::interface::e_renderresource_buffer_transform_feedback_buffer, GL_TRANSFORM_FEEDBACK_BUFFER },
    { ::e2::interface::e_renderresource_buffer_uniform_buffer, GL_UNIFORM_BUFFER },
};

std::unordered_map< uint64_t, GLenum > rendermap_gl::_map_render_data_type = {
    { ::e2::interface::e_renderresource_data_type_float, GL_FLOAT },
};

} }
