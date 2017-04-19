#ifndef I_LOCK_H
#define I_LOCK_H

#include <utility>
#include <functional>

class trait_lock_method {
public:
    class total{};
    class partial{};
    class synchronous{};
};

enum class lock_access_type {
    READ,
    WRITE
};

template< class Impl, class LockMethod >
class i_lock final : public Impl {
public:
        //lock traits
	using lock_method =        LockMethod;
	using lock_impl =          Impl;

              template< class... Args >
              i_lock( Args... args ) : Impl( std::forward<Args>(args)... ) {}
              ~i_lock(){}
	      i_lock( i_lock const & l ) : Impl( l ) {}
    
	 template< class... Args >
         bool lock( Args... args ){ return Impl::lock( std::forward<Args>(args)... ); }
	 template< class... Args >
	 bool unlock( Args... args ){ return Impl::unlock( std::forward<Args>(args)... ); }
	 template< class... Args >
	 bool is_free( Args... args ){ return Impl::is_free( std::forward<Args>(args)... ); }
	 template< class... Args >
	 size_t count( Args... args ){ return Impl::count( std::forward<Args>(args)... ); }
};

#endif
