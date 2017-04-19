#include <stdlib.h>
#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>

#include "list.hpp"
#include "memory.hpp"

int main(){

    list * l;
    mem_alloc( (void**)&l, sizeof(list) );
    list_init( l );

    list * l2;
    mem_alloc( (void**)&l2, sizeof(list) );
    list_init( l2 );
    
    {
	size_t s = list_size( l );
	assert( 0 == s );
    }
    
    for( uint64_t i = 0; i < 10; ++i ){
	list_push_back( l, (uint64_t)( i ) );
    }
    
    {
	size_t s = list_size( l );
	assert( 10 == s );
    }

    for( uint64_t i = 10; i < 20; ++i ){
	list_push_front( l, (uint64_t)( i ) );
    }    

    {
	size_t s = list_size( l );
	assert( 20 == s );
	assert( 20 == list_update_size( l ) );
    }

    int count = 0;
    list_node * n = list_iterator_begin( l );
    while( list_end( l ) != n ){
	int v = (int)( n->_val );
	if( count < 10 ){
	    assert( v == 19 - count );
	}else{
	    assert( v == count - 10 );
	}
	n = list_next( l, n );
	++count;
    }

    for( uint64_t i = 0; i < 10; ++i ){
	list_pop_front( l );
    }
    
    {
	size_t s = list_size( l );
	assert( 10 == s );
	assert( 10 == list_update_size( l ) );
    }
    

    for( uint64_t i = 0; i < 10; ++i ){
        uint64_t v;
	bool ret = list_front( l, &v );
	assert( ret );
	assert( i == v );
	ret = list_pop_front( l );
	assert( ret );
    }

    {
	size_t s = list_size( l );
	assert( 0 == s );
	assert( 0 == list_update_size( l ) );
    }


    for( uint64_t i = 0; i < 10; ++i ){
	list_push_back( l, (uint64_t)( i ) );
    }
    {
	list_node * it = list_iterator_begin( l );
	for( uint64_t i = 0; i < 5; ++i ){
	    it = list_next( l, it );
	}
	it = list_erase( l, it );
	uint64_t v = it->_val;
	assert( 6 == v );
	assert( 9 == list_size( l ) );
	assert( 9 == list_update_size( l ) );
    }
    {
	bool ret = list_pop_back( l );
	assert( ret );
	assert( 8 == list_size( l ) );
	assert( 8 == list_update_size( l ) );
    }

    list_clear( l );
    assert( 0 == list_size( l ) );
    assert( 0 == list_update_size( l ) );

    //splice entire
    {
	for( uint64_t i = 0; i < 10; ++i ){
	    list_push_back( l, (uint64_t)( i ) );
	}
	for( uint64_t i = 0; i < 10; ++i ){
	    list_push_back( l2, (uint64_t)( 2*i ) );
	}

	list_splice_entire( l, list_end( l ), l2 );
	assert( 20 == list_size( l ) );
	assert( 0 == list_size( l2 ) );
	
	list_splice_entire( l2, list_end( l2 ), l );
	assert( 0 == list_size( l ) );
	assert( 20 == list_size( l2 ) );

	assert( 0 == list_update_size( l ) );
	assert( 20 == list_update_size( l2 ) );

	list_node * n = list_iterator_begin( l2 );
	for( uint64_t i = 0; i < 10; ++i ){
	    assert( i == n->_val );
	    n = list_next( l2, n );
	}
	for( uint64_t i = 0; i < 10; ++i ){
	    assert( 2*i == n->_val );
	    n = list_next( l2, n );
	}
    }

    list_clear( l );
    list_clear( l2 );
    assert( 0 == list_size( l ) );
    assert( 0 == list_size( l2 ) );

    //splice single
    {
	for( uint64_t i = 0; i < 10; ++i ){
	    list_push_back( l, (uint64_t)( i ) );
	}
	for( uint64_t i = 0; i < 10; ++i ){
	    list_push_back( l2, (uint64_t)( 2*i ) );
	}
	assert( 10 == list_size( l ) );
	assert( 10 == list_size( l2 ) );
	
	list_splice_single( l, list_end( l ), l2, list_iterator_begin( l2 ) );
	assert( 11 == list_size( l ) );
	assert( 9 == list_size( l2 ) );

	uint64_t v;
	bool ret = list_back( l, &v );
	assert( ret );
	assert( 0 == v );

	list_splice_single( l, list_iterator_begin( l ), l2, list_iterator_end( l2 ) );
	assert( 12 == list_size( l ) );
	assert( 8 == list_size( l2 ) );

	ret = list_front( l, &v );
	assert( ret );
	assert( 18 == v );

	list_node * n = list_iterator_begin( l );
	n = list_next( l, n );
	n = list_next( l, n );
	assert( n->_val == 1 );
	n = list_next( l, n ); //4th item
	assert( n->_val == 2 );

	list_splice_single( l, n, l2, list_iterator_end( l2 ) );
	assert( 13 == list_size( l ) );
	assert( 7 == list_size( l2 ) );

	assert( 13 == list_update_size( l ) );
	assert( 7 == list_update_size( l2 ) );
	
	assert( n->_val == 2 );
	n = list_prev( l, n );
	assert( n->_val == 16 );
    }
    
    list_clear( l );
    list_clear( l2 );
    assert( 0 == list_size( l ) );
    assert( 0 == list_size( l2 ) );

    //splice range partial
    {
	for( uint64_t i = 0; i < 10; ++i ){
	    list_push_back( l, (uint64_t)( i ) );
	}
	for( uint64_t i = 0; i < 10; ++i ){
	    list_push_back( l2, (uint64_t)( 2*i ) );
	}
	assert( 10 == list_size( l ) );
	assert( 10 == list_size( l2 ) );

	list_node * n = list_iterator_begin( l );
	for( uint64_t i = 0; i < 5; ++i ){
	    n = list_next( l, n );
	}

	list_node * n2 = list_iterator_begin( l2 );
	for( uint64_t i = 0; i < 5; ++i ){
	    n2 = list_next( l2, n2 );
	}

	list_node * n3 = list_iterator_end( l2 );

	list_splice_range( l, n, l2, n2, n3 );
	
	assert( 14 == list_size( l ) );
	assert( 6 == list_size( l2 ) );

	assert( 14 == list_update_size( l ) );
	assert( 6 == list_update_size( l2 ) );
	
	list_node * check = list_begin( l );
	for( uint64_t i = 0; i <= 4; ++i ){
	    check = list_next( l, check );
	    assert( i == check->_val );
	}
	for( uint64_t i = 5; i < 9; ++i ){
	    check = list_next( l, check );
	    assert( 2*i == check->_val );
	}
	for( uint64_t i = 5; i < 10; ++i ){
	    check = list_next( l, check );
	    assert( i == check->_val );
	}
    }

    list_clear( l );
    list_clear( l2 );
    assert( 0 == list_size( l ) );
    assert( 0 == list_size( l2 ) );

    //splice range all
    {
	for( uint64_t i = 0; i < 10; ++i ){
	    list_push_back( l, (uint64_t)( i ) );
	}
	for( uint64_t i = 0; i < 10; ++i ){
	    list_push_back( l2, (uint64_t)( 2*i ) );
	}
	assert( 10 == list_size( l ) );
	assert( 10 == list_size( l2 ) );

	list_splice_range( l, list_iterator_begin( l ), l2, list_iterator_begin( l2 ), list_end( l2 ) );
	assert( 20 == list_size( l ) );
	assert( 0 == list_size( l2 ) );

	assert( 20 == list_update_size( l ) );
	assert( 0 == list_update_size( l2 ) );
    }
    
    list_deinit( l );
    list_deinit( l2 );
	
    mem_free( (void*)l );
    mem_free( (void*)l2 );

    printf( "complete.\n" );

    return 0;
}
