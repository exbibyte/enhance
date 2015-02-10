#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include "catch.hpp"
#include "Vec.h"
#include <math.h>
#include "PolyMesh_VV.h"
#include <vector>
using namespace std;

TEST_CASE( "VV", "[VV]" ) {

  vector< Vec > vVertex;
  vector< vector< int > > vConnect;

  for( int i = 0; i < 5; i++ ){ 
    Vec a = Vec(3);
    a._vec[0] = 0;
    a._vec[1] = 1;
    a._vec[2] = 2;
    vVertex.push_back( a );

    vector< int > newconnect;
    if( i == 0 )
    {
      newconnect.push_back(1);
    }
    else
    {
      for( int j = 0; j < 5; j++ )
      {
	if( i != j )
	{
	  newconnect.push_back(j);
	}
      }
    }
    vConnect.push_back( newconnect );
  }

  PolyMesh_VV MeshVV;
  MeshVV.SetVertices( vVertex, vConnect );

  Vec NewVec = Vec(3);
  NewVec._vec[0] = 99;
  NewVec._vec[1] = 100;
  NewVec._vec[2] = 101;
  vector< int > NewConnect { 3, 4 };
  MeshVV.ChangeVertex( 1, NewVec, NewConnect );

  vector< tuple< int, Vec, vector<int > > > Vertices;
  Vertices.clear();
  bool bRet = MeshVV.GetVertices( Vertices );

  Vec CheckVec;
  bRet = MeshVV.GetVertex( 1, CheckVec );

  SECTION( "Check ChangeVertex()" ){
    REQUIRE( bRet == true );
    REQUIRE( CheckVec._vec[0] == 99 );
    REQUIRE( CheckVec._vec[1] == 100 );
    REQUIRE( CheckVec._vec[2] == 101 );
  }  
}
