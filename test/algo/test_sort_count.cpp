#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include "catch.hpp"

#include "sort_count.hpp"

#include <vector>
#include <algorithm>
#include <iostream>
#include <utility>

using namespace std;

TEST_CASE( "sort_count", "[sort_count]" ) {
    vector< pair<unsigned int, unsigned int> > input { {9,0}, {3,1}, {4,2}, {6,7}, {1,4}, {0,5}, {3,6}, {6,3} };
    vector< pair<unsigned int, unsigned int> > output;
    vector< pair<unsigned int, unsigned int> > sort_check { {0,5}, {1,4}, {3,1}, {3,6}, {4,2}, {6,7}, {6,3}, {9,0} };
    SECTION( "counting sort" ) {
	bool bRet;
	bRet = sort_count::sort( input, output );
	CHECK( bRet );
	unsigned int size_output = output.size();
	unsigned int size_input = input.size();
        REQUIRE( size_output == size_input );
	for( int i = 0; i < input.size(); i++ ){
	    unsigned int val_key = output[ i ].first;
	    unsigned int val_id = output[ i ].second;
	    auto expect_val_key = sort_check[i].first;
	    auto expect_val_id = sort_check[i].second;
	    CHECK( val_key == expect_val_key );
	    CHECK( val_id == expect_val_id );
	}
    }
}
