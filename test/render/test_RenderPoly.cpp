//==================================
//
// Summary:      RenderPoly
// Notes:
// Dependencies: N/A
//==================================
#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include "catch.hpp"

#include "RenderPoly.h"
#include "Mat.h"

#include <map>
#include <vector>
#include <tuple>
#include <utility>
#include <set>
#include <iostream>
#include <string>

using namespace std;

TEST_CASE( "RenderPoly", "[ALL]" ) {
    SECTION( "RenderPoly" ){
        RenderPoly render_poly;
        vector<double> d_translate    {5,10,2};
        vector<double> d_rotate_axis  {1,0,0};
	vector<double> d_rotate_angle {45};
        render_poly.AddDataSingle( RenderPolyData::Coordinate(),    d_translate );
        render_poly.AddDataSingle( RenderPolyData::RotationAxis(),  d_rotate_axis );
	render_poly.AddDataSingle( RenderPolyData::RotationAngle(), d_rotate_angle );

        bool bRet = render_poly.CheckIsValid();
        CHECK( true == bRet );

        RenderPolyData::Coordinate::Type data_coordinate;
	RenderPolyData::RotationAxis::Type data_rotation_axis;
	RenderPolyData::RotationAngle::Type data_rotation_angle;
        Mat orientation_matrix;
        bRet = render_poly.Compute( data_coordinate, data_rotation_axis, data_rotation_angle, orientation_matrix );
        CHECK( true == bRet );

        int iSizeCoordinate;
	int iSizeRotationAxis;
	int iSizeRotationAngle;
        iSizeCoordinate = data_coordinate.size();
	iSizeRotationAxis = data_rotation_axis.size();
	iSizeRotationAngle = data_rotation_angle.size();
        CHECK( RenderPolyData::Coordinate::Size::value == iSizeCoordinate );
	CHECK( RenderPolyData::RotationAxis::Size::value == iSizeRotationAxis );
	CHECK( RenderPolyData::RotationAngle::Size::value == iSizeRotationAngle );

        double query_coord_0 = data_coordinate[0];
        double query_coord_1 = data_coordinate[1];
        double query_coord_2 = data_coordinate[2];
        double query_rotation_axis_0 = data_rotation_axis[0];
        double query_rotation_axis_1 = data_rotation_axis[1];
        double query_rotation_axis_2 = data_rotation_axis[2];
	double query_rotation_angle = data_rotation_angle[0];
        CHECK( 5.0 == query_coord_0 );
        CHECK( 10.0 == query_coord_1 );
        CHECK( 2 == query_coord_2 );
        CHECK( 1.0 == query_rotation_axis_0 );
        CHECK( 0.0 == query_rotation_axis_1 );
        CHECK( 0.0 == query_rotation_axis_2 );
	CHECK( 45.0 == query_rotation_angle );

	float orientation_array[16];
	size_t size_actual;
	bool ret = orientation_matrix.GetArray( orientation_array, 16, size_actual );
	CHECK( ret );
	CHECK( 16 == size_actual );
	for( int i = 0; i < 16; ++i ){
	    cout << orientation_array[i] << " ";
	}

	CHECK( orientation_array[12] == query_coord_0 );
        CHECK( orientation_array[13] == query_coord_1 );
        CHECK( orientation_array[14] == query_coord_2 );
    }
}
