#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include "catch.hpp"

#include "SortInsertion.h"

#include <vector>
#include <algorithm>
#include <set>

using namespace std;

TEST_CASE( "SortInsertion", "[SortInsertion]" ) {
    vector< int > unsorted { 9, 3, -1, 6, 7, 23, -8 };
    SECTION( "SortInsertion" ) {
	vector< int > sorted = unsorted;
	SortInsertion< int >::Sort( sorted, 0, sorted.size() -1 );
	int size = sorted.size();
	CHECK( ( unsorted.size() == size ) );
	int val_prev;
	for( int i = 1; i < sorted.size(); ++i ){
	    if( i == 1 ){
		val_prev = sorted[0];
	    }
	    int val_current = sorted[i];
	    CHECK( val_prev <= val_current );
	    val_prev = val_current;
	}
    }    
}
