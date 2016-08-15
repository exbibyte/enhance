#ifndef ALLOC_H
#define ALLOC_H

#include <utility>
#include <cstring>

template< typename policy >
class alloc {
public:
    using type = policy;
    template< typename T, typename ... Args >
    T * pool_new( Args &&... args ){
	return static_cast<policy*>(this)->pool_new( std::forward<Args>(args)... );
    }
    template< typename T >
    void pool_delete( T * p ){
	static_cast<policy*>(this)->pool_delete( p );
    }
    void reserve( size_t chunk_size, size_t num_chunks ){
	static_cast<policy*>(this)->reserve( chunk_size, num_chunks );
    }
    void dereserve( size_t n ){
	static_cast<policy*>(this)->dereserve( n );
    }
    double get_loading(){
	return static_cast<policy*>(this)->get_loading();
    }
};

#endif
