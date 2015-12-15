//==================================
//
// Summary:      Winged Edge Facilities Test
// Notes:
// Dependencies: N/A
//==================================
#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include "catch.hpp"

#include "InstanceManager.h"
#include "RenderType.h"

#include <map>
#include <vector>
#include <tuple>
#include <utility>
#include <set>
#include <iostream>
#include <string>
#include <memory>

using namespace std;

TEST_CASE( "InstanceManager RenderPoly", "[ALL]" ) {
    SECTION( "RenderPoly" ){
	InstanceManager< RenderPolyVertices > manager;
	vector< float > data = initializer_list<float>{ 2, 4, 6 };
	bool bRet = manager.AddData( 1, data );
	vector< float > retrieve;
	shared_ptr< float > retrieve_array;
	int size;
	bRet &= manager.GetData( 1, retrieve );
	bRet &= manager.GetDataArray( 1, retrieve_array, size );
	CHECK( true == bRet );
        REQUIRE( 3 == size );
	CHECK( 2 == retrieve[0] );
	CHECK( 4 == retrieve[1] );
	CHECK( 6 == retrieve[2] );
    }
}
