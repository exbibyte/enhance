#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include "catch.hpp"

#include "sort_bucket.hpp"

#include <vector>
#include <algorithm>
#include <iostream>
#include <utility>

using namespace std;

TEST_CASE( "sort_bucket", "[sort_bucket]" ) {
    vector< pair<double, unsigned int> > input { {0.9,0}, {0.3,1}, {0.4,2}, {0.6,7}, {0.1,4}, {0,5}, {0.33,6}, {0.66,3} };
    vector< pair<double, unsigned int> > output;
    vector< pair<double, unsigned int> > sort_check { {0,5}, {0.1,4}, {0.3,1}, {0.33,6}, {0.4,2}, {0.6,7}, {0.66,3}, {0.9,0} };
    SECTION( "bucket sort" ) {
	bool bRet;
	bRet = sort_bucket::sort( input, output, 3 );
	CHECK( bRet );
	unsigned int size_output = output.size();
	unsigned int size_input = input.size();
        REQUIRE( size_output == size_input );
	for( int i = 0; i < input.size(); i++ ){
	    double val_key = output[ i ].first;
	    unsigned int val_id = output[ i ].second;
	    double expect_val_key = sort_check[i].first;
	    unsigned int expect_val_id = sort_check[i].second;
	    CHECK( val_key == expect_val_key );
	    CHECK( val_id == expect_val_id );
	}
    }
}
