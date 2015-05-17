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

using namespace std;

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
    map_edge[ edges[3] ] = std::make_pair( vertices[1], vertices[3] );
    map_edge[ edges[4] ] = std::make_pair( vertices[3], vertices[2] );

    vector< Face * > faces;
    for( int i = 0; i < 2; i++ ){
        Face * f = new Face;
        f->data = i;
        faces.push_back( f );
    }
    
    //link faces and edges
    map_face[ faces[0] ] = std::make_tuple( vertices[0], vertices[1], vertices[2] );
    map_face[ faces[1] ] = std::make_tuple( vertices[1], vertices[3], vertices[2] );
    
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
            if( 0 != i->F_Left ){
                count_face_left++;
                generated_faces.insert( i->F_Left );
            }
            if( 0 != i->F_Right ){
                count_face_right++;
                generated_faces.insert( i->F_Right );
            }
        }
        CHECK( 1 == count_face_left ); //check number of left faces
        CHECK( 5 == count_face_right ); //check number of right faces
        CHECK( 2 == generated_faces.size() ); //check size of linked faces        
    }

    //TODO:
    SECTION( "Check entity linkage" ){
    }
}
