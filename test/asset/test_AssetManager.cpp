//==================================
//
// Summary:      Winged Edge Facilities Test
// Notes:
// Dependencies: N/A
//==================================
#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include "catch.hpp"

#include "AssetManager.h"

#include <map>
#include <vector>
#include <tuple>
#include <utility>
#include <set>
#include <iostream>
#include <string>

using namespace std;

TEST_CASE( "AssetManager Double", "[ALL]" ) {
    SECTION( "Double single item" ){
	AssetManager< std::string, double > manager;
	bool bRet = manager.AddData( "Item_A", 5 );
	bRet &= manager.AddData( "Item_B", 23 );
	bRet &= manager.AddData( "Item_C", -7 );

	double get1, get2, get3;
	bRet &= manager.GetData( "Item_A", get1 );
	bRet &= manager.GetData( "Item_B", get2 );
	bRet &= manager.GetData( "Item_C", get3 );

	CHECK( true == bRet );
	CHECK( 5 == get1 );
	CHECK( 23 == get2 );
	CHECK( -7 == get3 );
    }
    SECTION( "Double vector" ){
	vector< double > vector_A { 4.5, 5.5 };
	AssetManager< std::string, vector< double > > manager;
	bool bRet = manager.AddData( "Item_A", vector_A );
	vector<double> vector_B( std::initializer_list<double>{ 99.9, 22.2, 55.7 } );
	bRet &= manager.AddData( "Item_B", vector_B );

	vector< double > get1, get2;
	get1.clear();
	get2.clear();

	bRet &= manager.GetData( "Item_A", get1 );
	bRet &= manager.GetData( "Item_B", get2 );

	CHECK( true == bRet );
        REQUIRE( 2 == get1.size() );
	CHECK( 4.5 == get1[0] );
	CHECK( 5.5 == get1[1] );
        REQUIRE( 3 == get2.size() );
	CHECK( 99.9 == get2[0] );
	CHECK( 22.2 == get2[1] );
	CHECK( 55.7 == get2[2] );
    }
    SECTION( "Double array" ){
	vector< double > vector_A { 4.5, 5.5 };
	AssetManager< std::string, vector< double > > manager;
	bool bRet = manager.AddData( "Item_A", vector_A );
	shared_ptr< double > get1;
	int size;
	bRet &= manager.GetDataArray( "Item_A", get1, size );
	CHECK( true == bRet );
	REQUIRE( 2 == size );
	CHECK( 4.5 == get1.get()[0] );
	CHECK( 5.5 == get1.get()[1] );
    }
}
TEST_CASE( "AssetManager Int", "[ALL]" ) {
    SECTION( "Int single item" ){
	AssetManager< std::string, int > manager;
	bool bRet = manager.AddData( "Item_A", 5 );
	bRet &= manager.AddData( "Item_B", 23 );
	bRet &= manager.AddData( "Item_C", -7 );

	int get1, get2, get3;
	bRet &= manager.GetData( "Item_A", get1 );
	bRet &= manager.GetData( "Item_B", get2 );
	bRet &= manager.GetData( "Item_C", get3 );

	CHECK( true == bRet );
	CHECK( 5 == get1 );
	CHECK( 23 == get2 );
	CHECK( -7 == get3 );
    }
    SECTION( "Int vector" ){
	vector< int > vector_A { 4, 5 };
	AssetManager< std::string, vector< int > > manager;
	bool bRet = manager.AddData( "Item_A", vector_A );
	vector<int> vector_B( std::initializer_list<int>{ 99, 22, 55 } );
	bRet &= manager.AddData( "Item_B", vector_B );

	vector< int > get1, get2;
	get1.clear();
	get2.clear();

	bRet &= manager.GetData( "Item_A", get1 );
	bRet &= manager.GetData( "Item_B", get2 );

	CHECK( true == bRet );
        REQUIRE( 2 == get1.size() );
	CHECK( 4 == get1[0] );
	CHECK( 5 == get1[1] );
        REQUIRE( 3 == get2.size() );
	CHECK( 99 == get2[0] );
	CHECK( 22 == get2[1] );
	CHECK( 55 == get2[2] );
    }
    SECTION( "Int array" ){
	vector< int > vector_A { 4, 5 };
	AssetManager< std::string, vector< int > > manager;
	bool bRet = manager.AddData( "Item_A", vector_A );
	shared_ptr< int > get1;
	int size;
	bRet &= manager.GetDataArray( "Item_A", get1, size );
	CHECK( true == bRet );
	REQUIRE( 2 == size );
	CHECK( 4 == get1.get()[0] );
	CHECK( 5 == get1.get()[1] );
    }
}
