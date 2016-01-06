//==================================
//
// Summary:      RenderMaterial
// Notes:
// Dependencies: N/A
//==================================
#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include "catch.hpp"

#include "RenderMaterial.h"

#include <map>
#include <vector>
#include <tuple>
#include <utility>
#include <set>
#include <iostream>
#include <string>

using namespace std;

TEST_CASE( "RenderMaterial", "[ALL]" ) {
    SECTION( "RenderMaterial" ){
        RenderMaterial render_material;
        vector<double> d_0 {0.0,0.1,0.2};
        vector<double> d_1 {0.3,0.4,0.5};
        vector<double> d_2 {0.6,0.7,0.8};
	vector<double> d_3 {1.0};
        render_material.AddDataSingle( RenderMaterialData::Ambient(),     d_0 );
        render_material.AddDataSingle( RenderMaterialData::Diffuse(),     d_1 );
        render_material.AddDataSingle( RenderMaterialData::Specular(),    d_2 );
	render_material.AddDataSingle( RenderMaterialData::Shininess(),   d_3 );

        bool bRet = render_material.CheckIsValid();
        CHECK( true == bRet );

        RenderMaterialData::Ambient::Type data_ambient;
        RenderMaterialData::Diffuse::Type data_diffuse;
        RenderMaterialData::Specular::Type data_specular;
        RenderMaterialData::Shininess::Type data_shininess;
        bRet = render_material.Compute( data_ambient, data_diffuse, data_specular, data_shininess );
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
}
