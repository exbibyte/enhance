//==================================
//
// Summary:      Winged Edge Facilities Test
// Notes:
// Dependencies: N/A
//==================================
#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include "catch.hpp"

#include "WingedEdge.h"

#include <map>
#include <vector>
#include <tuple>
#include <utility>
#include <set>
#include <iostream>

using namespace std;
using namespace Winged_Edge;

TEST_CASE( "Basic", "[B]" ) {
  
    MapEdge map_edge;
    MapFace map_face;
    
    vector< Edge * > edges;
    for( int i = 0; i < 5; i++ ){
        Edge * e = new Edge;
        e->data = i;
        edges.push_back( e );
    }

    vector< Vertex * > vertices;
    for( int i = 0; i < 4; i++ ){
        Vertex * v = new Vertex;
        v->data = i;
        vertices.push_back( v );
    }

    //link edges and vertices
    map_edge[ edges[0] ] = std::make_pair( vertices[0], vertices[1] );
    map_edge[ edges[1] ] = std::make_pair( vertices[1], vertices[2] );
    map_edge[ edges[2] ] = std::make_pair( vertices[2], vertices[0] );
    map_edge[ edges[3] ] = std::make_pair( vertices[2], vertices[3] );
    map_edge[ edges[4] ] = std::make_pair( vertices[3], vertices[1] );

    vector< Face * > faces;
    for( int i = 0; i < 2; i++ ){
        Face * f = new Face;
        f->data = i;
        faces.push_back( f );
    }
    
    //link faces and edges
    bool bClockWise = true;
    map_face[ faces[0] ] = std::make_tuple( vertices[0], vertices[1], vertices[2], bClockWise );
    map_face[ faces[1] ] = std::make_tuple( vertices[1], vertices[2], vertices[3], !bClockWise );
    
    //generate winged edges
    vector< WingedEdge * > generated_wedges;
    Generate_WingedEdge( map_edge, map_face, generated_wedges );

    //verify generated wedges    
    // bool Get_Edge_CW_Next( WingedEdge * WEdge, WingedEdge * Next );
    // bool Get_Edge_CW_Prev( WingedEdge * WEdge, WingedEdge * Prev );
    // bool Get_Edge_CCW_Next( WingedEdge * WEdge, WingedEdge * Next );
    // bool Get_Edge_CCW_Prev( WingedEdge * WEdge, WingedEdge * Prev );
    // bool Get_Face_Left( WingedEdge * WEdge, Face * FaceLeft );
    // bool Get_Face_Right( WingedEdge * WEdge, Face * FaceRight );
    // bool Get_Vertex_Start( WingedEdge * WEdge, Vertex * VertexStart );
    // bool Get_Vertex_End( WingedEdge * WEdge, Vertex * VertexEnd );

    SECTION( "Check size of generated entities" ){
        CHECK( 5 == generated_wedges.size() ); //check size of winged edges
        set< Face * > generated_faces;
        int count_face_left = 0;
        int count_face_right = 0;
        for( auto i : generated_wedges ){
	    Face * left_face;
	    if( Get_Face_Left( i, left_face ) ){ 
                count_face_left++;
                generated_faces.insert( left_face );
            }
	    Face * right_face;
            if( Get_Face_Right( i, right_face ) ){
                count_face_right++;
                generated_faces.insert( right_face );
            }
        }
        CHECK( 3 == count_face_left ); //check number of edges linked with a left face
        CHECK( 3 == count_face_right ); //check number of edges linked with a right face
        CHECK( 2 == generated_faces.size() ); //check number of faces        
    }

    //TODO:
    SECTION( "Check entity linkage" ){
      for( auto i : generated_wedges ){
	//first trianglge
	if( i->E_Current->data == 0 ){
	  vector<int> expected_data { 1, 2, 0 };
	  WingedEdge * Current = i;
	  WingedEdge * Next = 0;
	  for( auto j: expected_data ){
	    bool bRet = Get_Edge_CW_Next( Current, Next );
	    CHECK( true == bRet );
	    CHECK( j == Next->E_Current->data );
	    Current = Next;
	    Next = 0;
	  }

	  vector<int> expected_data_reverse { 2, 1, 0 };
          Current = i;
	  Next = 0;
	  for( auto j: expected_data_reverse ){
	    bool bRet = Get_Edge_CW_Prev( Current, Next );
	    CHECK( true == bRet );
	    CHECK( j == Next->E_Current->data );
	    Current = Next;
	    Next = 0;
	  }	  
	}
	//second triangle
	if( i->E_Current->data == 3 ){
	  vector<int> expected_data { 4, 1, 3 };
	  WingedEdge * Current = i;
	  WingedEdge * Next = 0;
	  for( auto j: expected_data ){
	    bool bRet = Get_Edge_CCW_Next( Current, Next );
	    CHECK( true == bRet );
	    CHECK( j == Next->E_Current->data );
	    Current = Next;
	    Next = 0;
	  }
	}       
      }
    }

    SECTION( "Check size of generated entities" ){
      WingedEdge * Start;
      WingedEdge * End;
      for( auto i : generated_wedges ){
	if( i->E_Current->data == 2 ){
	  Start = i;
	}
	if( i->E_Current->data == 4 ){
	  End = i;
	}
      }
      std::vector< WingedEdge * > Path;
      CHECK( Search_WingedEdge_Via_Edge( Start, End, Path ) );
      cout << "Search path: ";
      for( auto j : Path ){
	cout << j->E_Current->data << " ";
      }
      cout<<endl;
      CHECK( 3 == Path.size() );
      
    }
}
