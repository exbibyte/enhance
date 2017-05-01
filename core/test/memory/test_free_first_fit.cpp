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
    typedef std::list< memory_block_info > t_list;
    void * p;
    size_t len = 10000000;
    std::list< memory_block_info > l { { 0, len } };
    std::list< memory_block_info > l_lent {};
    assert( 1 == l.size() );
    assert( 0 == l_lent.size() );

    p = malloc( len );

    std::vector< node * > objs( 10, nullptr );
    std::vector< node ** > objs_p {};
    //allocating
    
    alloc_first_fit alc;

    node ** n_ret = alc.newing<node>( p, len, &l, &l_lent, (void**)&objs[0], 1, 2 );
    objs_p.push_back( n_ret );
    
    assert( 1 == l.size() );	
    assert( 1 == l_lent.size() );

    assert( sizeof(**n_ret) == sizeof( node ) );
    assert( l.front()._offset == 16 );
    assert( l.front()._len == (10000000 - 16) );

    assert( l_lent.front()._offset == 0 );
    assert( l_lent.front()._len == 16 );
    
    for( int i = 1; i < 10; ++i ){
	node ** n_alloc = alc.newing<node>( p, len, &l, &l_lent, (void**)&objs[i] );
	objs_p.push_back( n_alloc );
	(*n_alloc)->a = i + 1;
	(*n_alloc)->b = 2*(i + 1);
    }
    assert( 1 == l.size() );
    assert( 10 == l_lent.size() );

    for( auto & i : objs ){
	assert( nullptr != i );
    }
	
    for( int i = 0; i < 10; ++i ){
	assert( (*objs_p[i])->a == i + 1 );
	assert( (*objs_p[i])->b == 2*(i + 1) );
    }

    objs[0]->a = 100;

    assert( (*objs_p[0])->a == 100 );
    
    //freeing 
    free_first_fit fr;

    for( auto & i : objs_p ){
	fr.freeing( p, len, &l, &l_lent, *i );
    }

    for( auto & i : objs ){
	assert( nullptr == i );
    }
    
    assert( 1 == l.size() );
    assert( 0 == l_lent.size() );
    
    free( p );
    return 0;
}
