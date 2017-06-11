#ifndef E2_I_RENDERBACKEND_HPP
#define E2_I_RENDERBACKEND_HPP

#include <utility>
#include <functional>
#include <list>

#include "i_rendernode.hpp"

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
	       bool renderbackend_process_rendernodes( std::list< ::e2::interface::i_rendernode * > * nodes  ){ return Impl::renderbackend_process_rendernodes( nodes ); }
               //swaps committed rangerpackages with a fresh one
               bool renderbackend_process_commit(){ return Impl::renderbackend_process_commit(); }
               //executes committed renderpackages
               bool renderbackend_process_renderpackages( int count ){ return Impl::renderbackend_process_renderpackages( count ); }
               bool renderbackend_get_window( void ** win ){ return Impl::renderbackend_get_window( win ); }
};

} }

#endif
