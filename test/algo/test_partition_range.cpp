#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#include "partition_range.hpp"

#include <iostream>
#include <vector>
#include <list>
#include <utility>

using namespace std;

TEST_CASE( "partition_range", "[partition_range]" ) {
    list< pair<int,int> > input { {0,11}, {6,6}, {10,15} };
    cout << "input: ";
    for( auto & i : input ){
	cout << "(" << i.first << ", " << i.second << ") ";
    }
    cout << endl;
    list< pair<int,int> > output{};
    SECTION( "partition_range" ) {
	partition_range::partition( input, output );
	CHECK( output.size() == 4 );
	int last = -99999;
	cout << "output: ";
	for( auto & i : output ){
	    cout << "(" << i.first << ", " << i.second << ") ";
	    CHECK( i.first > last );
	    CHECK( i.first <= i.second );
	    last = i.second;
	}
	cout << endl;
	CHECK( ( output.begin()->first == 0 ) );
	CHECK( ( output.rbegin()->second == 15 ) );
    }
}
