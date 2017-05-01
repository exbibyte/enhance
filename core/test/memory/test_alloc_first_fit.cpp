#include <list>
#include <cstdint>
#include <cassert>

#include "alloc_first_fit.hpp"

struct node {
    node(){}
    node( int x, int y ){
	a = x;
	b = y;
    }
    int a;
    int b;
    int c;
    int d;
};

using namespace e2::memory;

int main(){
    typedef std::list<memory_block_info > t_list;
    void * p;
    size_t len = 10000000;
    std::list< memory_block_info > l { { 0, len } };
    std::list< memory_block_info > l_lent {};
    assert( 1 == l.size() );
    assert( 0 == l_lent.size() );

    p = malloc( len );
    
    alloc_first_fit alc;

    node * n;
    node ** n_ret = alc.newing<node>( p, len, &l, &l_lent, (void**)&n, 45, 90 );

    assert( nullptr != n_ret && nullptr != *n_ret );
	
    assert( 1 == l.size() );	
    assert( 1 == l_lent.size() );

    assert( sizeof(**n_ret) == sizeof( node ) );
    assert( l.front()._offset == 16 );
    assert( l.front()._len == (10000000 - 16) );

    assert( l_lent.front()._offset == 0 );
    assert( l_lent.front()._len == 16 );

    assert( (*n_ret)->a == 45 );
    assert( (*n_ret)->b == 90 );

    for( int i = 0; i < 9; ++i ){
	node ** n_allocated = alc.newing<node>( p, len, &l, &l_lent, (void**)&n );
    }
    assert( 1 == l.size() );
    assert( 10 == l_lent.size() );
    
    free( p );
    return 0;
}
