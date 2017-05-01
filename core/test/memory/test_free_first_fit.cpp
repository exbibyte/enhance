#include <list>
#include <cstdint>
#include <cassert>
#include <vector>

#include "alloc_first_fit.hpp"
#include "free_first_fit.hpp"

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
    typedef std::list<std::pair<size_t, size_t> > t_list;
    void * p;
    size_t len = 10000000;
    std::list<std::pair<size_t, size_t> > l { { 0, len } };
    std::list<std::pair<size_t, size_t> > l_lent {};
    assert( 1 == l.size() );
    assert( 0 == l_lent.size() );

    p = malloc( len );

    std::vector< node * > objs {};
    //allocating
    
    alloc_first_fit alc;

    node * n;
    node ** n_p = &n;
    n = alc.newing<node>( p, len, &l, &l_lent, (void**)n_p, 45, 90 );
    objs.push_back( n );
    
    assert( 1 == l.size() );	
    assert( 1 == l_lent.size() );

    assert( sizeof(*n) == sizeof( node ) );
    assert( l.front().first == 16 );
    assert( l.front().second == (10000000 - 16) );

    assert( l_lent.front().first == 0 );
    assert( l_lent.front().second == 16 );

    assert( n->a == 45 );
    assert( n->b == 90 );
    
    for( int i = 0; i < 9; ++i ){
	n = alc.newing<node>( p, len, &l, &l_lent, (void**)n_p );
	objs.push_back( n );
    }
    assert( 1 == l.size() );
    assert( 10 == l_lent.size() );

    //freeing 
    free_first_fit fr;

    for( auto & i : objs ){
	fr.freeing( p, len, &l, &l_lent, i );
    }

    assert( 1 == l.size() );
    assert( 0 == l_lent.size() );
    
    free( p );
    return 0;
}
