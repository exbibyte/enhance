//==================================
//
// Summary:      InstanceManagerIter test case
// Notes:
// Dependencies: N/A
//==================================
#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include "catch.hpp"

#define NDEBUG //disable any assert in included files

#include "enInstanceManagerIter.h"
#include "InstanceType.h"

#include <map>
#include <vector>
#include <tuple>
#include <utility>
#include <set>
#include <iostream>

using namespace std;

TEST_CASE( "enInstanceManagerIter", "[ALL]" ) {
    SECTION( "enInstanceManagerIter" ){

	enInstanceManagerIter _manager_empty({});

	vector<double> vec_data_double;
	vec_data_double.push_back(55.5);
	vec_data_double.push_back(33.33);
	bool bRet = _manager_empty.SetData( 99, vec_data_double );
	CHECK( true == bRet );
	vector<double> query_vec_data_double;
	bRet = _manager_empty.QueryData( 99, query_vec_data_double );
	CHECK( true == bRet );
	CHECK( query_vec_data_double.size() == 2 );
	CHECK( query_vec_data_double[0] == 55.5 );
	CHECK( query_vec_data_double[1] == 33.33 );
		
	enInstanceManagerIter _manager2 ( { eInstanceType::PolyVerts } );
	_manager2.SetData( 1, eInstanceType::PolyVerts, 25 );
	enInstanceManagerIter _manager3 ( { eInstanceType::Displacement } );
	_manager3.SetData( 1, eInstanceType::Displacement, 26 );
				 
        enInstanceManagerIter _manager( { eInstanceType::ObjectGeneric, eInstanceType::Player, eInstanceType::Spectator } );
        bRet = _manager.SetData( 1, eInstanceType::ObjectGeneric, 5 );
	bRet &= _manager.SetData( 1, eInstanceType::Spectator, 10 );
	CHECK( true == bRet );
	
	int query1, query2, query3;
	bRet = _manager.QueryData( 1, eInstanceType::ObjectGeneric, query1 );
	CHECK( true == bRet );
	CHECK( 5 == query1 );
	bRet = _manager.QueryData( 1, eInstanceType::Spectator, query2 );
	CHECK( true == bRet );
	CHECK( 10 == query2 );
	bRet = _manager.QueryData( 2, eInstanceType::Player, query3 );
	CHECK( false == bRet );
	bRet = _manager.QueryData( 2, eInstanceType::Spectator, query3 );
	CHECK( false == bRet );


	bRet = _manager.LinkExternalInstanceManager( { { eInstanceType::ObjectGeneric, &_manager2 }, { eInstanceType::Spectator, &_manager3 } } );
	CHECK( true == bRet );

	enInstanceManagerIter * p_external_manager1, * p_external_manager2;
	bRet = _manager.GetExternalInstanceManager( eInstanceType::ObjectGeneric, p_external_manager1 );
	CHECK( true == bRet );
	p_external_manager1->QueryData( 1, eInstanceType::PolyVerts, query3 );
	CHECK( 25 == query3 );

	bRet = _manager.GetExternalInstanceManager( eInstanceType::Spectator, p_external_manager2 );
	CHECK( true == bRet );
	p_external_manager2->QueryData( 1, eInstanceType::Displacement, query3 );
	CHECK( 26 == query3 );
    }
}
