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
	bool bRet = _manager_empty.SetLinkedAttributeLeafData( 99, {}, vec_data_double );
	CHECK( true == bRet );
	vector<double> query_vec_data_double;
	bRet = _manager_empty.QueryDataLeaf( 99, query_vec_data_double );
	CHECK( true == bRet );
	CHECK( query_vec_data_double.size() == 2 );
	CHECK( query_vec_data_double[0] == 55.5 );
	CHECK( query_vec_data_double[1] == 33.33 );

	bRet = _manager_empty.SetLinkedAttributeLeafData( 53, {}, vec_data_double );
		
	InstanceManagerIter< eInstanceType > _manager2 ( { eInstanceType::PolyVerts } );
	_manager2.SetData( 1, eInstanceType::PolyVerts, 25 );
	InstanceManagerIter< eInstanceType > _manager3 ( { eInstanceType::Displacement } );
	_manager3.SetData( 1, eInstanceType::Displacement, 26 );
	_manager3.SetData( 10, eInstanceType::Displacement, 53 );
	vector<double> vec_data_double_2 { 88.8, 99.9 };
	_manager3.SetDataLeaf( 77, vec_data_double_2 );
	_manager3.SetDataLeaf( 10, vec_data_double_2 );

	_manager3.LinkExternalInstanceManager( { { eInstanceType::Displacement, &_manager_empty } } );
	    
        InstanceManagerIter< eInstanceType > _manager( { eInstanceType::ObjectGeneric, eInstanceType::Player, eInstanceType::Spectator } );
        bRet = _manager.SetData( 1, eInstanceType::ObjectGeneric, 5 );
	bRet &= _manager.SetData( 1, eInstanceType::Spectator, 10 );
	CHECK( true == bRet );
	
	unsigned int query1, query2, query3;
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
	std::vector< eInstanceType > attribute_keys { eInstanceType::Spectator, eInstanceType::Displacement };
        unsigned int query_val;
	bRet = _manager.QueryLinkedAttributeVal( 1, attribute_keys, query_val );
	CHECK( true == bRet );
	CHECK( 53 == query_val );

	std::vector< eInstanceType > attribute_keys2 { eInstanceType::Spectator, eInstanceType::Displacement };
	bRet = _manager.QueryLinkedAttributeVal( 2, attribute_keys2, query_val );
	CHECK( false == bRet );

	std::vector< eInstanceType > attribute_keys3 { eInstanceType::Spectator, eInstanceType::Player };
	bRet = _manager.QueryLinkedAttributeVal( 1, attribute_keys3, query_val );
	CHECK( false == bRet );

	std::vector< eInstanceType > attribute_keys4 { eInstanceType::ObjectGeneric };
	bRet = _manager.QueryLinkedAttributeVal( 1, attribute_keys4, query_val );
	CHECK( true == bRet );
	CHECK( 5 == query_val );

	//QueryLinkedAttributeLeafData
	{
	    std::vector< eInstanceType > attribute_keys5 { eInstanceType::Spectator };
	    vector<double> query_leaf_data;
	    bRet = _manager.QueryLinkedAttributeLeafData( 1, attribute_keys5, query_leaf_data );
	    CHECK( true == bRet );
	    int size_query_leaf_data = query_leaf_data.size();
	    CHECK( size_query_leaf_data == 2 );
	    double query_leaf_data_0 = query_leaf_data[0];
	    double query_leaf_data_1 = query_leaf_data[1];
	    CHECK( 88.8 == query_leaf_data_0 );
	    CHECK( 99.9 == query_leaf_data_1 );
	}

	{
	    std::vector< eInstanceType > attribute_keys5 { eInstanceType::Spectator, eInstanceType::Displacement };
	    vector<double> query_leaf_data;
	    bRet = _manager.QueryLinkedAttributeLeafData( 1, attribute_keys5, query_leaf_data );
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
	    std::vector< eInstanceType > attribute_keys5 { eInstanceType::Spectator, eInstanceType::Displacement };
	    InstanceManagerIter< eInstanceType > * p_get_manager;
	    bRet = _manager.GetLinkedAttributeManager( attribute_keys5, p_get_manager );
	    CHECK( true == bRet );
	    bRet = ( p_get_manager == &_manager_empty ? true : false );
	    CHECK( true == bRet );
	}
	{
	    std::vector< eInstanceType > attribute_keys5 { eInstanceType::Spectator };
	    InstanceManagerIter< eInstanceType > * p_get_manager;
	    bRet = _manager.GetLinkedAttributeManager( attribute_keys5, p_get_manager );
	    CHECK( true == bRet );
	    bRet = ( p_get_manager == &_manager_empty ? true : false );
	    CHECK( false == bRet );
	}

	//SetLinkedAttributeLeafData
	{
	    std::vector< eInstanceType > attribute_keys5 { eInstanceType::Spectator, eInstanceType::Displacement };
	    vector<double> set_leaf_data { 3.3, 4.4 };
	    bRet = _manager.SetLinkedAttributeLeafData( 1, attribute_keys5, set_leaf_data );
	    CHECK( true == bRet );
	    vector<double> query_leaf_data;
	    bRet = _manager.QueryLinkedAttributeLeafData( 1, attribute_keys5, query_leaf_data );
	    int size_query_leaf_data = query_leaf_data.size();
	    CHECK( size_query_leaf_data == 2 );
	    double query_leaf_data_0 = query_leaf_data[0];
	    double query_leaf_data_1 = query_leaf_data[1];
	    CHECK( 3.3 == query_leaf_data_0 );
	    CHECK( 4.4 == query_leaf_data_1 );
	}

	//SetLinkedAttributeLeafData with bCreateIfNotExist on, QueryLinkedAttributeValMultiple
	{
	    bool bCreateIfNotExist = true;
	    std::vector< eInstanceType > attribute_keys5 { eInstanceType::Spectator, eInstanceType::Displacement };
	    vector<double> set_leaf_data { 1234.5, 763.5, 252.76 };
	    bRet = _manager.SetLinkedAttributeLeafData( 8, attribute_keys5, set_leaf_data, bCreateIfNotExist );
	    CHECK( true == bRet );
	    vector<double> query_leaf_data;
	    bRet = _manager.QueryLinkedAttributeLeafData( 8, attribute_keys5, query_leaf_data );
	    int size_query_leaf_data = query_leaf_data.size();
	    CHECK( size_query_leaf_data == 3 );
	    double query_leaf_data_0 = query_leaf_data[0];
	    double query_leaf_data_1 = query_leaf_data[1];
	    double query_leaf_data_2 = query_leaf_data[2];
	    CHECK( 1234.5 == query_leaf_data_0 );
	    CHECK( 763.5 == query_leaf_data_1 );
	    CHECK( 252.76 == query_leaf_data_2 );

	    std::vector< std::pair< eInstanceType, unsigned int > > query_val;
	    bRet = _manager.QueryLinkedAttributeValMultiple( 8, attribute_keys5, query_val );
	    CHECK( bRet );
	    int size_query_attribute_multiple = query_val.size();
	    CHECK( 2 == size_query_attribute_multiple );
	    unsigned int attrib_val_1 = query_val[0].second;
	    unsigned int attrib_val_2 = query_val[1].second;
	    CHECK( 0 == attrib_val_1 );
	    CHECK( 0 == attrib_val_2 );
	}
	
	//SetLinkedAttributeVal
	{

	    std::vector< eInstanceType > attribute_keys { eInstanceType::Spectator, eInstanceType::Displacement };
	    unsigned int query_val;
	    unsigned int set_val = 90;
	    bRet = _manager.SetLinkedAttributeVal( 1, attribute_keys, set_val );
	    CHECK( true == bRet );
	    bRet = _manager.QueryLinkedAttributeVal( 1, attribute_keys, query_val );
	    CHECK( true == bRet );
	    CHECK( 90 == query_val );
	}

	//SetLinkedAttributeVal with attributes_default
	{

	    std::vector< eInstanceType > attribute_keys { eInstanceType::Spectator, eInstanceType::Displacement };
	    unsigned int query_val;
	    unsigned int set_val = 90;
	    bRet = _manager.SetLinkedAttributeVal( 99, attribute_keys, set_val, { 11 } );
	    CHECK( true == bRet );
	    bRet = _manager.QueryLinkedAttributeVal( 99, attribute_keys, query_val );
	    CHECK( true == bRet );
	    CHECK( 90 == query_val );

	    std::vector< std::pair< eInstanceType, unsigned int > > query_attrib_val;
	    bRet = _manager.QueryLinkedAttributeValMultiple( 99, attribute_keys, query_attrib_val );
	    CHECK( bRet );
	    int size_query_attribute_multiple = query_attrib_val.size();
	    CHECK( 2 == size_query_attribute_multiple );
	    unsigned int attrib_val_1 = query_attrib_val[0].second;
	    unsigned int attrib_val_2 = query_attrib_val[1].second;
	    CHECK( 11 == attrib_val_1 );
	    CHECK( 90 == attrib_val_2 );
	}
    }
}
