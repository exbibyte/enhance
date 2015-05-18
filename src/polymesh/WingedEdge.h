//==================================
//
// Summary:      Winged Edge Facilities Declaration
// Notes:
// Dependencies: N/A
//==================================
#ifndef WINGED_EDGE_H
#define WINGED_EDGE_H

#include <map>
#include <vector>
#include <tuple>
#include <utility>
#include <set>

#include "Vec.h"

namespace Winged_Edge {
  
    class WingedEdge;

    class Edge {
    public:
	int data;
	WingedEdge * WEdge;
    };
  
    class Face {
    public:
	int data;
	std::set< WingedEdge * > Neighbour_WEdges;
    };
  
    class Vertex {
    public:
	int data;
	Vec v;
	std::set< WingedEdge * > Neighbour_WEdges;
    };
  
    class WingedEdge {
    public:
	WingedEdge(){
	    E_Current = 0;
	    V_Start = 0;
	    V_End = 0;
	    E_CCW_Prev = 0;
	    E_CCW_Next = 0;
	    E_CW_Prev = 0;
	    E_CW_Next = 0;
	    F_Left = 0;
	    F_Right = 0;
	}
	Edge * E_Current;
	Vertex * V_Start;
	Vertex * V_End;
	WingedEdge * E_CCW_Prev;
	WingedEdge * E_CCW_Next;
	WingedEdge * E_CW_Prev;
	WingedEdge * E_CW_Next;
	Face * F_Left;
	Face * F_Right;
    };

    typedef std::map< Edge *, std::pair< Vertex *, Vertex * > > MapEdge;
    typedef std::map< Face *, std::tuple< Vertex *, Vertex *, Vertex *, bool > > MapFace;

    bool Get_Edge_CW_Next( WingedEdge * WEdge, WingedEdge * & Next );
    bool Get_Edge_CW_Prev( WingedEdge * WEdge, WingedEdge * & Prev );
    bool Get_Edge_CCW_Next( WingedEdge * WEdge, WingedEdge * & Next );
    bool Get_Edge_CCW_Prev( WingedEdge * WEdge, WingedEdge * & Prev );
    bool Get_Face_Left( WingedEdge * WEdge, Face * & FaceLeft );
    bool Get_Face_Right( WingedEdge * WEdge, Face * & FaceRight );
    bool Get_Vertex_Start( WingedEdge * WEdge, Vertex * & VertexStart );
    bool Get_Vertex_End( WingedEdge * WEdge, Vertex * & VertexEnd );

    bool Generate_WingedEdge( MapEdge map_edge, MapFace map_face, std::vector< WingedEdge * > & Generated ); //generates winged edges given input edge-vertices and face-vertices (CCW) maps

    bool Search_WingedEdge_Via_Edge( WingedEdge * Start, WingedEdge * End, std::vector< WingedEdge * > & Path );
    bool Search_WingedEdge_Via_Edge_Aux( WingedEdge * Start, WingedEdge * End, std::set< WingedEdge * > & Searched, std::vector< WingedEdge * > & Path );
        
    bool Is_WingedEdge_Neighour_WingedEdge( WingedEdge * WEdge1, WingedEdge * WEdge2 );
    bool Is_WingedEdge_Neighour_Face( WingedEdge * WEdge, Face * face );
    bool Is_WingedEdge_Neighbour_Vertex( WingedEdge * WEdge, Vertex * vertex );
    
    bool Get_WingedEdge_Neighour_Faces( WingedEdge * WEdge, std::set< Face * > & faces );
    bool Get_WingedEdge_Neighour_Vertices( WingedEdge * WEdge, std::set< Vertex * > & vertices );
    bool Get_WingedEdge_Neighour_WingedEdges( WingedEdge * WEdge, std::set< WingedEdge * > & WingedEdges );
    
    bool Get_Face_Neighbour_WingedEdges( Face * face, std::set< WingedEdge * > & WEdges );
    bool Get_Vertex_Neighbour_WingedEdges( Vertex * vertex, std::set< WingedEdge * > & WEdges );
}
#endif
