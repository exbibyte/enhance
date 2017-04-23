#ifndef HASHTABLE_UNIVERSAL_CHAIN_HPP
#define HASHTABLE_UNIVERSAL_CHAIN_HPP

#include <vector>
#include <iterator>
#include <functional>
#include <type_traits>
#include <random>
#include <cmath>
#include <iostream>
#include <list>

#include "i_hashtable.hpp"
#include "hash_universal.hpp"

namespace e2 { namespace ds {

template< class K, class V >
class hashtable_universal_chain_impl {
public:
                  hashtable_universal_chain_impl(){}
                  hashtable_universal_chain_impl( size_t table_size );
                  ~hashtable_universal_chain_impl();

             bool insert( K const, V const & );
             bool find( K const , V & );
             bool erase( K const );
             bool add_hash_func( std::function< K ( K ) > ); //add hash function into the univeral set
             bool get_hash_func_current( std::function< K ( K ) > & ); //add hash function into the univeral set
             bool resize( size_t size );
           size_t get_table_size();
           double get_load_factor();
private:
                  class hashnode {
		  public:		      
		      K          _key;
		      hashnode * _next = nullptr;
		      hashnode * _prev = nullptr;
		      V          _val;
		  };

            using vec_hash_func =    std::vector< std::function< K ( K )> >;
            using vec_hashnode =     std::vector< hashnode * >;
            using hash_func =        std::function< K ( K ) >;

             bool compute_hash( K const key, K & hashed_val );
             bool select_random_hash_func();
             bool set_default_hash_funcs( size_t const table_size );
             bool prepend_hashnode( hashnode * & node, K const key, V const & val );
             bool remove_hashnode( hashnode * & node );
       hashnode * find_hashnode( hashnode * const node, K const key );

     vec_hashnode _table;
    vec_hash_func _funcs_hash;
        hash_func _func_hash_selected;
           size_t _count_items;
};

#include "hashtable_universal_chain.tpp"

template< class K, class V >
class hashtable_universal_chain final : public ::e2::interface::i_hashtable < K, V, hashtable_universal_chain_impl > {
public:
    hashtable_universal_chain( size_t table_size ) : hashtable_universal_chain_impl < K, V > ( table_size ), ::e2::interface::i_hashtable < K, V, hashtable_universal_chain_impl >( table_size ) {
	using type_parent = ::e2::interface::i_hashtable < K, V, hashtable_universal_chain_impl >;

	type_parent::_trait_hashtable._hash_method = ::e2::trait::hashtable::e_hash_method::universal;
	type_parent::_trait_hashtable._table_method = ::e2::trait::hashtable::e_table_method::closed;
	type_parent::_trait_hashtable._lock_load_factor = ::e2::trait::hashtable::e_lock_load_factor::constant;
	type_parent::_trait_concurrency._bound_size = ::e2::trait::concurrency::e_bound_size::unbounded;
	type_parent::_trait_concurrency._method = ::e2::trait::concurrency::e_method::none;
	type_parent::_trait_concurrency._granularity = ::e2::trait::concurrency::e_granularity::none;
	type_parent::_trait_concurrency._fairness = ::e2::trait::concurrency::e_fairness::not_applicable;
    }
};

} }

#endif
