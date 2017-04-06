#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include "catch.hpp"

#include "sort_radix.hpp"
#include "sort_count.hpp"

#include <vector>
#include <algorithm>
#include <iostream>
#include <utility>

using namespace std;

TEST_CASE( "sort_radix", "[sort_radix]" ) {
    vector< pair<vector<unsigned int>, unsigned int> > input { { {9,3,0,7}, 1}, { {4,9,2,3}, 0}, { {4,8,2,3}, 2}, { {9,3,0,6}, 3}, { {9,3,0,6}, 4} };
    vector< pair<vector<unsigned int>, unsigned int> > output;
    vector< pair<vector<unsigned int>, unsigned int> > sort_check { { {4,8,2,3}, 2}, { {4,9,2,3}, 0}, { {9,3,0,6}, 3}, { {9,3,0,6}, 4}, { {9,3,0,7}, 1} };
    SECTION( "radix sort" ) {
	bool bRet;
	bRet = sort_radix::sort( sort_count::sort, input, output );
	CHECK( bRet );
	unsigned int size_output = output.size();
	unsigned int size_input = input.size();
        REQUIRE( size_output == size_input );
	for( int i = 0; i < input.size(); i++ ){
	    vector<unsigned int> & val_key = output[ i ].first;
	    unsigned int val_id = output[ i ].second;
	    auto & expect_val_key = sort_check[i].first;
	    auto expect_val_id = sort_check[i].second;
	    REQUIRE( val_key.size() == expect_val_key.size() );
	    for( int j = 0; j < val_key.size(); ++j ){
		CHECK( val_key[j] == expect_val_key[j] );
	    }
	    CHECK( val_id == expect_val_id );
	}
    }
}
