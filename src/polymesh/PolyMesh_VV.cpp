#include "PolyMesh_VV.h"
#include "Vec.h"
#include "TransMatrix.h"

#include <vector>
#include <tuple>
#include <string>
#include <algorithm>
#include <set>
#include <iterator>
using namespace std;

bool PolyMesh_VV::SetVertices( vector< Vec > & vVert, vector< set< int > > & vConnection )
{
    if( vVert.size() != vConnection.size() ) //constraint check
	return false;
    _MapConnect.clear(); //reset data
    _MapVec.clear();
    _SetIndex.clear();
    for( int i = 0; i < vVert.size(); i++ ){
	for( auto j = vConnection[i].begin(); j != vConnection[i].end(); j++ ){
	    int iConnect = *j; 
	    if( iConnect  >= vVert.size() || iConnect == i || iConnect < 0 ){ // constraint check
		return false;
	    }
	}
	_MapConnect.emplace( i, vConnection[i] ); // save connections
	_MapVec.emplace( i, vVert[i] );		  // save vec elements
	_SetIndex.emplace( i );			  // save vertex indices
    }
    return true;
}

bool PolyMesh_VV::ChangeVertex( int iIndex, Vec v ){
    if( iIndex >= _MapVec.size() || iIndex < 0 )
	return false;
    _MapVec[ iIndex ] = v;
    return true;
}

bool PolyMesh_VV::ChangeVertex( int iIndex, set< int > vConnect ){
    if( iIndex >= _MapVec.size() || iIndex < 0 )
	return false;
    _MapConnect[ iIndex] = vConnect;
    return true;
}

bool PolyMesh_VV::ChangeVertex( int iIndex, Vec v, set< int > vConnect ){
    if( iIndex >= _MapVec.size() || iIndex < 0 )
	return false;
    _MapVec[ iIndex ] = v;
    _MapConnect[ iIndex ] = vConnect;
    return true;
}

bool PolyMesh_VV::GetVec( int iIndex, Vec & v ){
    if( iIndex >= _MapVec.size() || iIndex < 0 )
	return false;
    v = _MapVec[ iIndex ];
    return true;
}

bool PolyMesh_VV::GetAllIndex( set< int > & index ){
    index = _SetIndex;
    return true;
}

bool PolyMesh_VV::GetAllVec( map< int, Vec > & v ){
    v = _MapVec;
    return true;
}

bool PolyMesh_VV::GetAllConnection( map< int, set< int > > & c ){
    c = _MapConnect;
    return true;
}

bool PolyMesh_VV::GetConnection( int iIndex, set< int > & c ){
    c.clear();
    if( _MapConnect.find( iIndex ) == _MapConnect.end() )
	return false;
    
    std::copy( _MapConnect[ iIndex ].begin(), _MapConnect[ iIndex ].end(), std::inserter( c, c.begin() ) );
    return true;
}

bool PolyMesh_VV::CalcFaces( map< int, set<int> > & MapFaceList, map< int, set<int> > & MapVertexList, map< int, Vec > & MapVec )
{
    bool bRet = true;
    //clear results
    MapFaceList.clear();
    MapVertexList.clear();
    MapVec.clear();

    MapVec = _MapVec;   
    
    TransMatrix< int > Transivity;
    //save vertices into a remaining set and calculate closure
    set< int > SetRemainIndex;
    set< int > SetCompleteIndex;
    GetAllIndex( SetRemainIndex );
    for( auto i : SetRemainIndex ){
	int iIndexCurrent = i;
	int iIndexConnect;
	int iCost = 1; //default cost of each connect edge
	set< int > SetConnection;
	GetConnection( iIndexCurrent, SetConnection );
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
		//save vertex information into face list
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
		set< int > SetFaceVertices;
		SetFaceVertices.emplace( v1 );
		SetFaceVertices.emplace( v2 );
		SetFaceVertices.emplace( v3 );
		int iFaceIndex = MapFaceList.size();
		MapFaceList[ iFaceIndex ] = SetFaceVertices;
		//save face information into vertex list
		MapVertexList[ v1 ].emplace( iFaceIndex );
		MapVertexList[ v2 ].emplace( iFaceIndex );
		MapVertexList[ v3 ].emplace( iFaceIndex );
                //add traiangle vertices to the completed set
		SetCompleteIndex.emplace( v1 );
		SetCompleteIndex.emplace( v2 );
		SetCompleteIndex.emplace( v3 );
		if( SetCompleteIndex.size() == SetRemainIndex.size() ){
		    break;
		}
		//reset iterators
		it_A = SetRemainIndex.end();
		it_B = SetRemainIndex.end();
	    }
	}
	if( SetCompleteIndex.size() == SetRemainIndex.size() ){
	    break;
	}
    }

    if( SetCompleteIndex.size() != SetRemainIndex.size() )
	return false;
    
    return bRet;
}
