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

class Edge {
public:
    int data;
};

class Face {
public:
    int data;
};

class Vertex {
public:
    int data;
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

#endif
