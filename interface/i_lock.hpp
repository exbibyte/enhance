#ifndef I_LOCK_HPP
#define I_LOCK_HPP

#include <utility>
#include <functional>


namespace e2 { namespace trait { namespace lock {
	    
enum class e_lock_method {
    total,
    partial,
    synchronous,
};

struct trait_lock {
    e_lock_method _lock_method;
};

} } }

namespace e2 { namespace interface {

enum class lock_access_type {
    READ,
    WRITE
};

template< class Impl >
class i_lock : public virtual Impl {
public:
	using lock_impl =          Impl;

        ::e2::trait::lock::trait_lock _trait_lock;
    
              template< class... Args >
              i_lock( Args && ... args ) : Impl( std::forward<Args>( args )... ) {}
              ~i_lock(){}
	      i_lock( i_lock const & l ) : Impl( l ) {}
    
	 template< class... Args >
         bool lock( Args &&... args ){ return Impl::lock( std::forward<Args>( args )... ); }
	 template< class... Args >
	 bool unlock( Args &&... args ){ return Impl::unlock( std::forward<Args>( args )... ); }
	 template< class... Args >
	 bool is_free( Args && ... args ){ return Impl::is_free( std::forward<Args>( args )... ); }
	 template< class... Args >
	 size_t count( Args && ... args ){ return Impl::count( std::forward<Args>( args )... ); }
};

} }
	
#endif
