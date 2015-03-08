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
		//create new edge connecting the vertices
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

}
bool PolyMeshGraph::GetNeighbourFace( PolyMeshBase * PolyMeshObj, set< PolyMeshFace * > & sFace ){

}
bool PolyMeshGraph::GetNeighbourVertex( PolyMeshBase * PolyMeshObj, set< PolyMeshVertex * > & sVertex ){

}
bool PolyMeshGraph::GetNeighbourEdge( PolyMeshBase * PolyMeshObj, set< PolyMeshEdge * > & sEdge ){

}
bool PolyMeshGraph::GetLinkedPolyMeshById( PolyMeshBase * PolyMeshObjSource, PolyMeshType::Enum PolyType, int Id, PolyMeshBase * & PolyMeshFound ){
    
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
