#ifndef HASH_UNIVERSAL_CHAIN_HPP
#define HASH_UNIVERSAL_CHAIN_HPP

#include <vector>
#include <iterator>
#include <functional>
#include <type_traits>
#include <random>
#include <cmath>
#include <iostream>

template< class K, class V >
class hash_universal_chain {
public:
                  hash_universal_chain();
                  ~hash_universal_chain();

             bool insert( K const, V const & );
             bool find( K const , V & );
             bool erase( K const );
             bool add_hash_func( std::function< size_t( size_t ) > ); //add hash function into the univeral set
             bool get_hash_func_current( std::function< size_t( size_t ) > & ); //add hash function into the univeral set
             bool resize( size_t size );
           size_t get_table_size();
           size_t get_load_factor();
private:
                  class hashnode {
		  public:
		      size_t _key;
		      hashnode * _next = nullptr;
		      hashnode * _prev = nullptr;
		      V _val;
		  };

            using vec_hash_func =    std::vector< std::function< size_t( size_t )> >;
            using vec_hashnode =     std::vector< hashnode * >;
            using hash_func =        std::function< size_t( size_t ) >;

             bool compute_hash( K const key, size_t & hashed_val );
             bool select_random_hash_func();
             bool set_default_hash_funcs();
             bool prepend_hashnode( hashnode * & node, size_t hashed_key, V val );
             bool remove_hashnode( hashnode * & node );
       hashnode * find_hashnode( hashnode * node, size_t hashed_key );

     vec_hashnode _table;
    vec_hash_func _funcs_hash;
        hash_func _func_hash_selected;
};

#include "hash_universal_chain.tpp"

#endif
