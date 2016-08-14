#ifndef ALLOC_H
#define ALLOC_H

template< typename policy >
class alloc {
public:
    using type = policy;
    void * malloc( size_t n ){
	return static_cast<policy*>(this)->malloc( n );
    }
    void free( void * p ){
	static_cast<policy*>(this)->free( p );
    }
    void reserve( size_t n ){
	static_cast<policy*>(this)->reserve( n );
    }
    void dereserve( size_t n ){
	static_cast<policy*>(this)->dereserve( n );
    }
};

#endif
