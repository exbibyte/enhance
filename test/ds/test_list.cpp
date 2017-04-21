#include <stdlib.h>
#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>

#include "list_st.hpp"
#include "memory.hpp"

int main(){

    using namespace i_list;
    using namespace i_startup;
    using namespace i_iterable;
    using namespace i_spliceable;

    list_st::list_node * tag = nullptr;
    
    list_st::list * l;
    mem_alloc( (void**)&l, sizeof(list_st::list) );
    i_init( l );

    list_st::list * l2;
    mem_alloc( (void**)&l2, sizeof(list_st::list) );
    i_init( l2 );
    
    {
	size_t s = i_list_size( l );
	assert( 0 == s );
    }
    
    for( uint64_t i = 0; i < 10; ++i ){
	uint64_t val = (uint64_t)( i );
	i_list_push_back( l, &val );
    }
    
    {
	size_t s = i_list_size( l );
	assert( 10 == s );
    }

    for( uint64_t i = 10; i < 20; ++i ){
	uint64_t val = (uint64_t)( i );
	list_push_front( l, &val );
    }    

    {
	size_t s = i_list_size( l );
	assert( 20 == s );
	assert( 20 == i_list_update_size( l ) );
    }

    int count = 0;
    list_st::list_node * n = i_iterator_begin( l, tag );
    while( i_end( l, tag ) != n ){
	int v = (int)( n->_val );
	if( count < 10 ){
	    assert( v == 19 - count );
	}else{
	    assert( v == count - 10 );
	}
	n = i_next( l, n );
	++count;
    }

    for( uint64_t i = 0; i < 10; ++i ){
	uint64_t val;
	i_list_pop_front( l, &val );
    }
    
    {
	size_t s = i_list_size( l );
	assert( 10 == s );
	assert( 10 == i_list_update_size( l ) );
    }
    

    for( uint64_t i = 0; i < 10; ++i ){
        uint64_t v;
	bool ret = list_front( l, &v );
	assert( ret );
	assert( i == v );
	uint64_t val;
	ret = i_list_pop_front( l, &val );
	assert( ret );
    }

    {
	size_t s = i_list_size( l );
	assert( 0 == s );
	assert( 0 == i_list_update_size( l ) );
    }


    for( uint64_t i = 0; i < 10; ++i ){
	uint64_t val = (uint64_t)( i );
	i_list_push_back( l, &val );
    }
    {
	list_st::list_node * it = i_iterator_begin( l, tag );
	for( uint64_t i = 0; i < 5; ++i ){
	    it = i_next( l, it );
	}
	it = list_erase( l, it );
	uint64_t v = it->_val;
	assert( 6 == v );
	assert( 9 == i_list_size( l ) );
	assert( 9 == i_list_update_size( l ) );
    }
    {
	uint64_t val;
	bool ret = i_list_pop_back( l, &val );
	assert( ret );
	assert( 8 == i_list_size( l ) );
	assert( 8 == i_list_update_size( l ) );
    }

    i_list_clear( l );
    assert( 0 == i_list_size( l ) );
    assert( 0 == i_list_update_size( l ) );

    //splice entire
    {
	for( uint64_t i = 0; i < 10; ++i ){
	    uint64_t val = (uint64_t)( i );
	    i_list_push_back( l, &val );
	}
	for( uint64_t i = 0; i < 10; ++i ){
	    uint64_t val = (uint64_t)( 2 * i );
	    i_list_push_back( l2, &val );
	}

	i_list_splice_entire( l, i_end( l, tag ), l2 );
	assert( 20 == i_list_size( l ) );
	assert( 0 == i_list_size( l2 ) );
	
	i_list_splice_entire( l2, i_end( l2, tag ), l );
	assert( 0 == i_list_size( l ) );
	assert( 20 == i_list_size( l2 ) );

	assert( 0 == i_list_update_size( l ) );
	assert( 20 == i_list_update_size( l2 ) );

	list_st::list_node * n = i_iterator_begin( l2, tag  );
	for( uint64_t i = 0; i < 10; ++i ){
	    assert( i == n->_val );
	    n = i_next( l2, n );
	}
	for( uint64_t i = 0; i < 10; ++i ){
	    assert( 2*i == n->_val );
	    n = i_next( l2, n );
	}
    }

    i_list_clear( l );
    i_list_clear( l2 );
    assert( 0 == i_list_size( l ) );
    assert( 0 == i_list_size( l2 ) );

    //splice single
    {
	for( uint64_t i = 0; i < 10; ++i ){
	    uint64_t val = (uint64_t)( i );
	    i_list_push_back( l, &val );
	}
	for( uint64_t i = 0; i < 10; ++i ){
	    uint64_t val = (uint64_t)( 2 * i );
	    i_list_push_back( l2, &val );
	}
	assert( 10 == i_list_size( l ) );
	assert( 10 == i_list_size( l2 ) );
	
	i_list_splice_single( l, i_end( l, tag ), l2, i_iterator_begin( l2, tag ) );
	assert( 11 == i_list_size( l ) );
	assert( 9 == i_list_size( l2 ) );

	uint64_t v;
	bool ret = list_back( l, &v );
	assert( ret );
	assert( 0 == v );

	i_list_splice_single( l, i_iterator_begin( l, tag ), l2, i_iterator_end( l2, tag ) );
	assert( 12 == i_list_size( l ) );
	assert( 8 == i_list_size( l2 ) );

	ret = list_front( l, &v );
	assert( ret );
	assert( 18 == v );

	list_st::list_node * n = i_iterator_begin( l, tag );
	n = i_next( l, n );
	n = i_next( l, n );
	assert( n->_val == 1 );
	n = i_next( l, n ); //4th item
	assert( n->_val == 2 );

	i_list_splice_single( l, n, l2, i_iterator_end( l2, tag ) );
	assert( 13 == i_list_size( l ) );
	assert( 7 == i_list_size( l2 ) );

	assert( 13 == i_list_update_size( l ) );
	assert( 7 == i_list_update_size( l2 ) );
	
	assert( n->_val == 2 );
	n = i_prev( l, n );
	assert( n->_val == 16 );
    }
    
    i_list_clear( l );
    i_list_clear( l2 );
    assert( 0 == i_list_size( l ) );
    assert( 0 == i_list_size( l2 ) );

    //splice range partial
    {
	for( uint64_t i = 0; i < 10; ++i ){
	    uint64_t val = (uint64_t)( i );
	    i_list_push_back( l, &val );
	}
	for( uint64_t i = 0; i < 10; ++i ){
	    uint64_t val = (uint64_t)( 2 * i );
	    i_list_push_back( l2, &val );
	}
	assert( 10 == i_list_size( l ) );
	assert( 10 == i_list_size( l2 ) );

	list_st::list_node * n = i_iterator_begin( l, tag );
	for( uint64_t i = 0; i < 5; ++i ){
	    n = i_next( l, n );
	}

	list_st::list_node * n2 = i_iterator_begin( l2, tag );
	for( uint64_t i = 0; i < 5; ++i ){
	    n2 = i_next( l2, n2 );
	}

	list_st::list_node * n3 = i_iterator_end( l2, tag );

	i_list_splice_range( l, n, l2, n2, n3 );
	
	assert( 14 == i_list_size( l ) );
	assert( 6 == i_list_size( l2 ) );

	assert( 14 == i_list_update_size( l ) );
	assert( 6 == i_list_update_size( l2 ) );
	
	list_st::list_node * check = i_begin( l, tag );
	for( uint64_t i = 0; i <= 4; ++i ){
	    check = i_next( l, check );
	    assert( i == check->_val );
	}
	for( uint64_t i = 5; i < 9; ++i ){
	    check = i_next( l, check );
	    assert( 2*i == check->_val );
	}
	for( uint64_t i = 5; i < 10; ++i ){
	    check = i_next( l, check );
	    assert( i == check->_val );
	}
    }

    i_list_clear( l );
    i_list_clear( l2 );
    assert( 0 == i_list_size( l ) );
    assert( 0 == i_list_size( l2 ) );

    //splice range all
    {
	for( uint64_t i = 0; i < 10; ++i ){
	    uint64_t val = (uint64_t)( i );
	    i_list_push_back( l, &val );
	}
	for( uint64_t i = 0; i < 10; ++i ){
	    uint64_t val = (uint64_t)( 2 * i );
	    i_list_push_back( l2, &val );
	}
	assert( 10 == i_list_size( l ) );
	assert( 10 == i_list_size( l2 ) );

	i_list_splice_range( l, i_iterator_begin( l, tag ), l2, i_iterator_begin( l2, tag ), i_end( l2, tag ) );
	assert( 20 == i_list_size( l ) );
	assert( 0 == i_list_size( l2 ) );

	assert( 20 == i_list_update_size( l ) );
	assert( 0 == i_list_update_size( l2 ) );
    }
    
    i_deinit( l );
    i_deinit( l2 );
	
    mem_free( (void*)l );
    mem_free( (void*)l2 );

    printf( "complete.\n" );

    return 0;
}
