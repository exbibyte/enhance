#ifndef I_HASH_HPP
#define I_HASH_HPP

#include <vector>
#include <iterator>
#include <functional>
#include <type_traits>
#include <random>
#include <cmath>
#include <iostream>

template< class K, class V, template<> class Impl >
class IHash< K, V > final : public Impl< K, V > {
public:
         using key_type =               K;
         using key_const_type =         K const;
         using value_type =             V;
         using val_reference =          V &;
         using val_const_reference =    V const &;

               template< class... Args >
               IHash( Args... args ) : Impl< K, V >( std::forward<Args>( args )... ) {}
               ~IHash(){ this->~Impl< K, V >(); }

          bool insert( key_const_type, val_const_reference );
          bool find( key_const_type, val_reference );
          bool erase( key_const_type );
          bool add_hash_func( std::function<size_t(size_t)> );
          bool get_hash_func_current( std::function<size_t(size_t)> & );
          bool resize( size_t size );
        size_t get_table_size();
};

#endif
