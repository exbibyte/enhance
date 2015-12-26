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

TEST_CASE( "enInstanceManagerIterPackage", "[ALL]" ) {
    SECTION( "enInstanceManagerIterPackage" ){

	enInstanceManagerIterPackage _package_instance_managers;
	_package_instance_managers.CreateManager( eInstanceType::PolyVertices, {} );
	_package_instance_managers.CreateManager( eInstanceType::LightAmbient, {} );
	_package_instance_managers.CreateManager( eInstanceType::LightSpectral, {} );
	_package_instance_managers.CreateManager( eInstanceType::LightDiffuse, {} );
	_package_instance_managers.CreateManager( eInstanceType::CameraProjection, {} );
	_package_instance_managers.CreateManager( eInstanceType::MaterialAmbient, {} );
	_package_instance_managers.CreateManager( eInstanceType::MaterialDiffuse, {} );
	_package_instance_managers.CreateManager( eInstanceType::MaterialSpectral, {} );
	_package_instance_managers.CreateManager( eInstanceType::MaterialShininess, {} );
	_package_instance_managers.CreateManager( eInstanceType::OrientOffset, {} );
	_package_instance_managers.CreateManager( eInstanceType::OrientRotation, {} );
	_package_instance_managers.CreateManager( eInstanceType::LightComposition, { eInstanceType::LightAmbient, eInstanceType::LightSpectral, eInstanceType::LightDiffuse } );
	_package_instance_managers.CreateManager( eInstanceType::MaterialComposition, { eInstanceType::MaterialAmbient, eInstanceType::MaterialDiffuse, eInstanceType::MaterialSpectral, eInstanceType::MaterialShininess } );
	_package_instance_managers.CreateManager( eInstanceType::EntityOrientation, { eInstanceType::OrientOffset, eInstanceType::OrientRotation } );
	_package_instance_managers.CreateManager( eInstanceType::EntityPackage, { eInstanceType::PolyVertices, eInstanceType::EntityOrientation, eInstanceType::MaterialComposition  } );
	_package_instance_managers.LinkManagers();

	enInstanceManagerIter * entity_package_manager;
	bool bRet = _package_instance_managers.GetManager( eInstanceType::EntityPackage, entity_package_manager );
	CHECK( bRet );

	bRet = entity_package_manager->SetData( 99, eInstanceType::EntityOrientation, 100 );
	CHECK( bRet );
	
	enInstanceManagerIter * entity_orientation_manager;
	bRet = _package_instance_managers.GetManager( eInstanceType::EntityOrientation, entity_orientation_manager );
	CHECK( bRet );

	bRet = entity_orientation_manager->SetData( 100, eInstanceType::OrientOffset, 101 );
	CHECK( bRet );

	enInstanceManagerIter * orient_offset_manager;
	bRet = _package_instance_managers.GetManager( eInstanceType::OrientOffset, orient_offset_manager );
	CHECK( bRet );

	vector<double> vec_data_double { 33.33, 55.55, 77.0 };
	bRet = orient_offset_manager->SetDataLeaf( 101, vec_data_double );
	CHECK( bRet );

	vector<double> vec_data_double_query;
	bRet = entity_package_manager->QueryLinkedAttributeLeafData( 99, { eInstanceType::EntityOrientation, eInstanceType::OrientOffset }, vec_data_double_query );
	CHECK( bRet );
	
	bRet = vec_data_double_query == vec_data_double ? true : false;
	CHECK( bRet );
    }
}
