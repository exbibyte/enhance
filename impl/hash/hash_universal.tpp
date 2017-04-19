#include <random>

template< class T >
bool hash_universal_impl< T >::generate_hash_funcs( size_t const table_size, std::vector< std::function< T ( T ) > > * funcs ){
    if( 0 >= table_size )
	return false;

    funcs->clear();

    constexpr T p = 2147483647; //2^31-1 prime number

    //select coefficients for hash function ( ( a * hashed_key + b ) mod p ) mod table_size
    std::random_device rd;
    std::mt19937 engine( rd() );
    std::uniform_int_distribution< T > distr_a( 1, p-1 ); //a from Zp* = [1,p-1]
    std::uniform_int_distribution< T > distr_b( 0, p-1 ); //b from Zp = [0,p-1]

    //create different instances from this set of hash functions Zp* and Zp
    for( size_t i = 0; i < 10; ++i ){
	T a = distr_a( engine );
	T b = distr_b( engine );
	auto hash_func = std::function< T ( T ) >( [=]( T hashed_key ) -> T {
		return ( ( a * hashed_key + b ) % p ) % table_size;
	    });
	funcs->push_back( hash_func );
    }

    return true;
}
