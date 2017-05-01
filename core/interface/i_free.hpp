#ifndef E2_I_FREE_HPP
#define E2_I_FREE_HPP

namespace e2 { namespace trait { namespace memory {

enum class e_free_method {
    first_fit,
    next_fit,
    best_fit,
};

} } }

namespace e2 { namespace interface {

template< class Impl >
class i_free : public virtual Impl {
public:
    using type_impl_free = Impl;

    template< class ... Args >
    i_free( Args && ... args ) : Impl( std::forward< Args >( args ) ... ) {}
    ~i_free(){}

    //reclaim a block
    template< class... Args >
    bool freeing( Args && ... args ){ return Impl::freeing( std::forward< Args >( args )... ); }

    template< class... Args >
    bool deleting( Args ... args ){ return Impl::deleting( std::forward< Args >( args )... ); }
};

} }

#endif
