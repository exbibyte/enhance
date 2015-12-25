//==================================
//
// Summary:      InstanceManagerIter test case
// Notes:
// Dependencies: N/A
//==================================
#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include "catch.hpp"

#define NDEBUG //disable any assert in included files

#include "InstanceManagerIter.h"
#include "InstanceType.h"

#include <map>
#include <vector>
#include <tuple>
#include <utility>
#include <set>
#include <iostream>

using namespace std;

TEST_CASE( "InstanceManagerIter", "[ALL]" ) {
    SECTION( "InstanceManagerIter" ){

	InstanceManagerIter< eInstanceType > _manager_empty({});

	vector<double> vec_data_double;
	vec_data_double.push_back(55.5);
	vec_data_double.push_back(33.33);
	bool bRet = _manager_empty.SetLinkedAttributeLeafData( {}, 99, vec_data_double );
	CHECK( true == bRet );
	vector<double> query_vec_data_double;
	bRet = _manager_empty.QueryDataLeaf( 99, query_vec_data_double );
	CHECK( true == bRet );
	CHECK( query_vec_data_double.size() == 2 );
	CHECK( query_vec_data_double[0] == 55.5 );
	CHECK( query_vec_data_double[1] == 33.33 );
		
	InstanceManagerIter< eInstanceType > _manager2 ( { eInstanceType::PolyVerts } );
	_manager2.SetData( 1, eInstanceType::PolyVerts, 25 );
	InstanceManagerIter< eInstanceType > _manager3 ( { eInstanceType::Displacement } );
	_manager3.SetData( 1, eInstanceType::Displacement, 26 );
	vector<double> vec_data_double_2 { 88.8, 99.9 };
	_manager3.SetDataLeaf( 77, vec_data_double_2 );

	_manager3.LinkExternalInstanceManager( { { eInstanceType::Displacement, &_manager_empty } } );
	    
        InstanceManagerIter< eInstanceType > _manager( { eInstanceType::ObjectGeneric, eInstanceType::Player, eInstanceType::Spectator } );
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

	InstanceManagerIter< eInstanceType > * p_external_manager1, * p_external_manager2;
	bRet = _manager.GetExternalInstanceManager( eInstanceType::ObjectGeneric, p_external_manager1 );
	CHECK( true == bRet );
	p_external_manager1->QueryData( 1, eInstanceType::PolyVerts, query3 );
	CHECK( 25 == query3 );

	bRet = _manager.GetExternalInstanceManager( eInstanceType::Spectator, p_external_manager2 );
	CHECK( true == bRet );
	p_external_manager2->QueryData( 1, eInstanceType::Displacement, query3 );
	CHECK( 26 == query3 );

	//GetLinkedAttributes
	std::vector< eInstanceType > attributes;
	bRet = _manager.GetLinkedAttributes( attributes );
	CHECK( true == bRet );
	int size_attributes = attributes.size();
	REQUIRE( 5 == size_attributes );
	bRet = ( attributes ==  vector<eInstanceType>{ eInstanceType::ObjectGeneric, eInstanceType::PolyVerts, eInstanceType::Player, eInstanceType::Spectator, eInstanceType::Displacement } ? true : false );
	CHECK( true == bRet );

	//QueryLinkedAttributeVal
	std::vector<std::pair<unsigned int, eInstanceType> > attribute_keys { { 1, eInstanceType::Spectator }, { 1, eInstanceType::Displacement } };
        int query_val;
	bRet = _manager.QueryLinkedAttributeVal( attribute_keys, query_val );
	CHECK( true == bRet );
	CHECK( 26 == query_val );

	std::vector<std::pair<unsigned int, eInstanceType> > attribute_keys2 { { 1, eInstanceType::Spectator }, { 2, eInstanceType::Displacement } };
bRet = _manager.QueryLinkedAttributeVal( attribute_keys2, query_val );
	CHECK( false == bRet );

	std::vector<std::pair<unsigned int, eInstanceType> > attribute_keys3 { { 1, eInstanceType::Spectator }, { 1, eInstanceType::Player } };
bRet = _manager.QueryLinkedAttributeVal( attribute_keys3, query_val );
	CHECK( false == bRet );

	std::vector<std::pair<unsigned int, eInstanceType> > attribute_keys4 { { 1, eInstanceType::ObjectGeneric } };
	bRet = _manager.QueryLinkedAttributeVal( attribute_keys4, query_val );
	CHECK( true == bRet );
	CHECK( 5 == query_val );

	//QueryLinkedAttributeLeafData
	{
	    std::vector<std::pair<unsigned int, eInstanceType> > attribute_keys5 { { 1, eInstanceType::Spectator } };
	    int query_leaf_id = 77;
	    vector<double> query_leaf_data;
	    bRet = _manager.QueryLinkedAttributeLeafData( attribute_keys5, query_leaf_id, query_leaf_data );
	    CHECK( true == bRet );
	    int size_query_leaf_data = query_leaf_data.size();
	    CHECK( size_query_leaf_data == 2 );
	    double query_leaf_data_0 = query_leaf_data[0];
	    double query_leaf_data_1 = query_leaf_data[1];
	    CHECK( 88.8 == query_leaf_data_0 );
	    CHECK( 99.9 == query_leaf_data_1 );
	}

	{
	    std::vector<std::pair<unsigned int, eInstanceType> > attribute_keys5 { { 1, eInstanceType::Spectator }, { 1, eInstanceType::Displacement } };
	    int query_leaf_id = 99;
	    vector<double> query_leaf_data;
	    bRet = _manager.QueryLinkedAttributeLeafData( attribute_keys5, query_leaf_id, query_leaf_data );
	    CHECK( true == bRet );
	    int size_query_leaf_data = query_leaf_data.size();
	    CHECK( size_query_leaf_data == 2 );
	    double query_leaf_data_0 = query_leaf_data[0];
	    double query_leaf_data_1 = query_leaf_data[1];
	    CHECK( 55.5 == query_leaf_data_0 );
	    CHECK( 33.33 == query_leaf_data_1 );
	}

	//InstanceManagerIter
	{
	    std::vector<std::pair<unsigned int, eInstanceType> > attribute_keys5 { { 1, eInstanceType::Spectator }, { 1, eInstanceType::Displacement } };
	    InstanceManagerIter< eInstanceType > * p_get_manager;
	    bRet = _manager.GetLinkedAttributeManager( attribute_keys5, p_get_manager );
	    CHECK( true == bRet );
	    bRet = ( p_get_manager == &_manager_empty ? true : false );
	    CHECK( true == bRet );
	}
	{
	    std::vector<std::pair<unsigned int, eInstanceType> > attribute_keys5 { { 1, eInstanceType::Spectator } };
	    InstanceManagerIter< eInstanceType > * p_get_manager;
	    bRet = _manager.GetLinkedAttributeManager( attribute_keys5, p_get_manager );
	    CHECK( true == bRet );
	    bRet = ( p_get_manager == &_manager_empty ? true : false );
	    CHECK( false == bRet );
	}
    }
}
