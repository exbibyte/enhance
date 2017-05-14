#ifndef E2_RENDERDEVICE_DISPATCH_HPP
#define E2_RENDERDEVICE_DISPATCH_HPP

#include <map>
#include <cstring>

#include "i_rendercmd.hpp"
#include "i_renderpackage.hpp"

namespace e2 { namespace render {

//helper to dispatch on tuple of triple
class renderdispatch_key {
public:
    renderdispatch_key(){}
    renderdispatch_key( size_t c, size_t r, size_t p ) : _cmd_type( c ), _resource_type ( r ), _payload_type( p ) {}
    size_t _cmd_type;
    size_t _resource_type;
    size_t _payload_type;
};
struct comp_dispatch {
    bool operator()( renderdispatch_key const & a, renderdispatch_key const & b ){
	if( a._cmd_type < b._cmd_type ||
	    a._resource_type < b._resource_type ||
	    a._payload_type < b._payload_type ){
	    return true;
	}
	return false;
    }
};
	
template< class Impl >
class renderdevice_dispatch {
public:
    bool dispatch_process( Impl * context, ::e2::interface::i_renderpackage pkg ){
	bool ret;
	renderdispatch_key k ( pkg.extract_render_cmd_type(), pkg.extract_render_resource_type(), pkg.extract_render_payload_type() );

	auto it = _dispatch_map.find( k );
	if( _dispatch_map.end() == it )
	    return false;
	bool (*f)( Impl *, ::e2::interface::i_renderpackage ) = it->second;
	ret = (*f)( context, pkg );
	return ret;
    }
    std::map< renderdispatch_key, bool(*)( Impl *, ::e2::interface::i_renderpackage ), comp_dispatch > _dispatch_map;
};

} }

#endif
