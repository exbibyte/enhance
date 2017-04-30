#ifndef I_HASHTABLE_HPP
#define I_HASHTABLE_HPP

#include <vector>
#include <iterator>
#include <functional>
#include <type_traits>
#include <random>
#include <cmath>
#include <iostream>

#include "i_concurrency.hpp"

namespace e2 { namespace trait { namespace hashtable {

enum class e_hash_method {
    division,
    multiplication,
    universal,
    other,
};

enum class e_table_method {
    closed,
    open,
};

enum class e_lock_load_factor {
    constant,
    linear,
    not_applicable,
};

struct trait_hashtable {
    e_hash_method _hash_method;
    e_table_method _table_method;
    e_lock_load_factor _lock_load_factor;
};

} } }

namespace e2 { namespace interface {
	
template< class K, class V, template<class,class> class Impl >
class i_hashtable : public virtual Impl< K, V > {
public:
         using key_type =               K;
         using key_const_type =         K const;
         using value_type =             V;
         using val_reference =          V &;
         using val_const_reference =    V const &;
	 using hashtable_impl = Impl< K, V >;
	 
	 //hashtable properties
	 ::e2::trait::hashtable::trait_hashtable _trait_hashtable;
	 ::e2::trait::concurrency::trait_concurrency _trait_concurrency;

               template< class... Args >
               i_hashtable( Args ... args ) : Impl< K, V >( args... ) {}
               ~i_hashtable(){}

          bool insert( key_const_type k, val_const_reference v ){ return Impl< K, V >::insert( k, v ); }
          bool find( key_const_type k, val_reference v ){ return Impl< K, V >::find( k, v ); }
          bool erase( key_const_type k ){ return Impl< K, V >::erase( k ); }
          bool add_hash_func( std::function<size_t(size_t)> f ){ return Impl< K, V >::add_hash_func( f ); }
          bool get_hash_func_current( std::function<size_t(size_t)> & f ){ return Impl< K, V >::get_hash_func_current( f ); }
          bool resize( size_t size ){ return Impl< K, V >::resize( size ); }
        size_t get_table_size(){ return Impl< K, V >::get_table_size(); }
        double get_load_factor(){ return Impl< K, V >::get_load_factor(); }
        size_t size(){ return Impl< K, V >::size(); }
};

} }
    
#endif
