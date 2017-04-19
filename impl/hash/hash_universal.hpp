#ifndef HASH_UNIVERSAL_HPP
#define HASH_UNIVERSAL_HPP

#include <functional>
#include <vector>
#include <cstring>

#include "i_hash.hpp"

template< class T >
class hash_universal_impl {
public:
          hash_universal_impl(){}
          ~hash_universal_impl(){}
          bool generate_hash_funcs( size_t const table_size, std::vector< std::function< T ( T ) > > * funcs );
};

#include "hash_universal.tpp"

template< class T >
using hash_universal = i_hash_gen_funcs < T, hash_universal_impl >;

#endif
