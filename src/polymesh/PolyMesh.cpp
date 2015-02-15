#include "PolyMesh.h"
#include "Vec.h"
#include "TransMatrix.h"

#include <vector>
#include <tuple>
#include <string>
#include <algorithm>
#include <set>
#include <iterator>
using namespace std;

bool PolyMesh::SetVertices( vector< Vec > & vVert, vector< set< int > > & vConnection )
{
    if( vVert.size() != vConnection.size() ) //constraint check
	return false;
    //reset data
    _MapVertexVertex.clear();
    _MapVertexVec.clear();
    _MapFaceVertex.clear();
    _MapVertexFace.clear();
    _MapFaceEdge.clear();
    _MapEdgeFace.clear();
    
    _SetIndexVertex.clear();
    _SetIndexFace.clear();
    _SetIndexEdge.clear();
    
    for( int i = 0; i < vVert.size(); i++ ){
	for( auto j = vConnection[i].begin(); j != vConnection[i].end(); j++ ){
	    int iConnect = *j; 
	    if( iConnect  >= vVert.size() || iConnect == i || iConnect < 0 ){ // constraint check
		return false;
	    }
	}
	//insert data
	_MapVertexVertex.insert( make_pair(i, vConnection[i]) );
	_MapVertexVec.insert( make_pair(i, vVert[i]) );
   
	_SetIndexVertex.insert( i );		
    }
    return true;
}
bool PolyMesh::ChangeVertex( int iIndex, Vec v ){
    if( iIndex >= _MapVertexVec.size() || iIndex < 0 )
	return false;
    _MapVertexVec[ iIndex ] = v;
    return true;
}
bool PolyMesh::Calculate()
{
    bool bRet = true;

    //clear results
    _MapFaceVertex.clear();
    _MapVertexFace.clear();
    _MapFaceEdge.clear();
    _MapEdgeFace.clear();
    _MapVertexPairToEdge.clear();
    
    _SetIndexFace.clear();
    _SetIndexEdge.clear();
    
    TransMatrix< int > Transivity;
    //save vertices into a remaining set and calculate closure
    set< int > SetRemainIndex = _SetIndexVertex;
    set< int > SetCompleteIndex;
    for( auto i : SetRemainIndex ){
	int iIndexCurrent = i;
	int iIndexConnect;
	int iCost = 1; //default cost of each connect edge
	set< int > SetConnection;
	GetVertexToVertex_Single( iIndexCurrent, SetConnection );
	for( auto j : SetConnection ){
	    iIndexConnect = j;
	    Transivity.SetTransition( iIndexCurrent, iIndexConnect, iCost );
	    Transivity.SetTransition( iIndexConnect, iIndexCurrent, iCost );
	}
    }
    Transivity.UpdateClosure();
    //search remaining vertex set for triangles
    for( auto i = SetRemainIndex.begin(); i != SetRemainIndex.end(); i++ ){
	auto it_A = SetRemainIndex.end();
	auto it_B = SetRemainIndex.end();
	for( auto j = SetRemainIndex.begin(); j != SetRemainIndex.end(); j++ ){
	    int iIndexStart = *i;
	    int iIndexEnd = *j;
	    int iCost;
	    vector < int > vPath;
	    if( iIndexStart == iIndexEnd )
		continue;
	    bool bClosed = Transivity.GetClosure( iIndexStart, iIndexEnd, iCost, vPath );
	    if( bClosed && iCost == 1 ){
		//found adjacent vertices, save it
		if( it_A == SetRemainIndex.end() )
		    it_A = j;
		else if( it_B == SetRemainIndex.end() )
		    it_B = j;
	    }	 
	    if( it_A != SetRemainIndex.end() && it_B != SetRemainIndex.end() ){
		//found triangle
		int v1 = *i;
		int v2 = *it_A;
		int v3 = *it_B;
		if( SetCompleteIndex.find(v1) != SetCompleteIndex.end() && // checks for repitition
		    SetCompleteIndex.find(v2) != SetCompleteIndex.end() &&
		    SetCompleteIndex.find(v3) != SetCompleteIndex.end() ){
		    it_A = SetRemainIndex.end();
		    it_B = SetRemainIndex.end();
		    continue;
		}
		//save face-vertex data
		set< int > SetFaceVertices;
		SetFaceVertices.insert( v1 );
		SetFaceVertices.insert( v2 );
		SetFaceVertices.insert( v3 );
		int iFaceIndex = _SetIndexFace.size();
		_MapFaceVertex[ iFaceIndex ] = SetFaceVertices;
		//save face index
		_SetIndexFace.insert( iFaceIndex );
		//save vertex-face data
		_MapVertexFace[ v1 ].insert( iFaceIndex );
		_MapVertexFace[ v2 ].insert( iFaceIndex );
		_MapVertexFace[ v3 ].insert( iFaceIndex );
		//prepate edge data for saving
		vector< int > vVertices;
		for( auto vert : SetFaceVertices ){
		    vVertices.push_back( vert );
		}
		//assume data in vector is ordered since it is copied from an ordered set
		std::pair<int,int> e1 = make_pair( vVertices[0], vVertices[1] );
		std::pair<int,int> e2 = make_pair( vVertices[0], vVertices[2] );
		std::pair<int,int> e3 = make_pair( vVertices[1], vVertices[2] );
		vector< std::pair<int,int> > vEdges;
		vEdges.push_back( e1 );
		vEdges.push_back( e2 );
		vEdges.push_back( e3 );
		for( auto e : vEdges ){
		    auto it_edge = _MapVertexPairToEdge.find( e );
		    //check if the edge is already registered
		    if( it_edge == _MapVertexPairToEdge.end() ){
			int iEdgeIndex = _SetIndexEdge.size();
			_SetIndexEdge.insert( iEdgeIndex );
			_MapVertexPairToEdge[ e ] = iEdgeIndex;
			it_edge = _MapVertexPairToEdge.find( e );
		    }
		    //save face-edge data
		    _MapFaceEdge[ iFaceIndex ].insert( it_edge->second );
		    //save edge-face data
		    _MapEdgeFace[ it_edge->second ].insert( iFaceIndex );
		}							     
                //add vertices to the completed set
		SetCompleteIndex.insert( v1 );
		SetCompleteIndex.insert( v2 );
		SetCompleteIndex.insert( v3 );
		//check if found all
		if( SetCompleteIndex.size() == SetRemainIndex.size() ){
		    break;
		}
		//reset iterators
		it_A = SetRemainIndex.end();
		it_B = SetRemainIndex.end();
	    }
	}
	//check if found all
	if( SetCompleteIndex.size() == SetRemainIndex.size() ){
	    break;
	}
    }

    if( SetCompleteIndex.size() != SetRemainIndex.size() )
	return false;
    
    return bRet;
}
bool PolyMesh::GetVertexIndices( set< int > & SetVertex ){
    SetVertex = _SetIndexVertex;
    return true;
}
bool PolyMesh::GetFaceIndices( set< int > & SetFace ){
    SetFace = _SetIndexFace;
    return true;
}
bool PolyMesh::GetEdgeIndices( set< int > & SetEdge ){
    SetEdge = _SetIndexEdge;
    return true;
}
bool PolyMesh::GetVec( vector< Vec > & vVec ){
    vVec.clear();
    for( auto i : _MapVertexVec ){
	vVec.push_back( i.second );
    }
    return true;
}
bool PolyMesh::GetVertexToVertex( map< int, set< int > > & MapVertexToVertex ){
    MapVertexToVertex = _MapVertexVertex;
    return true;
}
bool PolyMesh::GetFaceToVertex( map< int, set< int > > & MapFaceToVertex ){
    MapFaceToVertex = _MapFaceVertex;
    return true;
}
bool PolyMesh::GetVertexToFace( map< int, set< int > > & MapVertexToFace ){
    MapVertexToFace = _MapVertexFace;
    return true;
}
bool PolyMesh::GetVertexToVec( map< int, Vec > & MapVertexToVec ){
    MapVertexToVec = _MapVertexVec;
    return true;
}
bool PolyMesh::GetFaceToEdge( map< int, set< int > > & MapFaceToEdge ){
    MapFaceToEdge = _MapFaceEdge;
    return true;
}
bool PolyMesh::GetEdgeToFace( map< int, set< int > > & MapEdgeToFace ){
    MapEdgeToFace = _MapEdgeFace;
    return true;
}
bool PolyMesh::GetVertexToVertex_Single( int iVertex, set< int > & sVertex ){
    auto i = _MapVertexVertex.find( iVertex );
    if( i == _MapVertexVertex.end() )
	return false;
    sVertex = i->second;
    return true;
}
bool PolyMesh::GetFaceToVertex_Single( int iFace, set< int > & sVertex ){
    auto i = _MapFaceVertex.find( iFace );
    if( i == _MapFaceVertex.end() )
	return false;
    sVertex = i->second;
    return true;
}
bool PolyMesh::GetVertexToFace_Single( int iVertex, set< int > & sFace ){
    auto i = _MapVertexFace.find( iVertex );
    if( i == _MapVertexFace.end() )
	return false;
    sFace = i->second;;
    return true;
}
bool PolyMesh::GetVertexToVec_Single( int iVertex, Vec & vVec ){
    auto i = _MapVertexVec.find( iVertex );
    if( i == _MapVertexVec.end() )
	return false;
    vVec = i->second;
    return true;
}
bool PolyMesh::GetFaceToEdge_Single( int iFace, set< int > & sEdge ){
    auto i = _MapFaceEdge.find( iFace );
    if( i == _MapFaceEdge.end() )
	return false;
    sEdge = i->second;
    return true;
}
bool PolyMesh::GetEdgeToFace_Single( int iEdge, set< int > & sFace ){
    auto i = _MapEdgeFace.find( iEdge );
    if( i == _MapEdgeFace.end() )
	return false;
    sFace = i->second;
    return true;
}
