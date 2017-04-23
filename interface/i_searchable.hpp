#ifndef I_SEARCHABLE_HPP
#define I_SEARCHABLE_HPP

#include <utility>
#include <functional>

#include "i_concurrency.hpp"

namespace e2 { namespace interface {

template< class T, template< class > class ContainerType >
class i_searchable : public virtual ContainerType<T> {
public:
        //container and value traits
        using container_type =       ContainerType<T>;
	using value_type =           T;

        ::e2::trait::concurrency::trait_concurrency _trait_concurrency;

              template< class... Args >
              i_searchable( Args && ... args ) : ContainerType<T>( std::forward<Args>(args)... ) {}
              ~i_searchable(){}
    
         bool clear(){ return ContainerType<T>::clear(); }
         bool empty(){ return ContainerType<T>::empty(); }
       size_t size(){ return ContainerType<T>::size(); }
         bool add( T const * item, size_t key ){ return ContainerType<T>::add( item, key ); }
         bool remove( T * item, size_t key ){ return ContainerType<T>::remove( item, key ); }
         bool contains( T const * item, size_t key ){ return ContainerType<T>::contains( item, key ); }
};

} }

#endif
