#ifndef E2_I_RENDERDEVICE_HPP
#define E2_I_RENDERDEVICE_HPP

#include <utility>
#include <functional>
#include <cstdint>

namespace e2 { namespace interface {
	
template< class Impl, class RenderPackage >
class i_renderdevice : public virtual Impl {
public:
         using type_renderdevice_impl = Impl;

               template< class... Args >
               i_renderdevice( Args && ... args ) : Impl( std::move( args ) ... ) {}
   inline bool renderdevice_process( RenderPackage p ){ return Impl::renderdevice_process( p ); }
};

} }

#endif
