#include <stdlib.h>
#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>

#include "list_st.hpp"

int main(){

    using node = e2::ds::list_st_uint64_t::iterator;
	
    e2::ds::list_st_uint64_t * l = new e2::ds::list_st_uint64_t;

    e2::ds::list_st_uint64_t * l2 = new e2::ds::list_st_uint64_t;
    
    {
	size_t s = l->size();
	assert( 0 == s );
    }
    
    for( uint64_t i = 0; i < 10; ++i ){
	uint64_t val = (uint64_t)( i );
        l->push_back( &val );
    }
    
    {
	size_t s = l->size();
	assert( 10 == s );
    }

    for( uint64_t i = 10; i < 20; ++i ){
	uint64_t val = (uint64_t)( i );
        l->push_front( &val );
    }    

    {
	size_t s = l->size();
	assert( 20 == s );
	assert( 20 == l->update_size() );
    }

    int count = 0;
    node * n = l->iterator_begin();
    while( l->end() != n ){
	int v = (int)( n->_val );
	if( count < 10 ){
	    assert( v == 19 - count );
	}else{
	    assert( v == count - 10 );
	}
	n = l->next( n );
	++count;
    }

    for( uint64_t i = 0; i < 10; ++i ){
	uint64_t val;
        l->pop_front( &val );
    }
    
    {
	size_t s = l->size();
	assert( 10 == s );
	assert( 10 == l->update_size() );
    }
    

    for( uint64_t i = 0; i < 10; ++i ){
        uint64_t v;
	bool ret = l->front( &v );
	assert( ret );
	assert( i == v );
	uint64_t val;
	ret = l->pop_front( &val );
	assert( ret );
    }

    {
	size_t s = l->size();
	assert( 0 == s );
	assert( 0 == l->update_size() );
    }


    for( uint64_t i = 0; i < 10; ++i ){
	uint64_t val = (uint64_t)( i );
        l->push_back( &val );
    }
    {
	node * it = l->iterator_begin();
	for( uint64_t i = 0; i < 5; ++i ){
	    it = l->next( it );
	}
	it = l->erase( it );
	uint64_t v = it->_val;
	assert( 6 == v );
	assert( 9 == l->size() );
	assert( 9 == l->update_size() );
    }
    {
	uint64_t val;
	bool ret = l->pop_back( &val );
	assert( ret );
	assert( 8 == l->size() );
	assert( 8 == l->update_size() );
    }

    l->clear();
    assert( 0 == l->size() );
    assert( 0 == l->update_size() );

    l2->clear();
    assert( 0 == l2->size() );
    assert( 0 == l2->update_size() );
    
    //splice entire
    {
	for( uint64_t i = 0; i < 10; ++i ){
	    uint64_t val = (uint64_t)( i );
	    l->push_back( &val );
	}
	for( uint64_t i = 0; i < 10; ++i ){
	    uint64_t val = (uint64_t)( 2 * i );
	    l2->push_back( &val );
	}

	assert( 10 == l->update_size() );
	assert( 10 == l2->update_size() );
	
	l->splice_entire( l->end(), l2 );
	assert( 20 == l->size() );
	assert( 0 == l2->size() );

	assert( 20 == l->update_size() );
	assert( 0 == l2->update_size() );
	
	l2->splice_entire( l2->end(), l );
	assert( 0 == l->size() );
	assert( 20 == l2->size() );

	assert( 0 == l->update_size() );
	assert( 20 == l2->update_size() );

	node * n = l2->iterator_begin();
	for( uint64_t i = 0; i < 10; ++i ){
	    assert( i == n->_val );
	    n = l2->next( n );
	}
	for( uint64_t i = 0; i < 10; ++i ){
	    assert( 2*i == n->_val );
	    n = l2->next( n );
	}
    }

    l->clear();
    l2->clear();
    assert( 0 == l->size() );
    assert( 0 == l2->size() );

    //splice single
    {
	for( uint64_t i = 0; i < 10; ++i ){
	    uint64_t val = (uint64_t)( i );
	    l->push_back( &val );
	}
	for( uint64_t i = 0; i < 10; ++i ){
	    uint64_t val = (uint64_t)( 2 * i );
	    l2->push_back( &val );
	}
	assert( 10 == l->size() );
	assert( 10 == l2->size() );
	
	l->splice_single( l->end(), l2, l2->iterator_begin() );
	assert( 11 == l->size() );
	assert( 9 == l2->size() );

	uint64_t v;
	bool ret = l->back( &v );
	assert( ret );
	assert( 0 == v );

	l->splice_single( l->iterator_begin(), l2, l2->iterator_end() );
	assert( 12 == l->size() );
	assert( 8 == l2->size() );

	ret = l->front( &v );
	assert( ret );
	assert( 18 == v );

	node * n = l->iterator_begin();
	n = l->next( n );
	n = l->next( n );
	assert( n->_val == 1 );
	n = l->next( n ); //4th item
	assert( n->_val == 2 );

	l->splice_single( n, l2, l2->iterator_end() );
	assert( 13 == l->size() );
	assert( 7 == l2->size() );

	assert( 13 == l->update_size() );
	assert( 7 == l2->update_size() );
	
	assert( n->_val == 2 );
	n = l->prev( n );
	assert( n->_val == 16 );
    }
    
    l->clear();
    l2->clear();
    assert( 0 == l->size() );
    assert( 0 == l2->size() );

    //splice range partial
    {
	for( uint64_t i = 0; i < 10; ++i ){
	    uint64_t val = (uint64_t)( i );
	    l->push_back( &val );
	}
	for( uint64_t i = 0; i < 10; ++i ){
	    uint64_t val = (uint64_t)( 2 * i );
	    l2->push_back( &val );
	}
	assert( 10 == l->size() );
	assert( 10 == l2->size() );

	node * n = l->iterator_begin();
	for( uint64_t i = 0; i < 5; ++i ){
	    n = l->next( n );
	}

	node * n2 = l2->iterator_begin();
	for( uint64_t i = 0; i < 5; ++i ){
	    n2 = l2->next( n2 );
	}

	node * n3 = l2->iterator_end();

	l->splice_range( n, l2, n2, n3 );
	
	assert( 14 == l->size() );
	assert( 6 == l2->size() );

	assert( 14 == l->update_size() );
	assert( 6 == l2->update_size() );
	
	node * check = l->begin();
	for( uint64_t i = 0; i <= 4; ++i ){
	    check = l->next( check );
	    assert( i == check->_val );
	}
	for( uint64_t i = 5; i < 9; ++i ){
	    check = l->next( check );
	    assert( 2*i == check->_val );
	}
	for( uint64_t i = 5; i < 10; ++i ){
	    check = l->next( check );
	    assert( i == check->_val );
	}
    }

    l->clear();
    l2->clear();
    assert( 0 == l->size() );
    assert( 0 == l2->size() );

    //splice range all
    {
	for( uint64_t i = 0; i < 10; ++i ){
	    uint64_t val = (uint64_t)( i );
	    l->push_back( &val );
	}
	for( uint64_t i = 0; i < 10; ++i ){
	    uint64_t val = (uint64_t)( 2 * i );
	    l2->push_back( &val );
	}
	assert( 10 == l->size() );
	assert( 10 == l2->size() );

	l->splice_range( l->iterator_begin(), l2, l2->iterator_begin(), l2->end() );
	assert( 20 == l->size() );
	assert( 0 == l2->size() );

	assert( 20 == l->update_size() );
	assert( 0 == l2->update_size() );
    }

    l->~list_st_uint64_t();
    l2->~list_st_uint64_t();
    
    delete l;
    delete l2;

    printf( "complete.\n" );

    return 0;
}
