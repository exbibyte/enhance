#ifndef E2_I_MEMORY_MANAGER_HPP
#define E2_I_MEMORY_MANAGER_HPP

namespace e2 { namespace trait { namespace memory {

enum class e_parition_arity {
    single,
    multiple,
};

enum class e_generation_arity {
    single,
    multiple,
};

} } }

namespace e2 { namespace interface {

template< class Impl  >
class i_memory_manager : public virtual Impl {
public:
               template< class ... Args >
               i_memory_manager( Args && ... args ) : Impl( std::forward( std::move( args ) )... ) {}

               template< class T, class... Args >
           T * newing( Args ... args ){ return Impl::template newing< T, Args ... >( std::forward< Args >( args )... ); }

               template< class... Args >
	  bool deleting( Args ... args ){ return Impl::deleting( std::forward< Args >( args )... ); }

    ::e2::trait::memory::e_generation_arity _generation_arity;
      ::e2::trait::memory::e_parition_arity _parition_arity;
};

} }

#endif
