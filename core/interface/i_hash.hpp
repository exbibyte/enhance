#ifndef I_HASH_HPP
#define I_HASH_HPP

#include <functional>
#include <vector>
#include <cstring>

template< class T, template< class > class Impl >
class i_hash_gen_funcs final : public Impl< T > {
public:
              template< class... Args >
              i_hash_gen_funcs( Args... args ) : Impl< T >( std::forward<Args>(args)... ) {}
              ~i_hash_gen_funcs(){}

	      bool generate_hash_funcs( size_t const table_size, std::vector< std::function < T ( T ) > > * funcs ){ return Impl< T >::generate_hash_funcs( table_size, funcs ); }
};

#endif
