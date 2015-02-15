#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include "catch.hpp"
#include "Vec.h"
#include <math.h>
#include "PolyMesh_VV.h"
#include <vector>
#include <set>
#include <iostream>
using namespace std;

TEST_CASE( "VV", "[VV]" ) {

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

  PolyMesh_VV MeshVV;
  MeshVV.SetVertices( vVertex, vConnect );

  SECTION( "Check ChangeVertex()" ){
    Vec NewVec = Vec(3);
    NewVec._vec[0] = 99;
    NewVec._vec[1] = 100;
    NewVec._vec[2] = 101;
    bool bRet = MeshVV.ChangeVertex( 1, NewVec );
    REQUIRE( bRet == true );

    Vec CheckVec;
    bRet = MeshVV.GetVec( 1, CheckVec );
    REQUIRE( bRet == true );
    REQUIRE( CheckVec._vec[0] == 99 );
    REQUIRE( CheckVec._vec[1] == 100 );
    REQUIRE( CheckVec._vec[2] == 101 );
  }
  SECTION( "Check GetAllVec()" ){
    map< int, Vec > MapVec;
    bool bRet = MeshVV.GetAllVec( MapVec );
    CHECK( MapVec.size() == 5 );
  }
  SECTION( "Check GetAllIndex()" ){
    set< int > SetIndex;    
    bool bRet = MeshVV.GetAllIndex( SetIndex );
    REQUIRE( SetIndex.size() == 5 );
  }
  SECTION( "Check CalcFaces()" ){
    map< int, set<int> > MapFaceList;
    map< int, set<int> > MapVertexList;
    map< int, Vec > MapVec;
    MeshVV.CalcFaces( MapFaceList, MapVertexList, MapVec );
    REQUIRE( MapFaceList.size() == 2 );

    CHECK( MapFaceList[0].find(0) != MapFaceList[0].end() );
    CHECK( MapFaceList[0].find(1) != MapFaceList[0].end() );
    CHECK( MapFaceList[0].find(2) != MapFaceList[0].end() );
    CHECK( MapFaceList[0].find(3) == MapFaceList[0].end() );
    CHECK( MapFaceList[0].find(4) == MapFaceList[0].end() );

    CHECK( MapFaceList[1].find(2) != MapFaceList[1].end() );
    CHECK( MapFaceList[1].find(3) != MapFaceList[1].end() );
    CHECK( MapFaceList[1].find(4) != MapFaceList[1].end() );
    CHECK( MapFaceList[1].find(0) == MapFaceList[1].end() );
    CHECK( MapFaceList[1].find(1) == MapFaceList[1].end() );

    CHECK( MapVertexList.size() == 5 );
    
    CHECK( MapVec.size() == 5 );
  }
}
TEST_CASE( "VV_Unconnected", "[VV_Uncon]" ) {
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
    
    PolyMesh_VV MeshVV;
    MeshVV.SetVertices( vVertex, vConnect );

    map< int, set<int> > MapFaceList;
    map< int, set<int> > MapVertexList;
    map< int, Vec > MapVec;
    bool bRet = MeshVV.CalcFaces( MapFaceList, MapVertexList, MapVec );
    CHECK( bRet == false );
    REQUIRE( MapFaceList.size() == 1 );

    CHECK( MapFaceList[0].find(0) != MapFaceList[0].end() );
    CHECK( MapFaceList[0].find(1) != MapFaceList[0].end() );
    CHECK( MapFaceList[0].find(2) != MapFaceList[0].end() );
    CHECK( MapFaceList[0].find(3) == MapFaceList[0].end() );

    CHECK( MapVertexList.size() == 3 );
    
    CHECK( MapVec.size() == 4 );
  }
}
