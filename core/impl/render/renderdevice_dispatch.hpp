#ifndef E2_RENDERDEVICE_DISPATCH_HPP
#define E2_RENDERDEVICE_DISPATCH_HPP

#include <map>
#include <unordered_map>
#include <cstring>
#include <functional>

#include "i_rendercmd.hpp"
#include "i_renderpackage.hpp"

namespace e2 { namespace render {

//helper to dispatch on tuple of triple
class renderdispatch_key {
public:
    renderdispatch_key(){}
    renderdispatch_key( size_t c, size_t r ) : _cmd_type( c ), _resource_type ( r ) {}
    size_t _cmd_type;
    size_t _resource_type;
};
struct hash_dispatch {
    size_t operator()( renderdispatch_key const & a ) const {
	std::hash<uint64_t> hasher;
	return ( ( hasher( a._cmd_type ) ^ ( hasher( a._resource_type ) << 1 ) ) >> 1 );
    }
};

struct comp_dispatch {
    bool operator()( renderdispatch_key const & a, renderdispatch_key const & b ) const {
	return a._cmd_type == b._cmd_type
	    && a._resource_type == b._resource_type;
    }
};
	
template< class Impl >
class renderdevice_dispatch {
public:
    bool dispatch_process( Impl * context, ::e2::interface::i_renderpackage pkg ){
	bool ret;
	renderdispatch_key k ( pkg.extract_render_cmd_type(), pkg.extract_render_resource_type() );

	auto it = _dispatch_map.find( k );
	if( _dispatch_map.end() == it )
	    return false;
	bool (*f)( Impl *, ::e2::interface::i_renderpackage ) = it->second;
	ret = (*f)( context, pkg );
	return ret;
    }
    std::unordered_map< renderdispatch_key, bool(*)( Impl *, ::e2::interface::i_renderpackage ), hash_dispatch, comp_dispatch > _dispatch_map;
};

} }

#endif
