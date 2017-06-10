#ifndef E2_I_RENDERBACKEND_HPP
#define E2_I_RENDERBACKEND_HPP

#include <utility>
#include <functional>

namespace e2 { namespace interface {

enum class e_renderbackend_action {
    e_renderbackend_action_submit,
};

template< class Impl >
class i_renderbackend : public virtual Impl {
public:
         using type_renderbackend_impl = Impl;

               template< class... Args >
               i_renderbackend( Args && ... args ) : Impl( std::forward< Args >( std::move( args ) ) ... ) {}
               template< class Arg >
	       bool renderbackend_process( e_renderbackend_action a, Arg arg ){ return Impl::renderbackend_process( a, arg ); }
};

} }

#endif
