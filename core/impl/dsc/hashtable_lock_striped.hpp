#ifndef I_HASHTABLE_LOCK_STRIPED_HPP
#define I_HASHTABLE_LOCK_STRIPED_HPP

#include <vector>
#include <iterator>
#include <functional>
#include <type_traits>
#include <random>
#include <cmath>
#include <iostream>
#include <mutex>
#include <thread>
#include <atomic>
#include <list>
#include <chrono>
#include <cassert>
#include <functional>

#include "i_hashtable.hpp"
#include "i_concurrency.hpp"
#include "lock_recursive_total.hpp"
#include "hash_universal.hpp"

namespace e2 { namespace dsc {

template< class K, class V >
class hashtable_lock_striped_impl {
public:

                  hashtable_lock_striped_impl();
                  hashtable_lock_striped_impl( size_t table_size, double lock_factor );
                  ~hashtable_lock_striped_impl();

             bool insert( K const, V const & );
             bool find( K const , V & );
             bool erase( K const );
             bool add_hash_func( std::function< K ( K ) > );
             bool get_hash_func_current( std::function< K ( K ) > & );
             bool resize( size_t size );
           size_t get_table_size();
           double get_load_factor();
           size_t size();
             bool gather_items( std::function< void( V ) > f );
private:
                  class hashnode {
		  public:		      
		      K          _key;
		      hashnode * _next = nullptr;
		      hashnode * _prev = nullptr;
		      V          _val;
		      hashnode() : _next( nullptr ), _prev( nullptr ) {}
		  };

            using vec_hash_func =    std::vector< std::function< K ( K )> >;
            using vec_hashnode =     std::vector< hashnode * >;
            using hash_func =        std::function< K ( K ) >;
            using vec_lock =         std::vector< ::e2::mt::lock_recursive_total >;

             bool compute_hash( K const key, K & hashed_val );
             bool select_random_hash_func();
             bool set_default_hash_funcs( size_t const table_size );

             bool quiesce();
       hashnode * prepend_hashnode( hashnode * node, K const key, V const & val );
             bool remove_hashnode( hashnode * & node );
       hashnode * find_hashnode( hashnode * const node, K const key );
public:
         vec_lock _locks;
     vec_hashnode _table;
    vec_hash_func _funcs_hash;
        hash_func _func_hash_selected;
std::atomic<size_t> _count_items;
           double _lock_factor;
std::atomic<size_t> _lock_count;
std::atomic<bool> _is_resizing;
std::atomic<bool> _is_destructing;
std::atomic<std::thread::id> _id_resize;
};

#include "hashtable_lock_striped.tpp"

template< class K, class V >
class hashtable_lock_striped final : public ::e2::interface::i_hashtable < K, V, hashtable_lock_striped_impl > {
public:    
    hashtable_lock_striped( size_t table_size, double lock_factor ) : hashtable_lock_striped_impl< K, V > ( table_size, lock_factor ) {
	using type_parent = ::e2::interface::i_hashtable < K, V, hashtable_lock_striped_impl >;

	type_parent::_trait_hashtable._hash_method = ::e2::trait::hashtable::e_hash_method::universal;
	type_parent::_trait_hashtable._table_method = ::e2::trait::hashtable::e_table_method::open;
	type_parent::_trait_hashtable._lock_load_factor = ::e2::trait::hashtable::e_lock_load_factor::constant;
	type_parent::_trait_concurrency._bound_size = ::e2::trait::concurrency::e_bound_size::unbounded;
	type_parent::_trait_concurrency._method = ::e2::trait::concurrency::e_method::total;
	type_parent::_trait_concurrency._granularity = ::e2::trait::concurrency::e_granularity::disjoint_access;
	type_parent::_trait_concurrency._fairness = ::e2::trait::concurrency::e_fairness::not_applicable;
    }


    hashtable_lock_striped() : hashtable_lock_striped_impl< K, V > ( 100, 0.6 ) {
	using type_parent = ::e2::interface::i_hashtable < K, V, hashtable_lock_striped_impl >;

	type_parent::_trait_hashtable._hash_method = ::e2::trait::hashtable::e_hash_method::universal;
	type_parent::_trait_hashtable._table_method = ::e2::trait::hashtable::e_table_method::open;
	type_parent::_trait_hashtable._lock_load_factor = ::e2::trait::hashtable::e_lock_load_factor::constant;
	type_parent::_trait_concurrency._bound_size = ::e2::trait::concurrency::e_bound_size::unbounded;
	type_parent::_trait_concurrency._method = ::e2::trait::concurrency::e_method::total;
	type_parent::_trait_concurrency._granularity = ::e2::trait::concurrency::e_granularity::disjoint_access;
	type_parent::_trait_concurrency._fairness = ::e2::trait::concurrency::e_fairness::not_applicable;
    }
};

} }

#endif
