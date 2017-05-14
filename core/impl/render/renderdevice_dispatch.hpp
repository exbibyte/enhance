#ifndef E2_RENDERDEVICE_DISPATCH_HPP
#define E2_RENDERDEVICE_DISPATCH_HPP

#include <unordered_map>
#include <cstring>

#include "i_rendercmd.hpp"
#include "i_renderpackage.hpp"

namespace e2 { namespace render {

struct renderdispatch_key {
    renderdispatch_key( size_t c, size_t d, size_t r ) : _cmd( c ), _dest_type ( d ), _resource_type( r ) {}
    size_t _cmd;
    size_t _resource_type;
    size_t _dest_type;
};

class renderdevice_dispatch {
public:
    bool dispatch_process( i_renderpackage pkg ){
	bool ret;
	// renderdispatch_key k;
	// k._cmd = pkg._render_cmd.rendercmd_extract( e2::interface::e_rendercmd_extract::cmd_type );
	// k._resource_type = pkg._render_resource._resource_type;
	// k._dest_type = pkg._render_dest._dest_type;

	// auto it = _dispatch_map.find( k );
	// if( _dispatch_map.end() == it )
	//     return false;
	// bool (*f)( i_renderpackage ) = it->second;
	// bool ret = (*f)( pkg );
	return ret;
    }
    std::unordered_map< renderdispatch_key, bool(*)( i_renderpackage ) > _dispatch_map;
};

} }

#endif
