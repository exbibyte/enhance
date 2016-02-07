//==================================
//
// Summary:      Batcher
// Notes:
// Dependencies: N/A
//==================================
#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include "catch.hpp"

#include "Batch.h"

#include <map>
#include <vector>
#include <tuple>
#include <utility>
#include <set>
#include <iostream>
#include <string>

using namespace std;

TEST_CASE( "Batch Process", "[ALL]" ) {
    SECTION( "Double single item" ){
        Batch< double > batch;
	bool bRet = batch.AddToBatch( 0 );
	bRet &= batch.AddToBatch( 1 );
	bRet &= batch.AddToBatch( 2 );
	bRet &= batch.AddToBatch( 3 );
	CHECK( true == bRet );

	vector< double > vec_data;
	bRet = batch.GetBatch( vec_data );
	CHECK( true == bRet );
	REQUIRE( 4 == vec_data.size() );
	CHECK( 0 == vec_data[0] );
	CHECK( 1 == vec_data[1] );
	CHECK( 2 == vec_data[2] );
	CHECK( 3 == vec_data[3] );
    }
    SECTION( "Double vector" ){
	vector< double > vec_data_set { 0.0, 1.1, 2.2, 3.3 };
        Batch< double > batch;
	bool bRet = batch.AddToBatch( 0 );
	bRet &= batch.AddToBatch( vec_data_set );
	CHECK( true == bRet );

	vector< double > vec_data;
	bRet = batch.GetBatch( vec_data );
	CHECK( true == bRet );
	REQUIRE( 5 == vec_data.size() );
	CHECK( 0 == vec_data[0] );
	CHECK( 0 == vec_data[1] );
	CHECK( 1.1 == vec_data[2] );
	CHECK( 2.2 == vec_data[3] );
	CHECK( 3.3 == vec_data[4] );
    }
}
