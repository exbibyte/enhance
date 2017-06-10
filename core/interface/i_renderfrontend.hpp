#ifndef E2_I_RENDERFRONTEND_HPP
#define E2_I_RENDERFRONTEND_HPP

#include <utility>
#include <functional>

namespace e2 { namespace interface {

enum e_renderfrontend_action {
    e_renderfrontend_action_scatter = 1,
    e_renderfrontend_action_gather,
};

template< class Impl >
class i_renderfrontend : public virtual Impl {
public:
         using type_renderfrontend_impl = Impl;

               template< class... Args >
               i_renderfrontend( Args && ... args ) : Impl( std::forward< Args >( std::move( args ) ) ... ) {}
               template< class Arg >
          bool renderfrontend_process( ::e2::interface::e_renderfrontend_action a, Arg v ){ return Impl::renderfrontend_process( a, v ); }
};

} }

#endif
