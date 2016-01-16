//==================================
//
// Summary:      RenderVertex
// Notes:
// Dependencies: N/A
//==================================
#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include "catch.hpp"

#include "RenderVertex.h"

#include <map>
#include <vector>
#include <tuple>
#include <utility>
#include <set>
#include <iostream>
#include <string>

using namespace std;

TEST_CASE( "RenderVertex", "[ALL]" ) {
    SECTION( "RenderVertex" ){
        RenderVertex render_vertex;
        vector<double> d_vertices  { 0.0, 1.0, 2.0, 3.0, 4.0, 5.0 };
	vector<double> d_normals   { 0.5, 1.5, 2.5 };
	vector<double> d_normals_2 { 0.5, 1.5, 2.5, 3.5, 4.5, 5.5 };

        render_vertex.AddDataSingle( RenderVertexData::Vertices(), d_vertices );
        render_vertex.AddDataSingle( RenderVertexData::Normals(),  d_normals );
	render_vertex.AddDataSingle( RenderVertexData::Normals(),  d_normals_2 );
        bool bRet = render_vertex.CheckIsValid();
        CHECK( true == bRet );

        RenderVertexData::Vertices::Type data_vertices;
	RenderVertexData::Normals::Type data_normals;
        bRet = render_vertex.Compute( data_vertices, data_normals );
        CHECK( true == bRet );

        int iSizeVertices;
	int iSizeNormals;
	iSizeVertices = data_vertices.size();
	iSizeNormals = data_normals.size();
        CHECK( 6 == iSizeVertices );
	CHECK( 6 == iSizeNormals );
    }
}
