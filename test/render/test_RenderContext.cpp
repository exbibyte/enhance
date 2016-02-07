//==================================
//
// Summary:      RenderContext
// Notes:
// Dependencies: N/A
//==================================
#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include "catch.hpp"

#include "RenderContext.h"

#include <map>
#include <vector>
#include <tuple>
#include <utility>
#include <set>
#include <iostream>
#include <string>

using namespace std;

TEST_CASE( "RenderContext", "[ALL]" ) {
    SECTION( "RenderContext" ){
        RenderContext render_poly;
        vector<int> i_windowsize { 500, 450 };
        vector<int> i_texturesize_shadowmap {2500, 3000 };
	string str_title =  "Un-named";
        render_poly.AddDataSingle( RenderContextData::WindowSize(),           i_windowsize );
        render_poly.AddDataSingle( RenderContextData::TextureSizeShadowMap(), i_texturesize_shadowmap );
	render_poly.AddDataSingle( RenderContextData::Title(),                str_title );

        bool bRet = render_poly.CheckIsValid();
        CHECK( true == bRet );

        RenderContextData::WindowSize::Type data_windowsize;
	RenderContextData::TextureSizeShadowMap::Type data_texturesize_shadowmap;
	RenderContextData::Title::Type data_title;
        bRet = render_poly.Compute( data_windowsize, data_texturesize_shadowmap, data_title );
        CHECK( true == bRet );

        int iSizeWindowSize;
	int iSizeTextureSizeShadowMap;
	int iSizeTitle;
        iSizeWindowSize = data_windowsize.size();
	iSizeTextureSizeShadowMap = data_texturesize_shadowmap.size();
	iSizeTitle = data_title.size();
        CHECK( RenderContextData::WindowSize::Size::value == iSizeWindowSize );
	CHECK( RenderContextData::TextureSizeShadowMap::Size::value == iSizeTextureSizeShadowMap );
	CHECK( RenderContextData::Title::Size::value <= iSizeTitle );

        double query_coord_0 = data_windowsize[0];
        double query_coord_1 = data_windowsize[1];
        double query_texturesize_shadowmap_0 = data_texturesize_shadowmap[0];
        double query_texturesize_shadowmap_1 = data_texturesize_shadowmap[1];
	string query_title = data_title;
        CHECK( 500 == query_coord_0 );
        CHECK( 450 == query_coord_1 );
        CHECK( 2500 == query_texturesize_shadowmap_0 );
        CHECK( 3000 == query_texturesize_shadowmap_1 );
	CHECK( "Un-named" == query_title );
    }
}
