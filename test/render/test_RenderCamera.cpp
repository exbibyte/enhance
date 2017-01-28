//==================================
//
// Summary:      Batcher
// Notes:
// Dependencies: N/A
//==================================
#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include "catch.hpp"

#include "RenderCamera.h"
#include "Mat.h"

#include <map>
#include <vector>
#include <tuple>
#include <utility>
#include <set>
#include <iostream>
#include <string>

using namespace std;

TEST_CASE( "RenderCamera", "[ALL]" ) {
    SECTION( "RenderCamera" ){
        RenderCamera render_light;
        vector<double> d_0 {0.0,1.1,2.2};
        vector<double> d_1 {3.3,4.4,5.5};
        vector<double> d_2 {6.6,7.7,8.8};
        vector<double> d_3 {9.9,10.10,11.11,12.12};
        vector<double> d_4 {0.3,0.4,0.5};
        vector<double> d_5 {0.0,0.1,0.2};
        vector<double> d_6 {0.6,0.7,0.8};
        render_light.AddDataSingle( RenderCameraData::Coordinate(),  d_0 );
        render_light.AddDataSingle( RenderCameraData::Lookat(),      d_1 );
        render_light.AddDataSingle( RenderCameraData::Up(),          d_2 );
        render_light.AddDataSingle( RenderCameraData::Perspective(), d_3 );
        render_light.AddDataSingle( RenderCameraData::Ambient(),     d_4 );
        render_light.AddDataSingle( RenderCameraData::Diffuse(),     d_5 );
        render_light.AddDataSingle( RenderCameraData::Specular(),    d_6 );

        bool bRet = render_light.CheckIsValid();
        CHECK( true == bRet );

        RenderCameraData::Ambient::Type data_ambient;
        RenderCameraData::Diffuse::Type data_diffuse;
        RenderCameraData::Specular::Type data_specular;
        RenderCameraData::Coordinate::Type data_coordinate;
        Mat projectionMatrix;
        Mat viewMatrix;
        bRet = render_light.Compute( data_ambient, data_diffuse, data_specular, data_coordinate, projectionMatrix, viewMatrix );
        CHECK( true == bRet );

        int iSizeAmbient;
        int iSizeDiffuse;
        int iSizeSpecular;
        int iSizeCoordinate;
        iSizeAmbient = data_ambient.size();
        iSizeDiffuse = data_diffuse.size();
        iSizeSpecular = data_specular.size();
        iSizeCoordinate = data_coordinate.size();
        CHECK( RenderCameraData::Ambient::Size::value == iSizeAmbient );
        CHECK( RenderCameraData::Diffuse::Size::value == iSizeDiffuse );
        CHECK( RenderCameraData::Specular::Size::value == iSizeSpecular );
        CHECK( RenderCameraData::Coordinate::Size::value == iSizeCoordinate );

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
