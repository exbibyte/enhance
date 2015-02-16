#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include "catch.hpp"
#include "Vec.h"
#include <math.h>
#include "PolyMesh.h"
#include <vector>
#include <set>
#include <iostream>
using namespace std;

TEST_CASE( "Basic", "[B]" ) {

    vector< Vec > vVertex;
    vector< set< int > > vConnect;

    for( int i = 0; i < 5; i++ ){ 
	Vec a = Vec(3);
	a._vec[0] = 5;
	a._vec[1] = 5;
	a._vec[2] = 5;
	vVertex.push_back( a );
    }

    int conn0[] = {1,2};
    int conn1[] = {0,2};
    int conn2[] = {0,1,3,4};
    int conn3[] = {2,4};
    int conn4[] = {3,2};
    set<int> con0 (conn0, conn0+2);
    set<int> con1 (conn1, conn1+2);
    set<int> con2 (conn2, conn2+4);
    set<int> con3 (conn3, conn3+2);
    set<int> con4 (conn4, conn4+2);

    vConnect.push_back( con0 );
    vConnect.push_back( con1 );
    vConnect.push_back( con2 );
    vConnect.push_back( con3 );
    vConnect.push_back( con4 );

    PolyMesh MeshVV;
    bool bSetVertices = MeshVV.SetVertices( vVertex, vConnect );
    
    SECTION( "Check SetVertices()" ){
	REQUIRE( bSetVertices == true );
    }
	
    SECTION( "Check ChangeVertex()" ){
	Vec NewVec = Vec(3);
	NewVec._vec[0] = 99;
	NewVec._vec[1] = 100;
	NewVec._vec[2] = 101;
	bool bRet = MeshVV.ChangeVertex( 1, NewVec );
	CHECK( bRet == true );

	vector< Vec > vCheckVec;
	bRet = MeshVV.GetVec( vCheckVec );
	CHECK( bRet == true );
	REQUIRE( vCheckVec.size() == 5 );
	CHECK( vCheckVec[1]._vec[0] == 99 );
	CHECK( vCheckVec[1]._vec[1] == 100 );
	CHECK( vCheckVec[1]._vec[2] == 101 );
    }

    SECTION( "Check Vertex-Vertex connections" ){
	bool bRet = MeshVV.Calculate();
	CHECK( bRet == true );

	map< int, set<int> > MapVertexToVertex;
	bRet = MeshVV.GetVertexToVertex( MapVertexToVertex );
	CHECK( bRet == true );   

	REQUIRE( MapVertexToVertex.size() == 5 );

	REQUIRE( MapVertexToVertex[0].size() == 2 );
	CHECK( MapVertexToVertex[0].find(1) != MapVertexToVertex[0].end() );
	CHECK( MapVertexToVertex[0].find(2) != MapVertexToVertex[0].end() );

	REQUIRE( MapVertexToVertex[1].size() == 2 );
	CHECK( MapVertexToVertex[1].find(0) != MapVertexToVertex[1].end() );
	CHECK( MapVertexToVertex[1].find(2) != MapVertexToVertex[1].end() );

	REQUIRE( MapVertexToVertex[2].size() == 4 );
	CHECK( MapVertexToVertex[2].find(0) != MapVertexToVertex[2].end() );
	CHECK( MapVertexToVertex[2].find(1) != MapVertexToVertex[2].end() );
	CHECK( MapVertexToVertex[2].find(3) != MapVertexToVertex[2].end() );
	CHECK( MapVertexToVertex[2].find(4) != MapVertexToVertex[2].end() );

	REQUIRE( MapVertexToVertex[3].size() == 2 );
	CHECK( MapVertexToVertex[3].find(2) != MapVertexToVertex[3].end() );
	CHECK( MapVertexToVertex[3].find(4) != MapVertexToVertex[3].end() );

	REQUIRE( MapVertexToVertex[4].size() == 2 );
	CHECK( MapVertexToVertex[4].find(2) != MapVertexToVertex[4].end() );
	CHECK( MapVertexToVertex[4].find(3) != MapVertexToVertex[4].end() );
    }
    
    SECTION( "Check Vertice-Face connections" ){
	bool bRet = MeshVV.Calculate();
	CHECK( bRet == true );

	map< int, set<int> > MapVertexToFace;
	bRet = MeshVV.GetVertexToFace( MapVertexToFace );
	CHECK( bRet == true );   

	REQUIRE( MapVertexToFace.size() == 5);
	CHECK( MapVertexToFace[0].find(0) != MapVertexToFace[0].end() );    
	CHECK( MapVertexToFace[1].find(0) != MapVertexToFace[1].end() );    
	CHECK( MapVertexToFace[2].find(0) != MapVertexToFace[2].end() );    
	CHECK( MapVertexToFace[2].find(1) != MapVertexToFace[2].end() );    
	CHECK( MapVertexToFace[3].find(1) != MapVertexToFace[3].end() );    
	CHECK( MapVertexToFace[4].find(1) != MapVertexToFace[4].end() );

	CHECK( MapVertexToFace[0].find(1) == MapVertexToFace[0].end() );
	CHECK( MapVertexToFace[1].find(1) == MapVertexToFace[1].end() );
	CHECK( MapVertexToFace[3].find(0) == MapVertexToFace[3].end() );
	CHECK( MapVertexToFace[4].find(0) == MapVertexToFace[4].end() );
    }

    SECTION( "Check Face-Vertex connections" ){
	bool bRet = MeshVV.Calculate();
	CHECK( bRet == true );

	map< int, set<int> > MapFaceToVertex;
	bRet = MeshVV.GetFaceToVertex( MapFaceToVertex );
	CHECK( bRet == true );

	REQUIRE( MapFaceToVertex.size() == 2);
	CHECK( MapFaceToVertex[0].find(0) != MapFaceToVertex[0].end() );
	CHECK( MapFaceToVertex[0].find(1) != MapFaceToVertex[0].end() );
	CHECK( MapFaceToVertex[0].find(2) != MapFaceToVertex[0].end() );
	CHECK( MapFaceToVertex[0].find(3) == MapFaceToVertex[0].end() );
	CHECK( MapFaceToVertex[0].find(4) == MapFaceToVertex[0].end() );

	CHECK( MapFaceToVertex[1].find(2) != MapFaceToVertex[1].end() );
	CHECK( MapFaceToVertex[1].find(3) != MapFaceToVertex[1].end() );
	CHECK( MapFaceToVertex[1].find(4) != MapFaceToVertex[1].end() );
	CHECK( MapFaceToVertex[1].find(0) == MapFaceToVertex[1].end() );
	CHECK( MapFaceToVertex[1].find(1) == MapFaceToVertex[1].end() );
    }
}
TEST_CASE( "Unconnected", "[U]" ) {
    SECTION( "Check unconnected Face-Vertex" ){
	vector< Vec > vVertex;
	vector< set< int > > vConnect;
	for( int i = 0; i < 4; i++ ){ 
	    Vec a = Vec(3);
	    a._vec[0] = 5;
	    a._vec[1] = 5;
	    a._vec[2] = 5;
	    vVertex.push_back( a );
	}
    
	int conn0[] = {1,2};
	int conn1[] = {0,2};
	int conn2[] = {0,1,3};
	int conn3[] = {2};
	set<int> con0 (conn0, conn0+2);
	set<int> con1 (conn1, conn1+2);
	set<int> con2 (conn2, conn2+3);
	set<int> con3 (conn3, conn3+1);
	vConnect.push_back( con0 );
	vConnect.push_back( con1 );
	vConnect.push_back( con2 );
	vConnect.push_back( con3 );
    
	PolyMesh MeshVV;
	MeshVV.SetVertices( vVertex, vConnect );
	bool bRet = MeshVV.Calculate();
	CHECK( bRet == false );
    
	map< int, set<int> > MapFaceToVertex;
	bRet = MeshVV.GetFaceToVertex( MapFaceToVertex );

	REQUIRE( MapFaceToVertex.size() == 1 );
	CHECK( MapFaceToVertex[0].find(0) != MapFaceToVertex[0].end() );
	CHECK( MapFaceToVertex[0].find(1) != MapFaceToVertex[0].end() );
	CHECK( MapFaceToVertex[0].find(2) != MapFaceToVertex[0].end() );
	CHECK( MapFaceToVertex[0].find(3) == MapFaceToVertex[0].end() );
    }
}
TEST_CASE( "Edge", "[E]" ) {
    vector< Vec > vVertex;
    vector< set< int > > vConnect;

    for( int i = 0; i < 4; i++ ){ 
	Vec a = Vec(3);
	a._vec[0] = 5;
	a._vec[1] = 5;
	a._vec[2] = 5;
	vVertex.push_back( a );
    }

    //4 vertices forming 2 triangles, vertices 1 & 2 forming a common edge of the 2 triangles
    int conn0[] = {1,2};
    int conn1[] = {0,2,3};
    int conn2[] = {0,1,3};
    int conn3[] = {1,2};
    set<int> con0 (conn0, conn0+2);
    set<int> con1 (conn1, conn1+3);
    set<int> con2 (conn2, conn2+3);
    set<int> con3 (conn3, conn3+2);

    vConnect.push_back( con0 );
    vConnect.push_back( con1 );
    vConnect.push_back( con2 );
    vConnect.push_back( con3 );

    PolyMesh MeshVV;
    MeshVV.SetVertices( vVertex, vConnect );
    bool bCalc = MeshVV.Calculate();

    SECTION( "Compute poly mesh data" ){
	CHECK( bCalc == true );
    }
    SECTION( "Check Face-Edge connections" ){
	//expect 2 faces, each connected to 3 edges, 5 distinct edges, 1 edge shared by the 2 faces
	map< int, set< int > > MapFaceToEdge;
	bool bRet = MeshVV.GetFaceToEdge( MapFaceToEdge );
	CHECK( bRet == true );
	REQUIRE( MapFaceToEdge.size() == 2 );
	REQUIRE( MapFaceToEdge[0].size() == 3 );
	CHECK( MapFaceToEdge[0].find(0) != MapFaceToEdge[0].end() );
	CHECK( MapFaceToEdge[0].find(1) != MapFaceToEdge[0].end() );
	CHECK( MapFaceToEdge[0].find(2) != MapFaceToEdge[0].end() );
	REQUIRE( MapFaceToEdge[1].size() == 3 );
	CHECK( MapFaceToEdge[1].find(2) != MapFaceToEdge[1].end() );
	CHECK( MapFaceToEdge[1].find(3) != MapFaceToEdge[1].end() );
	CHECK( MapFaceToEdge[1].find(4) != MapFaceToEdge[1].end() );
    }
    SECTION( "Check Edge-Face connections" ){
	//expect 5 edges, 1 edge sharing 2 faces, 4 edges each coneected to 1 face
	map< int, set< int > > MapEdgeToFace;
	bool bRet = MeshVV.GetEdgeToFace( MapEdgeToFace );
	CHECK( bRet == true );
	REQUIRE( MapEdgeToFace.size() == 5 );

	REQUIRE( MapEdgeToFace[0].size() == 1 );
	CHECK( MapEdgeToFace[0].find(0) != MapEdgeToFace[0].end() );

	REQUIRE( MapEdgeToFace[1].size() == 1 );
	CHECK( MapEdgeToFace[1].find(0) != MapEdgeToFace[0].end() );

	REQUIRE( MapEdgeToFace[2].size() == 2 );
	CHECK( MapEdgeToFace[2].find(0) != MapEdgeToFace[2].end() );
	CHECK( MapEdgeToFace[2].find(1) != MapEdgeToFace[2].end() );
	
	REQUIRE( MapEdgeToFace[3].size() == 1 );
	CHECK( MapEdgeToFace[3].find(1) != MapEdgeToFace[3].end() );
	
	REQUIRE( MapEdgeToFace[4].size() == 1 );
	CHECK( MapEdgeToFace[4].find(1) != MapEdgeToFace[4].end() );
    }
}
