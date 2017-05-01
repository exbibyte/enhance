#ifndef IALLOC_HPP
#define IALLOC_HPP

namespace e2 { namespace trait { namespace memory {

enum class e_alloc_method {
    first_fit,
    next_fit,
    best_fit,
};
	    
} } }

namespace e2 { namespace interface {

template< class Impl >
class i_alloc : public virtual Impl {
public:
               using type_impl_alloc = Impl;

               template< class ... Args >
               i_alloc( Args && ... args ) : Impl( std::forward< Args >( args ) ... ) {}
               ~i_alloc(){}

               //lend out a free block to user
               template< class T, class... Args >
          T ** newing( Args ... args ){ return Impl::template newing< T, Args ... >( std::forward< Args >( args )... ); }

               //internal helper
               template< class ... Args >
	  bool allocating( Args ... args ){ return Impl::allocating( std::forward< Args >( args )... ); }
};

} }

#endif
