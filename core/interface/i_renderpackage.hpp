#ifndef E2_I_RENDERPACKAGE_HPP
#define E2_I_RENDERPACKAGE_HPP

#include <utility>
#include <map>
#include <set>
#include <initializer_list>

#include "i_rendercmd.hpp"
#include "i_renderresource.hpp"
#include "i_renderpayload.hpp"

namespace e2 { namespace interface {

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
    e_renderresource_subtype_vertex_attrib_data,
    e_renderresource_subtype_vertex_attrib_array,
    e_renderresource_subtype_object_vertex_array,
    e_renderresource_subtype_attrib_location,
    e_renderresource_subtype_attrib_frag,
    e_renderresource_subtype_persistent_vec_f,
    e_renderresource_subtype_persistent_vec_u,
    e_renderresource_subtype_persistent_vec_i,	
    e_renderresource_subtype_persistent_mat_f,
    e_renderresource_subtype_persistent_mat_u,
    e_renderresource_subtype_persistent_mat_i,
    e_renderresource_subtype_shader_file,
    e_renderresource_subtype_shader_string,
    e_renderresource_subtype_program_parameter,
};

class i_package_filter {
public:
    i_package_filter( std::initializer_list< uint64_t > l ){
	for( auto it = l.begin(); it != l.end(); ++it ){
	    _lookup[ *it ] = std::pair<bool, void * >( false, nullptr );
	}
    }
    void * operator []( uint64_t k ){
	auto it = _lookup.find( k );
	if( _lookup.end() == it || false == it->second.first ){
	    assert( false && "package filter lookup failed." );
	    return nullptr;
	}
	return it->second.second;
    }
    std::map< uint64_t, std::pair<bool, void * > > _lookup;
};

class i_renderpackage {
public:
                                    uint64_t _render_key; // 4 x 16 bit fields (cmd_type, resource_type, payload_type, channel_type )
                                    uint64_t _resource_subtype;
                           i_renderpayload * _payload;

    void set_render_cmd_type( size_t val ){
	_render_key &= ~( (uint64_t) 0xFFFF << 48 );
        _render_key |= ( (uint64_t) 0xFFFF & val ) << 48;
    }
    void set_render_resource_type( size_t val ){
	_render_key &= ~( (uint64_t) 0xFFFF << 32 );
        _render_key |= ( (uint64_t) 0xFFFF & val ) << 32;
    }
    // void set_render_payload_type( size_t val ){
    // 	_render_key &= ~( (uint64_t) 0xFFFF << 16 );
    //     _render_key |= ( (uint64_t) 0xFFFF & val ) << 16;
    // }
    void set_render_channel_type( size_t val ){
	_render_key &= ~( (uint64_t) 0xFFFF );
        _render_key |= ( (uint64_t) 0xFFFF & val );
    }
    size_t extract_render_cmd_type(){
	return ( _render_key & ( (uint64_t) 0xFFFF << 48 ) ) >> 48;
    }
    size_t extract_render_resource_type(){
	return ( _render_key & ( (uint64_t) 0xFFFF << 32 ) ) >> 32;
    }
    // size_t extract_render_payload_type(){
    // 	return ( _render_key & ( (uint64_t) 0xFFFF << 16 ) ) >> 16;
    // }
    size_t extract_render_channel_type(){
	return _render_key & (uint64_t) 0xFFFF;
    }
};

} }

#endif
