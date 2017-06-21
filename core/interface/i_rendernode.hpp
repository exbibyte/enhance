#ifndef E2_I_RENDERNODE_HPP
#define E2_I_RENDERNODE_HPP

#include <cstdint>
#include <cstring>
#include <list>
#include <utility>



namespace e2 { namespace interface {

enum e_rendercmd_type : uint64_t {
    e_rendercmd_type_query = 1,
    e_rendercmd_type_compute,
    e_rendercmd_type_store,
    e_rendercmd_type_load,
    e_rendercmd_type_init,
    e_rendercmd_type_deinit,
    e_rendercmd_type_exec,
    e_rendercmd_type_enable,
    e_rendercmd_type_disable,
    e_rendercmd_type_bind,
    e_rendercmd_type_unbind,
};
	
enum e_renderresource_type : uint64_t { //list of primitive resources
    e_renderresource_type_na = 1,
    e_renderresource_type_windowing,
    e_renderresource_type_defineformat,
    e_renderresource_type_drawbatch,
    e_renderresource_type_attrib,
    e_renderresource_type_buffer,
    e_renderresource_type_object,
    e_renderresource_type_program,
    e_renderresource_type_shader,
    e_renderresource_type_persistent,
//todo:
    e_renderresource_type_camera,
    e_renderresource_type_transform_rotate,
    e_renderresource_type_transform_translate,
};

enum e_renderresource_subtype : uint64_t { //list of primitive resources
    e_renderresource_subtype_na = 1,
    e_renderresource_subtype_window_buf_swap,
    e_renderresource_subtype_window_clear_colour,
    e_renderresource_subtype_window_clear_depth,
    e_renderresource_subtype_window_buffer_depth,
    e_renderresource_subtype_vertex_attrib_data,
    e_renderresource_subtype_vertex_attrib_array,
    e_renderresource_subtype_attrib_object_vertex_array,
    e_renderresource_subtype_attrib_attrib_location,
    e_renderresource_subtype_attrib_frag_location,
    e_renderresource_subtype_persistent_vec_f,
    e_renderresource_subtype_persistent_vec_u,
    e_renderresource_subtype_persistent_vec_i,	
    e_renderresource_subtype_persistent_mat_f,
    e_renderresource_subtype_persistent_mat_u,
    e_renderresource_subtype_persistent_mat_i,
    e_renderresource_subtype_shader_file,
    e_renderresource_subtype_shader_string,
    e_renderresource_subtype_program_parameter,
    e_renderresource_subtype_program_handle,
    e_renderresource_subtype_buffer_handle,
    e_renderresource_subtype_window_handle,
    e_renderresource_subtype_object_handle,
    e_renderresource_subtype_shader_handle,
    e_renderresource_subtype_buffer_data,
    e_renderresource_subtype_defineformat_vertexattrib,
    e_renderresource_subtype_program_compile,
    e_renderresource_subtype_object_vertex_array,
};

class i_renderdevice;

class i_rendernode {
public:
    uint64_t _handle; //unique id for graph building
    std::list< i_rendernode * > _inputs;
    std::list< i_rendernode * > _outputs;
    virtual bool process_renderdevice( i_renderdevice * rd ) = 0;
    
    uint64_t _render_key; // 4 x 16 bit fields (cmd_type, resource_type, resource_subtype, channel_type )
    void set_render_cmd_type( size_t val );
    void set_render_resource_type( size_t val );
    void set_render_resource_subtype( size_t val );
    void set_render_channel_type( size_t val );
    size_t extract_render_cmd_type();
    size_t extract_render_resource_type();
    size_t extract_render_resource_subtype();
    size_t extract_render_channel_type();
};

//init starts

class i_rendernode_init_window : public i_rendernode {
public:
    int _x;
    int _y;
    i_rendernode_init_window( uint64_t x, uint64_t y );
    bool process_renderdevice( i_renderdevice * rd ) override;
};

class i_rendernode_init_program : public i_rendernode {
public:
    uint64_t * _program_handle;
    i_rendernode_init_program( uint64_t * handle );
    bool process_renderdevice( i_renderdevice * rd ) override;
};

class i_rendernode_init_buffer : public i_rendernode {
public:
    uint64_t * _num_buffers;
    uint64_t * _buffers;
    i_rendernode_init_buffer( uint64_t * num_buf, uint64_t * buffers );
    bool process_renderdevice( i_renderdevice * rd ) override;
};

class i_rendernode_init_object_vertex_array : public i_rendernode {
public:
    // uint64_t _object_type;
    uint64_t * _num_objects;
    uint64_t * _handle_objects;
    i_rendernode_init_object_vertex_array( uint64_t * num_obj, uint64_t * obj_handles );
    bool process_renderdevice( i_renderdevice * rd ) override;
};

//init ends
//deinit starts

class i_rendernode_deinit_window : public i_rendernode {
public:
    i_rendernode_deinit_window();
    bool process_renderdevice( i_renderdevice * rd ) override;
};


class i_rendernode_deinit_program : public i_rendernode {
public:
    uint64_t * _program_handle;
    i_rendernode_deinit_program( uint64_t * program_handle );
    bool process_renderdevice( i_renderdevice * rd ) override;
};

class i_rendernode_deinit_buffer : public i_rendernode {
public:
    uint64_t * _num_buffers;
    uint64_t * _buffers;
    i_rendernode_deinit_buffer( uint64_t * num_buf, uint64_t * buffers );
    bool process_renderdevice( i_renderdevice * rd ) override;
};

class i_rendernode_deinit_object_vertex_array : public i_rendernode {
public:
    uint64_t * _num_objects;
    uint64_t * _objects;
    i_rendernode_deinit_object_vertex_array( uint64_t * num_obj, uint64_t * obj_handles );
    bool process_renderdevice( i_renderdevice * rd ) override;
};

//deinit ends
//query starts

class i_rendernode_query_attrib : public i_rendernode {
public:
    uint64_t * _program_handle;
    i_rendernode_query_attrib( uint64_t * program_handle );
    bool process_renderdevice( i_renderdevice * rd ) override;
};

class i_rendernode_query_persistent : public i_rendernode {
public:
    uint64_t * _program_handle;
    i_rendernode_query_persistent( uint64_t * program_handle );
    bool process_renderdevice( i_renderdevice * rd ) override;
};
//query ends
//load starts

class i_rendernode_load_shader : public i_rendernode {
public:
    uint64_t * _shader_handle;
    uint64_t * _shader_type;
        char * _source;
      uint64_t _source_type;
    i_rendernode_load_shader( uint64_t * shader_handle, uint64_t * shader_type, char * source, uint64_t source_type );
    bool process_renderdevice( i_renderdevice * rd ) override;
};
//load ends
//store starts

class i_rendernode_store_buffer : public i_rendernode {
public:
    uint64_t * _buffer_type;
    uint64_t * _buffer_size;
        void * _buffer_data;
    uint64_t * _buffer_usage;
    i_rendernode_store_buffer( uint64_t * buf_type, uint64_t * buf_size, void * buf_data, uint64_t * buf_usage );
    bool process_renderdevice( i_renderdevice * rd ) override;
};


class i_rendernode_store_defineformat_vertexattrib : public i_rendernode {
public:
    uint64_t * _attrib_handle;
    uint64_t * _attrib_size;
    uint64_t * _attrib_data_type;
        bool * _attrib_normalized;
    uint64_t * _attrib_stride;
        void * _attrib_pointer;
    i_rendernode_store_defineformat_vertexattrib( uint64_t * handle, uint64_t * size, uint64_t * data_type, bool * normalized, uint64_t * stride, void * pointer );
    bool process_renderdevice( i_renderdevice * rd ) override;
};

class i_rendernode_store_program_param : public i_rendernode {
public:
    uint64_t * _program_handle;
    uint64_t * _parameter;
    uint64_t * _parameter_value;
    i_rendernode_store_program_param( uint64_t * program_handle, uint64_t * param, uint64_t * param_val );
    bool process_renderdevice( i_renderdevice * rd ) override;
};


class i_rendernode_store_persistent_vecf : public i_rendernode {
public:
    uint64_t * _program_handle;
    uint64_t * _dim;
    char * _name;
    float * _vec;
    i_rendernode_store_persistent_vecf( uint64_t * program_handle, uint64_t * dim, char * name, float * vec );
    bool process_renderdevice( i_renderdevice * rd ) override;
};

class i_rendernode_store_persistent_matf : public i_rendernode {
public:
    uint64_t * _program_handle;
    uint64_t * _dim;
    char * _name;
    float * _mat;
    i_rendernode_store_persistent_matf( uint64_t * program_handle, uint64_t * dim, char * name, float * mat );
    bool process_renderdevice( i_renderdevice * rd ) override;
};

//store ends
//bind starts

class i_rendernode_bind_shader : public i_rendernode {
public:
    uint64_t * _program_handle;
    uint64_t * _shader_handle;
    i_rendernode_bind_shader( uint64_t * program_handle, uint64_t * shader_handle );
    bool process_renderdevice( i_renderdevice * rd ) override;
};

class i_rendernode_bind_program : public i_rendernode {
public:
    uint64_t * _program_handle;
    i_rendernode_bind_program( uint64_t * program_handle );
    bool process_renderdevice( i_renderdevice * rd ) override;
};

class i_rendernode_bind_attrib_attrib_loc : public i_rendernode {
public:
    uint64_t * _program_handle;
    uint64_t * _index;
    char * _var_name;
    i_rendernode_bind_attrib_attrib_loc( uint64_t * program_handle, uint64_t * index, char * var_name );
    bool process_renderdevice( i_renderdevice * rd ) override;
};

class i_rendernode_bind_attrib_frag_loc : public i_rendernode {
public:
    uint64_t * _program_handle;
    uint64_t * _index;
    char * _var_name;
    i_rendernode_bind_attrib_frag_loc( uint64_t * program_handle, uint64_t * index, char * var_name );
    bool process_renderdevice( i_renderdevice * rd ) override;
};

class i_rendernode_bind_buffer : public i_rendernode {
public:
    uint64_t * _buffer_type;
    uint64_t * _buffer_handle;
    i_rendernode_bind_buffer( uint64_t * buf_type, uint64_t * buf_handle );
    bool process_renderdevice( i_renderdevice * rd ) override;
};

class i_rendernode_bind_object_vertex_array : public i_rendernode {
public:
    uint64_t * _object_handle;
    i_rendernode_bind_object_vertex_array( uint64_t * obj_handle );
    bool process_renderdevice( i_renderdevice * rd ) override;
};
//bind ends

//unbind starts

class i_rendernode_unbind_shader : public i_rendernode {
public:
    uint64_t * _program_handle;
    uint64_t * _shader_handle;
    i_rendernode_unbind_shader( uint64_t * program_handle, uint64_t * shader_handle );
    bool process_renderdevice( i_renderdevice * rd ) override;
};
//unbind ends

//compute starts

class i_rendernode_compute_program : public i_rendernode {
public:
    uint64_t * _program_handle;
    i_rendernode_compute_program( uint64_t * program_handle );
    bool process_renderdevice( i_renderdevice * rd ) override;
};
//compute ends
//enable starts

class i_rendernode_enable_attrib_vertex_array : public i_rendernode {
public:
    uint64_t * _attrib_handle;
    i_rendernode_enable_attrib_vertex_array( uint64_t * attrib_handle );
    bool process_renderdevice( i_renderdevice * rd ) override;
};

class i_rendernode_enable_window_buffer_depth : public i_rendernode {
public:
    i_rendernode_enable_window_buffer_depth();
    bool process_renderdevice( i_renderdevice * rd ) override;
};

//enable ends

//disable starts

class i_rendernode_disable_attrib_vertex_array : public i_rendernode {
public:
    uint64_t * _attrib_handle;
    i_rendernode_disable_attrib_vertex_array( uint64_t * attrib_handle );
    bool process_renderdevice( i_renderdevice * rd ) override;
};

class i_rendernode_disable_window_buffer_depth : public i_rendernode {
public:
    i_rendernode_disable_window_buffer_depth();
    bool process_renderdevice( i_renderdevice * rd ) override;
};

//disable ends

//exec starts

class i_rendernode_exec_drawbatch : public i_rendernode {
public:
    uint64_t * _primitive_type;
    int64_t * _offset;
    uint64_t * _count;
    i_rendernode_exec_drawbatch( uint64_t * prim_type, int64_t * offset, uint64_t * count );
    bool process_renderdevice( i_renderdevice * rd ) override;
};

class i_rendernode_swap_window_buffer : public i_rendernode {
public:
    i_rendernode_swap_window_buffer();
    bool process_renderdevice( i_renderdevice * rd ) override;
};

class i_rendernode_clear_window_buffer_colour : public i_rendernode {
public:
    i_rendernode_clear_window_buffer_colour();
    bool process_renderdevice( i_renderdevice * rd ) override;
};

class i_rendernode_clear_window_buffer_depth : public i_rendernode {
public:
    i_rendernode_clear_window_buffer_depth();
    bool process_renderdevice( i_renderdevice * rd ) override;
};

//exec ends

} }
	
#endif
