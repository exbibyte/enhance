#include <cstdint>
#include <cstring>
#include <list>
#include <utility>

#include "i_rendernode.hpp"
#include "i_renderdevice.hpp"

namespace e2 { namespace interface {

void i_rendernode::set_render_cmd_type( size_t val ){
    _render_key &= ~( (uint64_t) 0xFFFF << 48 );
    _render_key |= ( (uint64_t) 0xFFFF & val ) << 48;
}
void i_rendernode::set_render_resource_type( size_t val ){
    _render_key &= ~( (uint64_t) 0xFFFF << 32 );
    _render_key |= ( (uint64_t) 0xFFFF & val ) << 32;
}
void i_rendernode::set_render_resource_subtype( size_t val ){
    _render_key &= ~( (uint64_t) 0xFFFF << 16 );
    _render_key |= ( (uint64_t) 0xFFFF & val ) << 16;
}
void i_rendernode::set_render_channel_type( size_t val ){
    _render_key &= ~( (uint64_t) 0xFFFF );
    _render_key |= ( (uint64_t) 0xFFFF & val );
}
size_t i_rendernode::extract_render_cmd_type(){
    return ( _render_key & ( (uint64_t) 0xFFFF << 48 ) ) >> 48;
}
size_t i_rendernode::extract_render_resource_type(){
    return ( _render_key & ( (uint64_t) 0xFFFF << 32 ) ) >> 32;
}
size_t i_rendernode::extract_render_resource_subtype(){
    return ( _render_key & ( (uint64_t) 0xFFFF << 16 ) ) >> 16;
}
size_t i_rendernode::extract_render_channel_type(){
    return _render_key & (uint64_t) 0xFFFF;
}

//init starts
	
i_rendernode_init_window::i_rendernode_init_window( uint64_t x, uint64_t y ){
    set_render_cmd_type( e_rendercmd_type_init );
    set_render_resource_type( e_renderresource_type_windowing );
    set_render_resource_subtype( e_renderresource_subtype_na );
    _x = x;
    _y = y;
}
bool i_rendernode_init_window::process_renderdevice( i_renderdevice * rd ) {
    return rd->renderdevice_process_impl( this );
}

i_rendernode_init_program::i_rendernode_init_program( uint64_t * handle ){
    set_render_cmd_type( e_rendercmd_type_init );
    set_render_resource_type( e_renderresource_type_program );
    set_render_resource_subtype( e_renderresource_subtype_program_handle );
    _program_handle = handle;
}
bool i_rendernode_init_program::process_renderdevice( i_renderdevice * rd ) {
    return rd->renderdevice_process_impl( this );
}

i_rendernode_init_buffer::i_rendernode_init_buffer( uint64_t * num_buf, uint64_t * buffers ){
    set_render_cmd_type( e_rendercmd_type_init );
    set_render_resource_type( e_renderresource_type_buffer );
    set_render_resource_subtype( e_renderresource_subtype_buffer_handle );
    _num_buffers = num_buf;
    _buffers = buffers;
}
bool i_rendernode_init_buffer::process_renderdevice( i_renderdevice * rd ) {
    return rd->renderdevice_process_impl( this );
}

i_rendernode_init_object_vertex_array::i_rendernode_init_object_vertex_array( uint64_t * num_obj, uint64_t * obj_handles ){
    set_render_cmd_type( e_rendercmd_type_init );
    set_render_resource_type( e_renderresource_type_object );
    set_render_resource_subtype( e_renderresource_subtype_object_vertex_array );
    _num_objects = num_obj;
    _handle_objects = obj_handles;
}
bool i_rendernode_init_object_vertex_array::process_renderdevice( i_renderdevice * rd ) {
    return rd->renderdevice_process_impl( this );
}

//init ends
//deinit starts

i_rendernode_deinit_window::i_rendernode_deinit_window(){
    set_render_cmd_type( e_rendercmd_type_deinit );
    set_render_resource_type( e_renderresource_type_windowing );
    set_render_resource_subtype( e_renderresource_subtype_window_handle );
}
bool i_rendernode_deinit_window::process_renderdevice( i_renderdevice * rd ) {
    return rd->renderdevice_process_impl( this );
}

i_rendernode_deinit_program::i_rendernode_deinit_program( uint64_t * program_handle ){
    set_render_cmd_type( e_rendercmd_type_deinit );
    set_render_resource_type( e_renderresource_type_program );
    set_render_resource_subtype( e_renderresource_subtype_program_handle );
    _program_handle = program_handle;
}
bool i_rendernode_deinit_program::process_renderdevice( i_renderdevice * rd ) {
    return rd->renderdevice_process_impl( this );
}

i_rendernode_deinit_buffer::i_rendernode_deinit_buffer( uint64_t * num_buf, uint64_t * buffers ){
    set_render_cmd_type( e_rendercmd_type_deinit );
    set_render_resource_type( e_renderresource_type_buffer );
    set_render_resource_subtype( e_renderresource_subtype_buffer_handle );
    _num_buffers = num_buf;
    _buffers = buffers;
}
bool i_rendernode_deinit_buffer::process_renderdevice( i_renderdevice * rd ) {
    return rd->renderdevice_process_impl( this );
}

i_rendernode_deinit_object_vertex_array::i_rendernode_deinit_object_vertex_array( uint64_t * num_obj, uint64_t * obj_handles ){
    set_render_cmd_type( e_rendercmd_type_deinit );
    set_render_resource_type( e_renderresource_type_object );
    set_render_resource_subtype( e_renderresource_subtype_object_vertex_array );
    _num_objects = num_obj;
    _objects = obj_handles;
}
bool i_rendernode_deinit_object_vertex_array::process_renderdevice( i_renderdevice * rd ) {
    return rd->renderdevice_process_impl( this );
}

//deinit ends
//query starts

i_rendernode_query_attrib::i_rendernode_query_attrib( uint64_t * program_handle ){
    set_render_cmd_type( e_rendercmd_type_query );
    set_render_resource_type( e_renderresource_type_attrib );
    set_render_resource_subtype( e_renderresource_subtype_na );
    _program_handle = program_handle;
}
bool i_rendernode_query_attrib::process_renderdevice( i_renderdevice * rd ) {
    return rd->renderdevice_process_impl( this );
}

i_rendernode_query_persistent::i_rendernode_query_persistent( uint64_t * program_handle ){
    set_render_cmd_type( e_rendercmd_type_query );
    set_render_resource_type( e_renderresource_type_persistent );
    set_render_resource_subtype( e_renderresource_subtype_na );
    _program_handle = program_handle;
}
bool i_rendernode_query_persistent::process_renderdevice( i_renderdevice * rd ) {
    return rd->renderdevice_process_impl( this );
}

//query ends
//load starts
	
i_rendernode_load_shader::i_rendernode_load_shader( uint64_t * shader_handle, uint64_t * shader_type, char * source, uint64_t source_type ){
    _shader_handle = shader_handle;
    _shader_type = shader_type;
    _source = source;
    _source_type = source_type;
    set_render_cmd_type( e_rendercmd_type_load );
    set_render_resource_type( e_renderresource_type_shader );
    set_render_resource_subtype( e_renderresource_subtype_na );
}
bool i_rendernode_load_shader::process_renderdevice( i_renderdevice * rd ) {
    return rd->renderdevice_process_impl( this );
}

//load ends
//store starts

i_rendernode_store_buffer::i_rendernode_store_buffer( uint64_t * buf_type, uint64_t * buf_size, void * buf_data, uint64_t * buf_usage ){
    _buffer_type = buf_type;
    _buffer_size = buf_size;
    _buffer_data = buf_data;
    _buffer_usage = buf_usage;
    set_render_cmd_type( e_rendercmd_type_store );
    set_render_resource_type( e_renderresource_type_buffer );
    set_render_resource_subtype( e_renderresource_subtype_buffer_data );
}
bool i_rendernode_store_buffer::process_renderdevice( i_renderdevice * rd ) {
    return rd->renderdevice_process_impl( this );
}

i_rendernode_store_defineformat_vertexattrib::i_rendernode_store_defineformat_vertexattrib( uint64_t * handle, uint64_t * size, uint64_t * data_type, bool * normalized, uint64_t * stride, void * pointer ){
    _attrib_handle = handle;
    _attrib_size = size;
    _attrib_data_type = data_type;
    _attrib_normalized = normalized;
    _attrib_stride = stride;
    _attrib_pointer = pointer;
    set_render_cmd_type( e_rendercmd_type_store );
    set_render_resource_type( e_renderresource_type_defineformat );
    set_render_resource_subtype( e_renderresource_subtype_defineformat_vertexattrib );
}
bool i_rendernode_store_defineformat_vertexattrib::process_renderdevice( i_renderdevice * rd ) {
    return rd->renderdevice_process_impl( this );
}

i_rendernode_store_program_param::i_rendernode_store_program_param( uint64_t * program_handle, uint64_t * param, uint64_t * param_val ){
    _program_handle = program_handle;
    _parameter = param;
    _parameter_value = param_val;
    set_render_cmd_type( e_rendercmd_type_store );
    set_render_resource_type( e_renderresource_type_program );
    set_render_resource_subtype( e_renderresource_subtype_program_parameter );
}
bool i_rendernode_store_program_param::process_renderdevice( i_renderdevice * rd ) {
    return rd->renderdevice_process_impl( this );
}

i_rendernode_store_persistent_vecf::i_rendernode_store_persistent_vecf( uint64_t * program_handle, uint64_t * dim, char * name, float * vec ){
    _program_handle = program_handle;
    _dim = dim;
    _name = name;
    _vec = vec;
    set_render_cmd_type( e_rendercmd_type_store );
    set_render_resource_type( e_renderresource_type_persistent );
    set_render_resource_subtype( e_renderresource_subtype_persistent_vec_f );
}
bool i_rendernode_store_persistent_vecf::process_renderdevice( i_renderdevice * rd ) {
    return rd->renderdevice_process_impl( this );
}

i_rendernode_store_persistent_matf::i_rendernode_store_persistent_matf( uint64_t * program_handle, uint64_t * dim, char * name, float * mat ){
    _program_handle = program_handle;
    _dim = dim;
    _name = name;
    _mat = mat;
    set_render_cmd_type( e_rendercmd_type_store );
    set_render_resource_type( e_renderresource_type_persistent );
    set_render_resource_subtype( e_renderresource_subtype_persistent_mat_f );
}
bool i_rendernode_store_persistent_matf::process_renderdevice( i_renderdevice * rd ) {
    return rd->renderdevice_process_impl( this );
}

//store ends
//bind starts

i_rendernode_bind_shader::i_rendernode_bind_shader( uint64_t * program_handle, uint64_t * shader_handle ){
    _program_handle = program_handle;
    _shader_handle = shader_handle;
    set_render_cmd_type( e_rendercmd_type_bind );
    set_render_resource_type( e_renderresource_type_shader );
    set_render_resource_subtype( e_renderresource_subtype_shader_handle );
}
bool i_rendernode_bind_shader::process_renderdevice( i_renderdevice * rd ) {
    return rd->renderdevice_process_impl( this );
}

i_rendernode_bind_program::i_rendernode_bind_program( uint64_t * program_handle ){
    _program_handle = program_handle;
    set_render_cmd_type( e_rendercmd_type_bind );
    set_render_resource_type( e_renderresource_type_program );
    set_render_resource_subtype( e_renderresource_subtype_program_handle );	
}
bool i_rendernode_bind_program::process_renderdevice( i_renderdevice * rd ) {
    return rd->renderdevice_process_impl( this );
}

i_rendernode_bind_attrib_attrib_loc::i_rendernode_bind_attrib_attrib_loc( uint64_t * program_handle, uint64_t * index, char * var_name ){
    _program_handle = program_handle;
    _index = index;
    _var_name = var_name;
    set_render_cmd_type( e_rendercmd_type_bind );
    set_render_resource_type( e_renderresource_type_attrib );
    set_render_resource_subtype( e_renderresource_subtype_attrib_attrib_location );
}
bool i_rendernode_bind_attrib_attrib_loc::process_renderdevice( i_renderdevice * rd ) {
    return rd->renderdevice_process_impl( this );
}

i_rendernode_bind_attrib_frag_loc::i_rendernode_bind_attrib_frag_loc( uint64_t * program_handle, uint64_t * index, char * var_name ){
    _program_handle = program_handle;
    _index = index;
    _var_name = var_name;
    set_render_cmd_type( e_rendercmd_type_bind );
    set_render_resource_type( e_renderresource_type_attrib );
    set_render_resource_subtype( e_renderresource_subtype_attrib_frag_location );
}
bool i_rendernode_bind_attrib_frag_loc::process_renderdevice( i_renderdevice * rd ) {
    return rd->renderdevice_process_impl( this );
}

i_rendernode_bind_buffer::i_rendernode_bind_buffer( uint64_t * buf_type, uint64_t * buf_handle ){
    _buffer_type = buf_type;
    _buffer_handle = buf_handle;
    set_render_cmd_type( e_rendercmd_type_bind );
    set_render_resource_type( e_renderresource_type_buffer );
    set_render_resource_subtype( e_renderresource_subtype_buffer_handle );	
    }
bool i_rendernode_bind_buffer::process_renderdevice( i_renderdevice * rd ) {
    return rd->renderdevice_process_impl( this );
}

i_rendernode_bind_object_vertex_array::i_rendernode_bind_object_vertex_array( uint64_t * obj_handle ){
    _object_handle = obj_handle;
    set_render_cmd_type( e_rendercmd_type_bind );
    set_render_resource_type( e_renderresource_type_object );
    set_render_resource_subtype( e_renderresource_subtype_object_vertex_array );
}
bool i_rendernode_bind_object_vertex_array::process_renderdevice( i_renderdevice * rd ) {
    return rd->renderdevice_process_impl( this );
}

//bind ends

//unbind starts

i_rendernode_unbind_shader::i_rendernode_unbind_shader( uint64_t * program_handle, uint64_t * shader_handle ){
    _program_handle = program_handle;
    _shader_handle = shader_handle;
    set_render_cmd_type( e_rendercmd_type_unbind );
    set_render_resource_type( e_renderresource_type_shader );
    set_render_resource_subtype( e_renderresource_subtype_shader_handle );
}
bool i_rendernode_unbind_shader::process_renderdevice( i_renderdevice * rd ) {
    return rd->renderdevice_process_impl( this );
}

//unbind ends

//compute starts
i_rendernode_compute_program::i_rendernode_compute_program( uint64_t * program_handle ){
    _program_handle = program_handle;
    set_render_cmd_type( e_rendercmd_type_compute );
    set_render_resource_type( e_renderresource_type_program );
    set_render_resource_subtype( e_renderresource_subtype_program_compile );
}
bool i_rendernode_compute_program::process_renderdevice( i_renderdevice * rd ) {
    return rd->renderdevice_process_impl( this );
}
//compute ends
//enable starts

i_rendernode_enable_attrib_vertex_array::i_rendernode_enable_attrib_vertex_array( uint64_t * attrib_handle ){
    _attrib_handle = attrib_handle;
    set_render_cmd_type( e_rendercmd_type_enable );
    set_render_resource_type( e_renderresource_type_attrib );
    set_render_resource_subtype( e_renderresource_subtype_attrib_object_vertex_array );
}
bool i_rendernode_enable_attrib_vertex_array::process_renderdevice( i_renderdevice * rd ) {
    return rd->renderdevice_process_impl( this );
}

i_rendernode_enable_window_buffer_depth::i_rendernode_enable_window_buffer_depth(){
    set_render_cmd_type( e_rendercmd_type_enable );
    set_render_resource_type( e_renderresource_type_windowing );
    set_render_resource_subtype( e_renderresource_subtype_window_buffer_depth );
}
bool i_rendernode_enable_window_buffer_depth::process_renderdevice( i_renderdevice * rd ) {
    return rd->renderdevice_process_impl( this );
}

//enable ends

//disable starts

i_rendernode_disable_attrib_vertex_array::i_rendernode_disable_attrib_vertex_array( uint64_t * attrib_handle ){
    _attrib_handle = attrib_handle;
    set_render_cmd_type( e_rendercmd_type_disable );
    set_render_resource_type( e_renderresource_type_attrib );
    set_render_resource_subtype( e_renderresource_subtype_attrib_object_vertex_array );
}
bool i_rendernode_disable_attrib_vertex_array::process_renderdevice( i_renderdevice * rd ) {
    return rd->renderdevice_process_impl( this );
}

i_rendernode_disable_window_buffer_depth::i_rendernode_disable_window_buffer_depth(){
    set_render_cmd_type( e_rendercmd_type_disable );
    set_render_resource_type( e_renderresource_type_windowing );
    set_render_resource_subtype( e_renderresource_subtype_window_buffer_depth );
}
bool i_rendernode_disable_window_buffer_depth::process_renderdevice( i_renderdevice * rd ) {
    return rd->renderdevice_process_impl( this );
}

//disable ends

//exec starts

i_rendernode_exec_drawbatch::i_rendernode_exec_drawbatch( uint64_t * prim_type, int64_t * offset, uint64_t * count ){
    _primitive_type = prim_type;
    _offset = offset;
    _count = count;
    set_render_cmd_type( e_rendercmd_type_exec );
    set_render_resource_type( e_renderresource_type_drawbatch );
    set_render_resource_subtype( e_renderresource_subtype_na );
}
bool i_rendernode_exec_drawbatch::process_renderdevice( i_renderdevice * rd ) {
    return rd->renderdevice_process_impl( this );
}

i_rendernode_swap_window_buffer::i_rendernode_swap_window_buffer(){
    set_render_cmd_type( e_rendercmd_type_exec );
    set_render_resource_type( e_renderresource_type_windowing );
    set_render_resource_subtype( e_renderresource_subtype_window_buf_swap );
}
bool i_rendernode_swap_window_buffer::process_renderdevice( i_renderdevice * rd ) {
    return rd->renderdevice_process_impl( this );
}

i_rendernode_clear_window_buffer_colour::i_rendernode_clear_window_buffer_colour(){
    set_render_cmd_type( e_rendercmd_type_exec );
    set_render_resource_type( e_renderresource_type_windowing );
    set_render_resource_subtype( e_renderresource_subtype_window_clear_colour );
}
bool i_rendernode_clear_window_buffer_colour::process_renderdevice( i_renderdevice * rd ) {
    return rd->renderdevice_process_impl( this );
}

i_rendernode_clear_window_buffer_depth::i_rendernode_clear_window_buffer_depth(){
    set_render_cmd_type( e_rendercmd_type_exec );
    set_render_resource_type( e_renderresource_type_windowing );
    set_render_resource_subtype( e_renderresource_subtype_window_clear_depth );
}
bool i_rendernode_clear_window_buffer_depth::process_renderdevice( i_renderdevice * rd ) {
    return rd->renderdevice_process_impl( this );
}

//exec ends

} }
