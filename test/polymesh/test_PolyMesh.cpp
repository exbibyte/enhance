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
  int conn2[] = {0,1,4};
  int conn3[] = {2,4};
  int conn4[] = {3,2};
  set<int> con0 (conn0, conn0+2);
  set<int> con1 (conn1, conn1+2);
  set<int> con2 (conn2, conn2+3);
  set<int> con3 (conn3, conn3+2);
  set<int> con4 (conn4, conn4+2);

  vConnect.push_back( con0 );
  vConnect.push_back( con1 );
  vConnect.push_back( con2 );
  vConnect.push_back( con3 );
  vConnect.push_back( con4 );

  PolyMesh MeshVV;
  MeshVV.SetVertices( vVertex, vConnect );

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

  SECTION( "Check VerticeList" ){
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

  SECTION( "Check FaceList" ){
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
  SECTION( "Check unconnected vertices" ){
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
