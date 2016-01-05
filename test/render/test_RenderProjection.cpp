//==================================
//
// Summary:      Batcher
// Notes:
// Dependencies: N/A
//==================================
#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include "catch.hpp"

#include "RenderProjection.h"

#include <map>
#include <vector>
#include <tuple>
#include <utility>
#include <set>
#include <iostream>
#include <string>

using namespace std;

TEST_CASE( "RenderProjection", "[ALL]" ) {
    SECTION( "RenderProjection" ){
	RenderProjection render_proj;
	vector<double> d_0 {0.0,1.1,2.2};
	vector<double> d_1 {3.3,4.4,5.5};
	vector<double> d_2 {6.6,7.7,8.8};
	vector<double> d_3 {9.9,10.10,11.11,12.12};
	vector<double> d_4 {0.3,0.4,0.5};
	vector<double> d_5 {0.0,0.1,0.2};
	vector<double> d_6 {0.6,0.7,0.8};
	render_proj.AddDataSingle( RenderProjectionData::Coordinate(),  d_0 );
	render_proj.AddDataSingle( RenderProjectionData::Lookat(),      d_1 );
	render_proj.AddDataSingle( RenderProjectionData::Up(),          d_2 );
	render_proj.AddDataSingle( RenderProjectionData::Perspective(), d_3 );
	render_proj.AddDataSingle( RenderProjectionData::Ambient(),     d_4 );
	render_proj.AddDataSingle( RenderProjectionData::Diffuse(),     d_5 );
	render_proj.AddDataSingle( RenderProjectionData::Specular(),    d_6 );

	bool bRet = render_proj.CheckIsValid();
	CHECK( true == bRet );

	RenderProjectionData::Perspective::Type data_ambient;
	RenderProjectionData::Diffuse::Type data_diffuse;
	RenderProjectionData::Specular::Type data_specular;
	RenderProjectionData::Coordinate::Type data_coordinate;
	mat4 projectionMatrix;
	mat4 viewMatrix;
	bRet = render_proj.Compute( data_ambient, data_diffuse, data_specular, data_coordinate, projectionMatrix, viewMatrix );
	CHECK( true == bRet );

	int iSizeAmbient;
	int iSizeDiffuse;
	int iSizeSpecular;
	int iSizeCoordinate;
	iSizeAmbient = data_ambient.size();
	iSizeDiffuse = data_diffuse.size();
	iSizeSpecular = data_specular.size();
	iSizeCoordinate = data_coordinate.size();
	CHECK( 3 == iSizeAmbient );
	CHECK( 3 == iSizeDiffuse );
	CHECK( 3 == iSizeSpecular );
	CHECK( 3 == iSizeCoordinate );

	double query_coord_0 = data_coordinate[0];
	double query_coord_1 = data_coordinate[1];
	double query_coord_2 = data_coordinate[2];
	double query_ambient_0 = data_ambient[0];
	double query_ambient_1 = data_ambient[1];
	double query_ambient_2 = data_ambient[2];
	double query_diffuse_0 = data_diffuse[0];
	double query_diffuse_1 = data_diffuse[1];
	double query_diffuse_2 = data_diffuse[2];
	double query_specular_0 = data_specular[0];
	double query_specular_1 = data_specular[1];
	double query_specular_2 = data_specular[2];
	CHECK( 0.0 == query_coord_0 );
	CHECK( 1.1 == query_coord_1 );
	CHECK( 2.2 == query_coord_2 );
	CHECK( 0.3 == query_ambient_0 );
	CHECK( 0.4 == query_ambient_1 );
	CHECK( 0.5 == query_ambient_2 );
	CHECK( 0.0 == query_diffuse_0 );
	CHECK( 0.1 == query_diffuse_1 );
	CHECK( 0.2 == query_diffuse_2 );
	CHECK( 0.6 == query_specular_0 );
	CHECK( 0.7 == query_specular_1 );
	CHECK( 0.8 == query_specular_2 );
    }
}
