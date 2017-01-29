#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include "catch.hpp"
#include "Vec.hpp"
#include <math.h>
#include "PolyMeshInterface.h"
#include "PolyMeshGraph.h"
#include <vector>
#include <set>
#include <iostream>
using namespace std;
using namespace PolyMesh;

class PolyMeshInterfaceGraph : public PolyMeshInterface< PolyMeshGraph > {};

TEST_CASE( "Basic", "[B]" ) {

    vector< Vec > vVertex;
    vector< set< int > > vConnect;

    for( int i = 0; i < 5; i++ ){ 
	Vec a = Vec(3);
	a._vec[0] = i;
	a._vec[1] = i;
	a._vec[2] = i;
	vVertex.push_back( a );
    }

    // create 2 triangles from 5 points, with 1 vertex shared:
    // Triangle 0: V0-V1-V2, E0-E1-E2
    // Triangle 1: V2-V3-V4, E3-E4-E5
    // Edge 0: V0-V1
    // Edge 1: V0-V2
    // Edge 2: V1-V2
    // Edge 3: V2-V3
    // Edge 4: V2-V4
    // Edge 5: V3-V4
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

    PolyMeshInterfaceGraph _PolyMesh;
    
    //create vertice and edge information
    bool bSetVertices = _PolyMesh.SetVertices( vVertex, vConnect );    
    SECTION( "Check SetVertices() return value" ){
	REQUIRE( bSetVertices == true );
    }

    //create face information
    bool bCreatePolyMesh = _PolyMesh.CreatePolyMesh();
    SECTION( "Check CreatePolyMesh() return value" ){
	REQUIRE( bCreatePolyMesh == true );
    }
    
    //check linkages
    SECTION( "Check Vertex-Edge linkage" ){
	list< PolyMeshVertex * > Vertices;
	bool bGetVertices = _PolyMesh.GetVertices( Vertices );
	REQUIRE( bGetVertices == true );

	// lambda for checking expected edges for each vertex
	auto CheckEdges = [&]( list< PolyMeshVertex * > VertexList, int iVertId, int iEdge1Id, int iEdge2Id )->bool{	   
	    for( auto & i : VertexList ){
		if( i->_Id == iVertId ){
		    bool bFoundFirstEdge = false;
		    bool bFoundSecondEdge = false;
		    for( auto & j : i->_Edges ){
			if( j->_Id == iEdge1Id ){
			    bFoundFirstEdge = true;
			}else if( j->_Id == iEdge2Id ){
			    bFoundSecondEdge = true;
			}
		    }
		    bool bFoundEdges = ( bFoundFirstEdge & bFoundSecondEdge );
		    return bFoundEdges;
		}
	    }
	    return false;
	};

	CHECK( CheckEdges( Vertices, 0, 0, 1 ) == true ); // V0: E0, E1
	CHECK( CheckEdges( Vertices, 1, 0, 2 ) == true ); // V1: E0, E2
	CHECK( CheckEdges( Vertices, 2, 1, 2 ) == true ); // V2: E1, E2, E3, E4
	CHECK( CheckEdges( Vertices, 2, 3, 4 ) == true );
	CHECK( CheckEdges( Vertices, 3, 3, 5 ) == true ); // V3: E3, E5
	CHECK( CheckEdges( Vertices, 4, 4, 5 ) == true ); // V4: E4, E5	
    }   
    SECTION( "Check Vertex-Face connections" ){
	list< PolyMeshVertex * > Vertices;
	bool bGetVertices = _PolyMesh.GetVertices( Vertices );
	REQUIRE( bGetVertices == true );

	// lambda for checking expected faces for each vertex
	auto CheckFaces = [&]( list< PolyMeshVertex * > VertexList, int iVertId, int iFaceId )->bool{	   
	    for( auto & i : VertexList ){
		if( i->_Id == iVertId ){
		    for( auto & j : i->_Faces ){
			if( j->_Id == iFaceId ){
			    return true;
			}
		    }
		}
	    }
	    return false;
	};

	CHECK( CheckFaces( Vertices, 0, 0 ) == true ); // V0: F0
	CHECK( CheckFaces( Vertices, 1, 0 ) == true ); // V1: F0
	CHECK( CheckFaces( Vertices, 2, 0 ) == true ); // V2: F0, F1
	CHECK( CheckFaces( Vertices, 2, 1 ) == true );
	CHECK( CheckFaces( Vertices, 3, 1 ) == true ); // V3: F1
	CHECK( CheckFaces( Vertices, 4, 1 ) == true ); // V4: F1	
    }
    SECTION( "Check Edge-Vertex connections" ){
	list< PolyMeshEdge * > Edges;
	bool bGetEdges = _PolyMesh.GetEdges( Edges );
	REQUIRE( bGetEdges == true );

	// lambda for checking expected vertices for each edge
	auto CheckVertices = [&]( list< PolyMeshEdge * > EdgeList, int iEdgeId, int iVert1Id, int iVert2Id )->bool{	   
	    for( auto & i : EdgeList ){
		if( i->_Id == iEdgeId ){
		    bool bFoundFirstVert = false;
		    bool bFoundSecondVert = false;
		    for( auto & j : i->_Vertices ){
			if( j->_Id == iVert1Id ){
			    bFoundFirstVert = true;
			}else if( j->_Id == iVert2Id ){
			    bFoundSecondVert = true;
			}
		    }
		    bool bFoundVerts = ( bFoundFirstVert & bFoundSecondVert );
		    return bFoundVerts;
		}
	    }
	    return false;
	};
	
	CHECK( CheckVertices( Edges, 0, 0, 1 ) == true ); // 	// Edge 0: V0-V1
	CHECK( CheckVertices( Edges, 1, 0, 2 ) == true ); // 	// Edge 1: V0-V2
	CHECK( CheckVertices( Edges, 2, 1, 2 ) == true ); // 	// Edge 2: V1-V2
	CHECK( CheckVertices( Edges, 3, 2, 3 ) == true ); // 	// Edge 3: V2-V3
	CHECK( CheckVertices( Edges, 4, 2, 4 ) == true ); // 	// Edge 4: V2-V4
	CHECK( CheckVertices( Edges, 5, 3, 4 ) == true ); // 	// Edge 5: V3-V4
    }

    SECTION( "Check Edge-Face connections" ){
	list< PolyMeshEdge * > Edges;
	bool bGetEdges = _PolyMesh.GetEdges( Edges );
	REQUIRE( bGetEdges == true );

	// lambda for checking expected vertices for each edge
	auto CheckFaces = [&]( list< PolyMeshEdge * > EdgeList, int iEdgeId, int iFaceId )->bool{	   
	    for( auto & i : EdgeList ){
		if( i->_Id == iEdgeId ){
		    for( auto & j : i->_Faces ){
			if( j->_Id == iFaceId ){
			    return true;
			}
		    }
		}
	    }
	    return false;
	};
	
	CHECK( CheckFaces( Edges, 0, 0 ) == true ); // 	// Edge 0: F0
	CHECK( CheckFaces( Edges, 1, 0 ) == true ); // 	// Edge 1: F0
	CHECK( CheckFaces( Edges, 2, 0 ) == true ); // 	// Edge 2: F0
	CHECK( CheckFaces( Edges, 3, 1 ) == true ); // 	// Edge 3: F1
	CHECK( CheckFaces( Edges, 4, 1 ) == true ); // 	// Edge 4: F1
	CHECK( CheckFaces( Edges, 5, 1 ) == true ); // 	// Edge 5: F1
    }

    SECTION( "Check Face-Vertex connections" ){
	list< PolyMeshFace * > Faces;	
	bool bGetFaces = _PolyMesh.GetFaces( Faces );
	REQUIRE( bGetFaces == true );

	// lambda for checking expected vertices for each face
	auto CheckVertices = [&]( list< PolyMeshFace * > FaceList, int iFaceId, int iVert1Id, int iVert2Id, int iVert3Id )->bool{	   
	    for( auto & i : FaceList ){
		if( i->_Id == iFaceId ){
		    bool bFoundVert1 = true;
		    bool bFoundVert2 = true;
		    bool bFoundVert3 = true;
		    for( auto & j : i->_Vertices ){
			if( j->_Id == iVert1Id ){
			    bFoundVert1 = true;
			}else if( j->_Id == iVert2Id ){
			    bFoundVert2 = true;
			}else if( j->_Id == iVert3Id ){
			    bFoundVert3 = true;
			}
		    }
		    bool bFoundVerts = ( bFoundVert1 & bFoundVert2 & bFoundVert3 );
		    return bFoundVerts;
		}
	    }
	    return false;
	};
	
	CHECK( CheckVertices( Faces, 0, 0, 1, 2 ) == true ); // 	// F0: V0, V1, V2
	CHECK( CheckVertices( Faces, 1, 2, 3, 4 ) == true ); // 	// F1: V2, V3, V4
    }

    SECTION( "Check Face-Edge connections" ){
	list< PolyMeshFace * > Faces;	
	bool bGetFaces = _PolyMesh.GetFaces( Faces );
	REQUIRE( bGetFaces == true );

	// lambda for checking expected edges for each face
	auto CheckEdges = [&]( list< PolyMeshFace * > FaceList, int iFaceId, int iEdge1Id, int iEdge2Id, int iEdge3Id )->bool{	   
	    for( auto & i : FaceList ){
		if( i->_Id == iFaceId ){
		    bool bFoundEdge1 = true;
		    bool bFoundEdge2 = true;
		    bool bFoundEdge3 = true;
		    for( auto & j : i->_Edges ){
			if( j->_Id == iEdge1Id ){
			    bFoundEdge1 = true;
			}else if( j->_Id == iEdge2Id ){
			    bFoundEdge2 = true;
			}else if( j->_Id == iEdge3Id ){
			    bFoundEdge3 = true;
			}
		    }
		    bool bFoundEdges = ( bFoundEdge1 & bFoundEdge2 & bFoundEdge3 );
		    return bFoundEdges;
		}
	    }
	    return false;
	};
	
	CHECK( CheckEdges( Faces, 0, 0, 1, 2 ) == true ); // 	// F0: E0, E1, E2
	CHECK( CheckEdges( Faces, 1, 3, 4, 5 ) == true ); // 	// F1: E3, E4, E5
    }
}
