#include "PolyMeshGraph.h"
#include "PolyMeshInterface.h"
#include <vector>
#include <set>
#include <list>
#include <map>
using namespace std;
using namespace PolyMesh;

bool PolyMeshGraph::SetVertices( vector< Vec > & vVert, vector< set< int > > & vConnection ){
    if( vVert.size() != vConnection.size() ){
	//constraint check
	return false;
    }
    //reset data
    CleanUpAll();
    for( int i = 0; i < vVert.size(); i++ ){
	PolyMeshVertex * NewVertex = new PolyMeshVertex( i );
	_GraphVertices[ i ] = NewVertex;
	for( auto & j : vConnection[i] ){
	    int iConnect = j; 
	    if( iConnect  >= vVert.size() || iConnect < 0 ){
                // constraint check
		MarkForCleanUpAll();
		return false;
	    }else if( iConnect == i ){
		//ignore if 2 vertices are the same
		continue;
	    }
	    //check to see if vertices are already connected
	    bool bConnected = false;
	    PolyMeshVertex * FoundVertex = nullptr;
	    for( auto & k : NewVertex->_Edges ){
		for( auto & l : k->_Vertices ){
		    if( l->_Id == iConnect ){
			bConnected = true;
			FoundVertex = l;
			break;
		    }
		}
		if( bConnected ){		    
		    break;
		}
	    }
	    if( !bConnected ){
		//create new edge connecting the vertices and link them together
		int iSizeEdges = _Edges.size();
		PolyMeshEdge * NewEdge = new PolyMeshEdge( iSizeEdge );
		_GraphEdges[ iSizeEdge ] = NewEdge;

		//connect 2 vertices and 1 edge
		if( !ConnectPolyMeshObjects( NewVertex, NewEdge ) ){
		    MarkForCleanUpAll();
		    return false;
		}
		if( !ConnectPolyMeshObjects( FoundVertex, NewEdge ) ){
		    MarkForCleanUpAll();
		    return false;
		}
	    }
	}
    }
    return true;
}
bool PolyMeshGraph::CreatePolyMesh(){
    //walk the edges and detect cycles to create triangles, link them together
    list< PolyMeshVertex * > VertexList;
    list< PolyMeshEdge * > EdgeList;

    //lambda function inserts found neighbouring vertices and edges to lists
    auto Lambda_ProcessNeighbour = [&]( PolyMeshVertex * & Neighbour, PolyMeshEdge * & PathEdge ) -> bool {
	VertexList.insert( Neighbour );
	EdgeList.insert( PathEdge );
    };
    
    for( auto & i : _GraphVertices ){
	// for each vertex
	WalkNeighbourVertexToVertexViaEdge( i, Lambda_ProcessNeighbour );
	for( auto & j : VertexList ){
	    //for each vertex
	    WalkNeighbourVertexToVertexViaEdge( j, Lambda_ProcessNeighbour );
	    for( auto & k : VertexList ){
		WalkNeighbourVertexToVertexViaEdge( k, Lambda_ProcessNeighbour );
		//check if a triangle is found by number of expected items and closure
		if( 3 == VertexList.size() &&
		    3 == EdgeList.size() &&
		    VertexList.back() == i ){
		    //create face and link it with vertex and edge
		    int iCountFaces = _GraphFaces.size();
		    PolyMeshFace * NewFace = new PolyMeshFace( iCountFaces );
		    _GraphFaces[ iCountFaces ] = NewFace;		    
		    NewFace->_Vertices.insert( VertexList.begin(), VertexList.end() );
		    NewFace->_Edges.insert( EdgeList.begin(), EdgeList.end() );		    
		}
		VertexList.pop_back(); //remove last set of elements before looping to different neighbour
		EdgeList.pop_back();
	    }
	    VertexList.pop_back();
	    EdgeList.pop_back();
	}
	VertexList.pop_back();
	EdgeList.pop_back();	
    }
    return true;
}
bool PolyMeshGraph::WalkNeighbourFace( PolyMeshVertex * PolyMeshObj, std::function<bool( PolyMeshFace * )> Func ){
    bool bRet = true;
    for( auto & i : PolyMeshObj->_Faces ){
	bRet &= Func( i );
    }
    return bRet;
}
bool PolyMeshGraph::WalkNeighbourFace( PolyMeshEdge * PolyMeshObj, std::function<bool( PolyMeshFace * )> Func ){
    bool bRet = true;
    for( auto & i : PolyMeshObj->_Faces ){
	bRet &= Func( i );
    }
    return bRet;
}
bool PolyMeshGraph::WalkNeighbourVertex( PolyMeshEdge * PolyMeshObj, std::function<bool( PolyMeshVertex * )> Func ){
    bool bRet = true;
    for( auto & i : PolyMeshObj->_Vertices ){
	bRet &= Func( i );
    }
    return bRet;
}
bool PolyMeshGraph::WalkNeighbourVertex( PolyMeshFace * PolyMeshObj, std::function<bool( PolyMeshVertex * )> Func ){
    bool bRet = true;
    for( auto & i : PolyMeshObj->_Vertices ){
	bRet &= Func( i );
    }
    return bRet;
}
bool PolyMeshGraph::WalkNeighbourEdge( PolyMeshVertex * PolyMeshObj, std::function<bool( PolyMeshEdge * )> Func ){
    bool bRet = true;
    for( auto & i : PolyMeshObj->_Edges ){
	bRet &= Func( i );
    }
    return bRet;
}
bool PolyMeshGraph::WalkNeighbourEdge( PolyMeshFace * PolyMeshObj, std::function<bool( PolyMeshEdge * )> Func ){
    bool bRet = true;
    for( auto & i : PolyMeshObj->_Edges ){
	bRet &= Func( i );
    }
    return bRet;
}
bool PolyMeshGraph::GetLinkedPolyMeshById( PolyMeshBase * PolyMeshObjSource, PolyMeshType::Enum PolyType, int Id, PolyMeshBase * & PolyMeshFound ){
    return false;
}
bool PolyMeshGraph::WalkNeighbourVertexToVertexViaEdge( PolyMeshVertex * PolyMeshSource, std::function<bool( PolyMeshVertex * & Neighbour, PolyMeshEdge * & PathEdge )> Func ){
    bool bRet = true;
    //get neighbouring edges
    for( auto & i : PolyMeshSource->_Edges ){
	//get neighbouring vertices
	for( auto & j : i->_Vertices ){	    
	    //don't get same vertex as the previous vertex
	    if( PolyMeshSource == j ){
		continue;
	    }else{
		bRet &= Func( j, i );
	    }	    
	}
    }
    return bRet;
}
bool PolyMeshGraph::MarkForCleanUpAll(){
    for( auto & i : _GraphVertices ){
	i->MarkForCleanUp();
    }
    for( auto & i : _GraphEdges ){
	i->MarkForCleanUp();
    }
    for( auto & i : _GraphFaces ){
	i->MarkForCleanUp();
    }
}
bool PolyMeshGraph::CleanUpAll(){
    for( auto & i : _GraphVertices ){
	CleanUp( i );
    }
    for( auto & i : _GraphEdges ){
	CleanUp( i );
    }
    for( auto & i : _GraphFaces ){
	CleanUp( i );
    }
    return true;
}
