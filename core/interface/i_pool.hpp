#ifndef I_POOL_H
#define I_POOL_H

#include <utility>
#include <functional>

namespace e2 { namespace interface {
	
template< class T, template< class > class ContainerType >
class i_pool : public virtual ContainerType<T> {
public:
        //container and value traits
        using container_type =     ContainerType<T>;
	using value_type =         T;

              template< class... Args >
              i_pool( Args && ... args ) : ContainerType<T>( std::forward<Args>(args)... ) {}

         bool clear(){ return ContainerType<T>::clear(); }
         bool empty(){ return ContainerType<T>::empty(); }
       size_t size(){ return ContainerType<T>::size(); }
         bool put( value_type const * item ){ return ContainerType<T>::put( item ); }
         bool get( value_type * item ){ return ContainerType<T>::get( item ); }
};

} }

#endif
