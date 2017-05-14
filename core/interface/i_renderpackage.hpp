#ifndef E2_I_RENDERPACKAGE_HPP
#define E2_I_RENDERPACKAGE_HPP

#include <utility>

#include "i_rendercmd.hpp"
#include "i_renderresource.hpp"
#include "i_renderpayload.hpp"

namespace e2 { namespace interface {

class i_renderpackage {
public:
                                    uint64_t _render_key; // 4 x 16 bit fields (cmd_type, resource_type, payload_type, channel_type )
                                      size_t _num_resource;
                                      size_t _num_payload;
                           i_renderpayload * _payload;
                          i_renderresource * _resource;
                                      size_t _num_resource_payload_map;
               std::pair< size_t, size_t > * _resource_payload_map;

    void set_render_cmd_type( size_t val ){
	_render_key &= ~( (uint64_t) 0xFFFF << 48 );
        _render_key |= ( (uint64_t) 0xFFFF & val ) << 48;
    }
    void set_render_resource_type( size_t val ){
	_render_key &= ~( (uint64_t) 0xFFFF << 32 );
        _render_key |= ( (uint64_t) 0xFFFF & val ) << 32;
    }
    void set_render_payload_type( size_t val ){
	_render_key &= ~( (uint64_t) 0xFFFF << 16 );
        _render_key |= ( (uint64_t) 0xFFFF & val ) << 16;
    }
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
    size_t extract_render_payload_type(){
	return ( _render_key & ( (uint64_t) 0xFFFF << 16 ) ) >> 16;
    }
    size_t extract_render_channel_type(){
	return _render_key & (uint64_t) 0xFFFF;
    }
};

} }

#endif
