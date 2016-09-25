#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include "catch.hpp"

#include "heap.hpp"

#include <vector>
#include <algorithm>
#include <set>

using namespace std;

TEST_CASE( "Heap", "[Heap]" ) {
    heap_max< int > heap_max;
    vector< typename ::heap_max< int >::_t_unit > unsorted { { 0, 100 }, { 9, 101 }, { -5, 102 }, { 6, 103 }, { -6, 104 }, { -8, 105 }, };
    vector< typename ::heap_max< int >::_t_unit > empty;
    vector< typename ::heap_max< int >::_t_unit > retrieve_sorted;
    typename ::heap_max< int >::_t_unit retrieve;
    SECTION( "Heap Empty" ) {
	bool bRet;
	heap_max.build_heap( empty );
	bRet = heap_max.heapify( 0 );
	CHECK( bRet );
	bRet = heap_max.get_max( retrieve );
	CHECK( !bRet );
	bRet = heap_max.extract_max( retrieve );
	CHECK( !bRet );
	bRet = heap_max.increase_val( 0, 110 );
	CHECK( !bRet );
    }
    SECTION( "Heap Max" ) {
	bool bRet;
	int val, data;
	heap_max.build_heap( unsorted );
	bRet = heap_max.heapify( 0 );
	CHECK( bRet );
	bRet = heap_max.get_max( retrieve );
	CHECK( bRet );
	val = retrieve._val;
	data = retrieve._data;
	CHECK( 9 == val );
	CHECK( 101 == data );
	bRet = heap_max.extract_max( retrieve );
	val = retrieve._val;
	data = retrieve._data;
	CHECK( 9 == val );
	CHECK( 101 == data );	
	bRet = heap_max.extract_max( retrieve );
	CHECK( bRet );
	val = retrieve._val;
	data = retrieve._data;
	CHECK( 6 == val );
	CHECK( 103 == data );
	bRet = heap_max.increase_val( 3, 99 );
	bRet = heap_max.get_max( retrieve );
	CHECK( bRet );
	val = retrieve._val;
	data = retrieve._data;
	CHECK( 99 == val );
	CHECK( 105 == data );
	heap_max.sort( retrieve_sorted );
	CHECK( 4 == retrieve_sorted.size() );
	int val_prev;
	std::set< int > expected_vals { 99, 0, -5, -6 };
	for( int i = 0; i < retrieve_sorted.size(); ++i ){
	    if( 0 == i ){
		val_prev = retrieve_sorted[i]._val;
	    }
	    int val_current = retrieve_sorted[i]._val;
	    CHECK( val_prev <= val_current );
	    val_prev = val_current;
	    auto it_find = expected_vals.find( val_current );
	    if( it_find != expected_vals.end() ){
		expected_vals.erase( it_find );
	    }
	}
	int size_remain = expected_vals.size();
	CHECK( 0 == size_remain );
    }
    SECTION( "Heap insert" ) {
	bool bRet;
	typename ::heap_max< int >::_t_unit unit_2 {10,3};
	typename ::heap_max< int >::_t_unit unit_3 {6,4};
	typename ::heap_max< int >::_t_unit unit_4 {15,5};
	typename ::heap_max< int >::_t_unit unit_1 {3,2};
	typename ::heap_max< int >::_t_unit unit_0 {4,1};

	bRet = heap_max.insert( unit_0 );
	bRet &= heap_max.insert( unit_1 );
	bRet &= heap_max.insert( unit_2 );
	bRet &= heap_max.insert( unit_3 );
	bRet &= heap_max.insert( unit_4 );
	CHECK( bRet );
	
	bRet = heap_max.extract_max( retrieve );
	CHECK( bRet );
	CHECK( retrieve._val == 15 );
	CHECK( retrieve._data == 5 );

	bRet = heap_max.extract_max( retrieve );
	CHECK( bRet );
	CHECK( retrieve._val == 10 );
	CHECK( retrieve._data == 3 );

	bRet = heap_max.extract_max( retrieve );
	CHECK( bRet );
	CHECK( retrieve._val == 6 );
	CHECK( retrieve._data == 4 );

	bRet = heap_max.extract_max( retrieve );
	CHECK( bRet );
	CHECK( retrieve._val == 4 );
	CHECK( retrieve._data == 1 );

	bRet = heap_max.extract_max( retrieve );
	CHECK( bRet );
	CHECK( retrieve._val == 3 );
	CHECK( retrieve._data == 2 );

	bRet = heap_max.extract_max( retrieve );
	CHECK( !bRet );
    }
}
