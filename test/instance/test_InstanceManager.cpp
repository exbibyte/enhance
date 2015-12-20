//==================================
//
// Summary:      Winged Edge Facilities Test
// Notes:
// Dependencies: N/A
//==================================
#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include "catch.hpp"

#define NDEBUG //disable any assert in included files

#include "InstanceManager.h"
#include "InstanceType.h"

#include <map>
#include <vector>
#include <tuple>
#include <utility>
#include <set>
#include <iostream>

using namespace std;

TEST_CASE( "InstanceManager", "[ALL]" ) {
    SECTION( "InstanceManager Complex Types" ){
        InstanceManager< InstanceType::ObjectGeneric, InstanceType::Player, InstanceType::Spectator > _manager_generic;
	int val_set1 = 5;
	int val_set2 = 10;
	int val_set3 = 15;
	bool bRet = _manager_generic.SetValue<InstanceType::ObjectGeneric>( 1, val_set1 );
	bRet &= _manager_generic.SetValue<InstanceType::Player>( 1, val_set2 );
	bRet &= _manager_generic.SetValue<InstanceType::Spectator>( 1, val_set3 );
	int val_query1;
	int val_query2;
	int val_query3;
	bRet &=  _manager_generic.QueryValue<InstanceType::ObjectGeneric>( 1, val_query1 );
	bRet &=  _manager_generic.QueryValue<InstanceType::Player>( 1, val_query2 );
	bRet &=  _manager_generic.QueryValue<InstanceType::Spectator>( 1, val_query3 );
	CHECK( true == bRet );
	CHECK( 5 == val_query1 );
	CHECK( 10 == val_query2 );
	CHECK( 15 == val_query3 );

	bRet =  _manager_generic.QueryValue<InstanceType::Spectator>( 2, val_query3 );
	CHECK( false == bRet );

	int val_set4 = 20;
	int val_query4;
	bRet = _manager_generic.SetValue<InstanceType::Spectator>( 1, val_set4 );
	bRet &= _manager_generic.QueryValue<InstanceType::Spectator>( 1, val_query4 );
	CHECK( true == bRet );
	CHECK( 20 == val_query4 );
    }
    SECTION( "InstanceManager Basic Types" ){
        InstanceManager< int > _manager_int;
	int val_set1 = 5;
	int val_set2 = 10;
	bool bRet = _manager_int.SetValue( 1, val_set1 );
	bRet &= _manager_int.SetValue( 2, val_set2 );
	int val_query1;
	int val_query2;
	bRet &=  _manager_int.QueryValue( 1, val_query1 );
	bRet &=  _manager_int.QueryValue( 2, val_query2 );
	CHECK( true == bRet );
	CHECK( 5 == val_query1 );
	CHECK( 10 == val_query2 );

	bRet = _manager_int.SetValue( 1, val_set2 );
	bRet &=  _manager_int.QueryValue( 1, val_query1 );
	CHECK( true == bRet );
	CHECK( 10 == val_query1 );
    }
    SECTION( "InstanceManager Basic Vectors" ){
        InstanceManager< vector<double> > _manager_vec_double;
	vector<double> set_vec_1 = std::initializer_list<double>{ 6.6, 7.7, 8.8 };
	vector<double> set_vec_2 = std::initializer_list<double>{ 1.1, 2.2, 3.3 };
	bool bRet = _manager_vec_double.SetValue( 1, set_vec_1 );
	bRet &= _manager_vec_double.SetValue( 2, set_vec_2 );

        vector<double> query_vec_1;
	vector<double> query_vec_2;
	bRet &=  _manager_vec_double.QueryValue( 1, query_vec_1 );
	bRet &=  _manager_vec_double.QueryValue( 2, query_vec_2 );

	CHECK( true == bRet );
	double i = query_vec_1[0];
	CHECK( 6.6 == i );
	i = query_vec_1[1];
	CHECK( 7.7 == i );
	i = query_vec_1[2];
	CHECK( 8.8 == i );

	i = query_vec_2[0];
	CHECK( 1.1 == i );
	i = query_vec_2[1];
	CHECK( 2.2 == i );
	i = query_vec_2[2];
	CHECK( 3.3 == i );
    }
}
