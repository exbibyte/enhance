//==================================
//
// Summary:      Winged Edge Facilities Implementation
// Notes:
// Dependencies: N/A
//==================================
#include "WingedEdge.h"
#include <map>
#include <vector>
#include <tuple>
#include <utility>

using namespace std;

namespace Winged_Edge {
    bool Get_Edge_CW_Next( WingedEdge * WEdge, WingedEdge * & Next )
    {
	if( !WEdge ){
	    return false;
	}
	Next = WEdge->E_CW_Next;
	if( !Next ){
	    return false;
	}
	return true;
    }
    
    bool Get_Edge_CW_Prev( WingedEdge * WEdge, WingedEdge * & Prev )
    {
	if( !WEdge ){
	    return false;
	}
	Prev = WEdge->E_CW_Prev;
	if( !Prev ){
	    return false;
	}
	return true;
    }

    bool Get_Edge_CCW_Next( WingedEdge * WEdge, WingedEdge * & Next )
    {
	if( !WEdge ){
	    return false;
	}
	Next = WEdge->E_CCW_Next;
	if( !Next ){
	    return false;
	}
	return true;
    }

    bool Get_Edge_CCW_Prev( WingedEdge * WEdge, WingedEdge * & Prev )
    {
	if( !WEdge ){
	    return false;
	}
	Prev = WEdge->E_CCW_Prev;
	if( !Prev ){
	    return false;
	}
	return true;
    }

    bool Get_Face_Left( WingedEdge * WEdge, Face * & FaceLeft )
    {
	if( !WEdge ){
	    return false;
	}
	FaceLeft = WEdge->F_Left;
	if( !FaceLeft ){
	    return false;
	}
	return true;    
    }

    bool Get_Face_Right( WingedEdge * WEdge, Face * & FaceRight )
    {
	if( !WEdge ){
	    return false;
	}
	FaceRight = WEdge->F_Right;
	if( !FaceRight ){
	    return false;
	}
	return true; 
    }

    bool Get_Vertex_Start( WingedEdge * WEdge, Vertex * & VertexStart )
    {
	if( !WEdge ){
	    return false;
	}
	VertexStart = WEdge->V_Start;
	if( !VertexStart ){
	    return false;
	}
	return true; 
    }

    bool Get_Vertex_End( WingedEdge * WEdge, Vertex * & VertexEnd )
    {
	if( !WEdge ){
	    return false;
	}
	VertexEnd = WEdge->V_End;
	if( !VertexEnd ){
	    return false;
	}
	return true; 
    }

    bool Generate_WingedEdge( MapEdge map_edge, MapFace map_face, vector< WingedEdge * > & Generated )
    {
	Generated.clear();
	map< pair< Vertex *, Vertex * >, pair< WingedEdge *, bool > > map_ExistingWEdges;

	for( auto i : map_edge ){
	    //save vertices, edge of the winged edge
	    WingedEdge * current_wedge = new WingedEdge;
	    current_wedge->V_Start = i.second.first;
	    current_wedge->V_End = i.second.second;
	    current_wedge->E_Current = i.first;

	    //link entities to current winged edge
	    current_wedge->V_Start->Neighbour_WEdges.insert( current_wedge );
	    current_wedge->V_End->Neighbour_WEdges.insert( current_wedge );
	    current_wedge->E_Current->WEdge = current_wedge;
   
	    //save edge for later checking and take directionality into account
	    bool bDirPositive = true;
	    map_ExistingWEdges[ i.second ] = std::make_pair( current_wedge, bDirPositive );
       
	    //save the edge, except the other direction
	    auto alt = std::make_pair( i.second.second, i.second.first );
	    map_ExistingWEdges[ alt ] = std::make_pair( current_wedge, !bDirPositive );

	    Generated.push_back( current_wedge );
	}

	//for each face
	for( auto i : map_face ){
	    Face * face = i.first;
	    //get vertices of the face, assuming CCW order
	    Vertex * v1 = std::get<0>( i.second );
	    Vertex * v2 = std::get<1>( i.second );
	    Vertex * v3 = std::get<2>( i.second );
	    bool bClockWise = std::get<3>( i.second );
	    auto e1 = std::make_pair( v1, v2 );
	    auto e2 = std::make_pair( v2, v3 );
	    auto e3 = std::make_pair( v3, v1 );
	    vector< pair< Vertex *, Vertex * > > current_edges;
	    current_edges.push_back( e1 );
	    current_edges.push_back( e2 );
	    current_edges.push_back( e3 );

	    vector< pair< WingedEdge *, bool > > triangle_winged_edges; //save edges of current triangles to be linked

	    //get each edge of current face
	    for( auto j : current_edges ){
		auto k = map_ExistingWEdges.find( j ); //find if vertice pair already exist
		if( k == map_ExistingWEdges.end() ){ //if edge doesn't exist, this function fails
		    for( auto z : Generated ){
			delete z;
		    }  
		    return false;
		}else{
		    //link face to current winged edge depending on winged edge directionality
		    bool bDirection = k->second.second;
		    WingedEdge * winged_edge = k->second.first;
		    if( bDirection ){
			//winged_edge->F_Right = face;
			if( bClockWise ){
			    winged_edge->F_Right = face;
			}else{
			    winged_edge->F_Left = face;
			}
			face->Neighbour_WEdges.insert( winged_edge );
		    }else{
			//winged_edge->F_Left = face;
			for( auto z : Generated ){
			    delete z;
			}
			return false;
		    }
		    triangle_winged_edges.push_back( make_pair( winged_edge, bClockWise ) );
		}
	    }

	    //save neighbour winged edges depending on directionality of current winged edge
	    for( auto j = triangle_winged_edges.begin(); j != triangle_winged_edges.end(); j++ ) {
		WingedEdge * winged_edge = j->first;
		bool bDirClockWise = j->second;
		if( bDirClockWise ){
		    auto next = j+1;
		    if( next == triangle_winged_edges.end() ){
			next = triangle_winged_edges.begin();
		    }
		    auto prev = j-1;
		    if( prev < triangle_winged_edges.begin() ){
			prev = triangle_winged_edges.end()-1;
		    }
		    winged_edge->E_CW_Next = next->first;
		    winged_edge->E_CW_Prev = prev->first;
		}else{
		    auto next = j+1;
		    if( next == triangle_winged_edges.end() ){
			next = triangle_winged_edges.begin();
		    }
		    auto prev = j-1;
		    if( prev < triangle_winged_edges.begin() ){
			prev = triangle_winged_edges.end()-1;
		    }
		    winged_edge->E_CCW_Next = next->first;
		    winged_edge->E_CCW_Prev = prev->first;
		}
	    }   
	}
	return true;
    }

    bool Search_WingedEdge( WingedEdge * Start, WingedEdge * End, vector< WingedEdge * > & Path ){
      set< WingedEdge * > Searched;
	Path.clear();
	if( !Start || !End ){
	    return false;
	}
	Path.push_back( Start );
	if( Search_WingedEdge_Aux( Start, End, Searched, Path ) ){
	    return true;
	}
	return false;
    }

  bool Search_WingedEdge_Aux( WingedEdge * Start, WingedEdge * End, set< WingedEdge * > & Searched, vector< WingedEdge * > & Path ){
	if( !Start ){
	    return false;
	}
    
	//search neighbouring edges
	WingedEdge * CW_Next = Start->E_CW_Next;
	WingedEdge * CW_Prev = Start->E_CW_Prev;
	WingedEdge * CCW_Next = Start->E_CCW_Next;
	WingedEdge * CCW_Prev = Start->E_CCW_Prev;

	vector< WingedEdge * > neighbours;
	neighbours.push_back( CW_Next );
	neighbours.push_back( CW_Prev );
	neighbours.push_back( CCW_Next );
	neighbours.push_back( CCW_Prev );

	for( auto i : neighbours ){
	    if( !i ){
		continue;
	    }
	    if( i == End ){
		Searched.insert( i );
		Path.push_back( i );
		return true;
	    }
	}
	for( auto i : neighbours ){
	    if( !i ){
		continue;
	    }
	    set< WingedEdge * > Searched_Copy = Searched;
	    vector< WingedEdge * > Path_Copy = Path;
	    Searched_Copy.insert( i );
	    Path_Copy.push_back( i );
	    if( Search_WingedEdge_Aux( i, End, Searched_Copy, Path_Copy ) ){
		Searched = Searched_Copy; //save path
		Path = Path_Copy;
		return true;
	    }	
	}
	return false;
    }

}
