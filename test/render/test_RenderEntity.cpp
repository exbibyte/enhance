//==================================
//
// Summary:      RenderEntity
// Notes:
// Dependencies: N/A
//==================================
#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include "catch.hpp"

#include "RenderEntity.h"
#include "Mat.h"

#include <map>
#include <vector>
#include <tuple>
#include <utility>
#include <set>
#include <iostream>
#include <string>

using namespace std;

TEST_CASE( "RenderEntity", "[ALL]" ) {
    SECTION( "RenderEntity" ){
	bool bRet;

	RenderEntity render_entity;

	//set poly data
	vector<double> d_translate    {5,10,2};
        vector<double> d_rotate_axis  {1,0,0};
	vector<double> d_rotate_angle {45};
	render_entity.AddDataSingle( RenderPolyData::Coordinate(),    d_translate );
        render_entity.AddDataSingle( RenderPolyData::RotationAxis(),  d_rotate_axis );
	render_entity.AddDataSingle( RenderPolyData::RotationAngle(), d_rotate_angle );

	//set material data
        vector<double> d_0 {0.0,0.1,0.2};
        vector<double> d_1 {0.3,0.4,0.5};
        vector<double> d_2 {0.6,0.7,0.8};
	vector<double> d_3 {1.0};
        render_entity.AddDataSingle( RenderMaterialData::Ambient(),     d_0 );
        render_entity.AddDataSingle( RenderMaterialData::Diffuse(),     d_1 );
        render_entity.AddDataSingle( RenderMaterialData::Specular(),    d_2 );
	render_entity.AddDataSingle( RenderMaterialData::Shininess(),   d_3 );

	//set vertex data
	vector<double> d_normals { 0, 1, 2, 3, 4, 5 };
	vector<double> d_vertices { 6.1, 6.2, 6.3, 6.4, 6.5, 6.6 };
	render_entity.AddDataSingle( RenderVertexData::Normals(),  d_normals );
	render_entity.AddDataSingle( RenderVertexData::Vertices(), d_vertices );

        bRet = render_entity.CheckIsValid();
        CHECK( true == bRet );

	//check poly data
	{
	    RenderPolyData::Coordinate::Type data_coordinate;
	    RenderPolyData::RotationAxis::Type data_rotation_axis;
	    RenderPolyData::RotationAngle::Type data_rotation_angle;
	    Mat orientation_matrix;
	    bRet = render_entity.Compute( RenderPolyData(), data_coordinate, data_rotation_axis, data_rotation_angle, orientation_matrix );
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

	    cout << "Orientation Array Retrieved: ";
	    float orientation_array[16];
	    size_t size_actual;
	    bool ret = orientation_matrix.GetArray( orientation_array, 16, size_actual );
	    CHECK( ret );
	    CHECK( 16 == size_actual );
	    for( int i = 0; i < 16; ++i ){
	    	cout << orientation_array[i] << " ";
	    }
	    cout << endl;

	    CHECK( orientation_array[12] == query_coord_0 );
	    CHECK( orientation_array[13] == query_coord_1 );
	    CHECK( orientation_array[14] == query_coord_2 );
	}
	//check material data
	{
	    RenderMaterialData::Ambient::Type data_ambient;
	    RenderMaterialData::Diffuse::Type data_diffuse;
	    RenderMaterialData::Specular::Type data_specular;
	    RenderMaterialData::Shininess::Type data_shininess;
	    bRet = render_entity.Compute( RenderMaterialData(), data_ambient, data_diffuse, data_specular, data_shininess );
	    CHECK( true == bRet );

	    int iSizeAmbient;
	    int iSizeDiffuse;
	    int iSizeSpecular;
	    int iSizeShininess;
	    iSizeAmbient = data_ambient.size();
	    iSizeDiffuse = data_diffuse.size();
	    iSizeSpecular = data_specular.size();
	    iSizeShininess = data_shininess.size();
	    CHECK( RenderMaterialData::Ambient::Size::value == iSizeAmbient );
	    CHECK( RenderMaterialData::Diffuse::Size::value == iSizeDiffuse );
	    CHECK( RenderMaterialData::Specular::Size::value == iSizeSpecular );
	    CHECK( RenderMaterialData::Shininess::Size::value == iSizeShininess );

	    double query_ambient_0 = data_ambient[0];
	    double query_ambient_1 = data_ambient[1];
	    double query_ambient_2 = data_ambient[2];
	    double query_diffuse_0 = data_diffuse[0];
	    double query_diffuse_1 = data_diffuse[1];
	    double query_diffuse_2 = data_diffuse[2];
	    double query_specular_0 = data_specular[0];
	    double query_specular_1 = data_specular[1];
	    double query_specular_2 = data_specular[2];
	    double query_shininess_0 = data_shininess[0];
	    CHECK( 0.0 == query_ambient_0 );
	    CHECK( 0.1 == query_ambient_1 );
	    CHECK( 0.2 == query_ambient_2 );
	    CHECK( 0.3 == query_diffuse_0 );
	    CHECK( 0.4 == query_diffuse_1 );
	    CHECK( 0.5 == query_diffuse_2 );
	    CHECK( 0.6 == query_specular_0 );
	    CHECK( 0.7 == query_specular_1 );
	    CHECK( 0.8 == query_specular_2 );
	    CHECK( 1.0 == query_shininess_0 );
	}

	//check vertex data
	{
	    RenderVertexData::Normals::Type data_normals;
	    RenderVertexData::Vertices::Type data_vertices;
	    bRet = render_entity.Compute( RenderVertexData(), data_vertices, data_normals );
	    CHECK( true == bRet );
	    int iSizeVertices;
	    int iSizeNormals;
	    iSizeVertices = data_vertices.size();
	    iSizeNormals = data_normals.size();
	    CHECK( 6 == iSizeVertices );
	    CHECK( 6 == iSizeNormals );
	}
    }
}
