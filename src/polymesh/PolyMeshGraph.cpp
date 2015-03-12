#include "PolyMeshGraph.h"
#include "PolyMeshInterface.h"
#include <vector>
#include <set>
#include <list>
#include <map>
#include <tuple>
#include <iostream>
#include <algorithm>
#include <utility>
using namespace std;
using namespace PolyMesh;

PolyMeshGraph::~PolyMeshGraph(){
   //reset data
    MarkAllForCleanUp();
    CleanUp();
    DeleteCleanUp();
}
bool PolyMeshGraph::SetVertices( vector< Vec > & vVert, vector< set< int > > & vConnection ){
    if( vVert.size() != vConnection.size() ){
	//constraint check
	return false;
    }
    //reset data
    MarkAllForCleanUp();
    CleanUp();
    //create vertices
    for( int i = 0; i < vVert.size(); i++ ){
	PolyMeshVertex * NewVertex = new PolyMeshVertex( i, vVert[i] );
	_GraphVertices[ i ] = NewVertex;
    }

    //create edges
    for( int i = 0; i < vVert.size(); i++ ){
	PolyMeshVertex * CurrentVertex = _GraphVertices[ i ];
	for( auto & j : vConnection[i] ){
	    int iConnect = j; 
	    if( iConnect  >= vVert.size() || iConnect < 0 ){
                // constraint check
		MarkAllForCleanUp();
		CleanUp();
		DeleteCleanUp();
		return false;
	    }else if( iConnect == i ){
		//ignore if 2 vertices are the same
		continue;
	    }
	    //check to see if vertices are already connected
	    bool bConnected = false;
	    PolyMeshVertex * FoundVertex = nullptr;
	    for( auto & k : CurrentVertex->_Edges ){
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
		//get vertex
		auto it_FoundVert = _GraphVertices.find( iConnect );
		if( it_FoundVert == _GraphVertices.end() ){		    
		    MarkAllForCleanUp();
		    CleanUp();	
		    return false;
		}
		FoundVertex = it_FoundVert->second;
		
		//create new edge connecting the vertices and link them together
		int iSizeEdges = _GraphEdges.size();
		PolyMeshEdge * NewEdge = new PolyMeshEdge( iSizeEdges );
		_GraphEdges[ iSizeEdges ] = NewEdge;

		//connect 2 vertices and 1 edge
		if( !ConnectPolyMeshObjects( CurrentVertex, NewEdge ) ){
		    MarkAllForCleanUp();
		    CleanUp();
		    return false;
		}
		if( !ConnectPolyMeshObjects( FoundVertex, NewEdge ) ){
		    MarkAllForCleanUp();
		    CleanUp();
		    return false;
		}
	    }
	}
    }
    
    return true;
}
bool PolyMeshGraph::CreatePolyMesh(){
    bool bRet = true;

    auto CompVert = []( PolyMeshVertex * const & lhs, PolyMeshVertex * const & rhs ) -> bool {
	return lhs->_Id < rhs->_Id;
    };
    
    //storing already found triangles
    set< set< PolyMeshVertex * const, decltype( CompVert ) > > SetTriangles;
    
    //walk the edges and detect cycles to create triangles, link them together
    set< PolyMeshVertex * const > VertexList;
    set< PolyMeshEdge * const > EdgeList;
    set< PolyMeshVertex * const > VertexListFound;
    set< PolyMeshEdge * const > EdgeListFound;

    map< tuple< int, int, int >, pair< list< PolyMeshVertex * >, list< PolyMeshEdge * > > > FoundTriangles;
    
    for( auto i : _GraphVertices ){
	WalkToFindTriangle( i.second, FoundTriangles );
    }
    
#ifdef DEBUG_POLYMESH_GRAPH
    cout << "Number of triangles created: " << FoundTriangles.size() <<endl;
#endif

    //create faces for found triangles
    for( auto i : FoundTriangles ){
    	int iCountFaces = _GraphFaces.size();
	auto vertices = i.second.first;
	auto edges = i.second.second;
	PolyMeshFace * NewFace = new PolyMeshFace( iCountFaces );
    	_GraphFaces[ iCountFaces ] = NewFace;
	//link vertices to face
	for( auto j : vertices ){
	    bool bConnect = ConnectPolyMeshObjects( j, NewFace );
	}
	//link edges to face
	for( auto k : edges ){
	    bool bConnect = ConnectPolyMeshObjects( k, NewFace );
	}

#ifdef DEBUG_POLYMESH_GRAPH
	cout << "created face: id: " << NewFace->_Id << endl;
#endif
    }
    
    return bRet;
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
bool PolyMeshGraph::WalkNeighbourVertexToVertexViaEdge( PolyMeshVertex * PolyMeshSource, bool bEndOnTrue, std::function<bool( PolyMeshVertex * PathVertex, PolyMeshEdge * PathEdge )> Func ){
    bool bRet = true;
    //get neighbouring edges
    for( set< PolyMeshEdge * >::iterator i = PolyMeshSource->_Edges.begin(); i != PolyMeshSource->_Edges.end(); ++i ){
	//get neighbouring vertices
	for(  set< PolyMeshVertex * >::iterator j = (*i)->_Vertices.begin(); j != (*i)->_Vertices.end(); ++j ){	    
	    //don't get same vertex as the previous vertex
	    if( PolyMeshSource == *j ){
		continue;
	    }else{
		bRet &= Func( *j, *i );
		if( bRet && bEndOnTrue ){
		    return true;
		}
	    }	    
	}
    }
    return bRet;
}
bool PolyMeshGraph::WalkToFindTriangle( PolyMeshVertex * PolyMeshSource, map< tuple< int, int, int >, pair< list< PolyMeshVertex * >, list < PolyMeshEdge * > > >  & FoundTriangles )
{
    bool bRet = true;
    
    list< PolyMeshVertex * > NeighbourVertices;
    list< PolyMeshEdge * > NeighbourEdges;

    auto GetNeighbourVertices = [&]( PolyMeshVertex * PathVertex, PolyMeshEdge * PathEdge ) -> bool {
	NeighbourVertices.push_back( PathVertex );
	NeighbourEdges.push_back( PathEdge );
	return true;
    };
    //get neighbouring vertices and edges
    WalkNeighbourVertexToVertexViaEdge( PolyMeshSource, false, GetNeighbourVertices );
    
    for( auto i : NeighbourVertices ){
	for( auto j : NeighbourVertices ){
	    if( i == j ){
			
	    }else{
		PolyMeshEdge * Edge_1st = nullptr;
		PolyMeshEdge * Edge_2nd = nullptr;
		PolyMeshEdge * Edge_3rd = nullptr;
		bool bIsClosed = false;
		auto IsTriangleClosed = [&]( PolyMeshVertex * PathVertex, PolyMeshEdge * PathEdge ) -> bool {
		    if( j == PathVertex ){ //check if ending vertex is same as starting vertex
			bIsClosed = true;
			Edge_3rd = PathEdge; //save the 3rd edge of triangle
			return true;
		    }
		    return false;
		};
		WalkNeighbourVertexToVertexViaEdge( i, true, IsTriangleClosed );
		if( bIsClosed ){
		    bool bFoundEdge1 = false;
		    bool bFoundEdge2 = false;
		    auto GetEdges = [&]( PolyMeshVertex * PathVertex, PolyMeshEdge * PathEdge ) -> bool {
			if( i == PathVertex ){
			    Edge_1st = PathEdge; //save an edge of triangle
			    bFoundEdge1 = true;
			}else if( j == PathVertex ){
			    Edge_2nd = PathEdge; //save an edge of triangle
			    bFoundEdge2 = true;
			}
			if( bFoundEdge1 && bFoundEdge2 ){
			    return true;
			}
			return false;
		    };		    
		    //get other 2 edges
		    WalkNeighbourVertexToVertexViaEdge( PolyMeshSource, true, GetEdges );
		    
		    vector< int > Ids;
		    Ids.push_back( PolyMeshSource->_Id );
		    Ids.push_back( i->_Id );
		    Ids.push_back( j->_Id );
		    std::sort( Ids.begin(), Ids.end() ); // sort Ids
		    tuple< int, int, int > KeyIds = make_tuple( Ids[0], Ids[1], Ids[2] ); // make tuple key
		    if( FoundTriangles.find( KeyIds ) == FoundTriangles.end() ){
			// make a new triangle if it doesn't already exist
			list < PolyMeshVertex * > FoundVertices;
			FoundVertices.push_back( PolyMeshSource );
			FoundVertices.push_back( i );
			FoundVertices.push_back( j );

			list < PolyMeshEdge * > FoundEdges;
			FoundEdges.push_back( Edge_1st );
			FoundEdges.push_back( Edge_2nd );
			FoundEdges.push_back( Edge_3rd );
			
			FoundTriangles[ KeyIds ] = make_pair( FoundVertices, FoundEdges );
#ifdef DEBUG_POLYMESH_GRAPH
			cout << "created triangle: " << endl;
			cout << "                  v( " << PolyMeshSource->_Id << ", " << i->_Id << ", " << j->_Id << " )" <<endl;
			cout << "                  e( " << Edge_1st->_Id << ", " << Edge_2nd->_Id << ", " << Edge_3rd->_Id << " )" << endl;
#endif
		    }		   
		}
	    }	 
	}
    }
    
    return bRet;
}
bool PolyMeshGraph::MarkAllForCleanUp(){
    for( auto & i : _GraphVertices ){
    	i.second->MarkForCleanUp();
    }
    for( auto & i : _GraphEdges ){
    	i.second->MarkForCleanUp();
    }
    for( auto & i : _GraphFaces ){
    	i.second->MarkForCleanUp();
    }
    return true;
}
bool PolyMeshGraph::CleanUp(){
    bool bRet = true;   

    for( auto i = _GraphVertices.begin(); i != _GraphVertices.end(); ){
	bool bCleanUp = false;	
	GetPolyMeshStatus( &i->second->_Status, PolyMeshStatus::CleanUp , &bCleanUp );
	if( bCleanUp ){
	    _GraphCleanUp.insert( std::pair< int, PolyMeshBase * >( i->second->_Id, i->second ) );
	    i = _GraphVertices.erase( i );
	}else{
	    ++i;
	}	
    }

    for( auto i = _GraphEdges.begin(); i != _GraphEdges.end(); ){
	bool bCleanUp = false;	
	GetPolyMeshStatus( &i->second->_Status, PolyMeshStatus::CleanUp , &bCleanUp );
	if( bCleanUp ){
	    _GraphCleanUp.insert( std::pair< int, PolyMeshBase * >( i->second->_Id, i->second ) );
	    i = _GraphEdges.erase( i );
	}else{
	    ++i;
	}	
    }

    for( auto i = _GraphFaces.begin(); i != _GraphFaces.end(); ){
	bool bCleanUp = false;	
	GetPolyMeshStatus( &i->second->_Status, PolyMeshStatus::CleanUp , &bCleanUp );
	if( bCleanUp ){
	    _GraphCleanUp.insert( std::pair< int, PolyMeshBase * >( i->second->_Id, i->second ) );
	    i = _GraphFaces.erase( i );
	}else{
	    ++i;
	}	
    }

    return true;
}

bool PolyMeshGraph::DeleteCleanUp(){

#ifdef DEBUG_POLYMESH_GRAPH
    cout << "Number of PolyMesh objects to delete: " << _GraphCleanUp.size() <<endl;
#endif
    
    for( auto i : _GraphCleanUp ){
#ifdef DEBUG_POLYMESH_GRAPH
	cout << "Deleted Type: " << i.second->_PolyType << ". Id: "<< i.second->_Id << endl;
#endif	
    	PolyMesh::Delete( i.second );
    }
    _GraphCleanUp.clear();
    return true;
}

bool PolyMeshGraph::GetFaces( list< PolyMeshFace * > & Faces ){
    for( auto & i : _GraphFaces ){
	Faces.push_back( i.second );
    }
    return true;
}
bool PolyMeshGraph::GetVertices( list< PolyMeshVertex * > & Vertices ){
    for( auto & i : _GraphVertices ){
	Vertices.push_back( i.second );
    }
    return true;
}
bool PolyMeshGraph::GetEdges( list< PolyMeshEdge * > & Edges ){
    for( auto & i : _GraphEdges ){
	Edges.push_back( i.second );
    }
    return true;
}
