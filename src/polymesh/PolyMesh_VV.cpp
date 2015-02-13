#include "PolyMesh_VV.h"
#include "Vec.h"
#include "TransMatrix.h"

#include <vector>
#include <tuple>
#include <string>
#include <utlity>
#include <algorithm>
using namespace std;

bool PolyMesh_VV::SetVertices( vector< Vec > & vVert, vector< vector< int > > & vConnection )
{
  if( vVert.size() != vConnection.size() ) //constraint check
    return false;
  _VV.clear();
  for( int i = 0; i < vVert.size(); i++ ){
    for( int j = 0; j < vConnection[i].size(); j++ ){
      int iConnect = vConnection[i][j];
      if( iConnect  >= vVert.size() || iConnect == i ){ // constraint check
	return false;
      }
    } 
    auto a = std::make_tuple( i, vVert[i], vConnection[i] );
    _VV.push_back( a );
  }
  return true;
}

bool PolyMesh_VV::GetVertices( vector< tuple< int, Vec, vector< int > > > & VV )
{
  VV = _VV;
  return true;
}

bool PolyMesh_VV::ChangeVertex( int iIndex, Vec v ){
  if( iIndex >= _VV.size() )
    return false;
  std::get<VEC>( _VV[ iIndex ] ) = v;
  return true;
}

bool PolyMesh_VV::ChangeVertex( int iIndex, vector< int > vConnect ){
  if( iIndex >= _VV.size() )
    return false;
  std::get<CON>( _VV[ iIndex ] ) = vConnect;
  return true;
}

bool PolyMesh_VV::ChangeVertex( int iIndex, Vec v, vector< int > vConnect ){
  if( iIndex >= _VV.size() )
    return false;
  std::get<VEC>( _VV[ iIndex ] ) = v;
  std::get<CON>( _VV[ iIndex ] ) = vConnect;
  return true;
}

bool PolyMesh_VV::GetVertex( int iIndex, Vec & v ){
  if( iIndex >= _VV.size() )
    return false;
  v = std::get<VEC>( _VV[ iIndex ] );
  return true;
}

bool PolyMesh_VV::CalcFaces( vector< vector<int> > & vFaceList, vector< vector<int> > & vVerticeList, vector< Vec > & vVertices )
{
  TransMatrix< int > _Transivity;
  vVertices.clear();

  set< pair< int, int > > SetPairVertices; // stores all vertice pairs
  
  for( auto i : _VV ){
    vVertices.push_back( std::get<VEC>( i ) );   //copy vertices
    for( auto j : std::get<CON>( i ) ){
      if( i <= j ){ //make the vertice pair ordered
	SetPairVertices.insert( std::make_pair( i, j ) ); // add to the set of vertice pairs
      }else{
	SetPairVertices.insert( std::make_pair( j, i ) );
      }
    }				
  }

  std::for_each( SetPairVertices.begin(), SetPairVertices.end() [&]( std::set< pair< int, int > >::iterator & it ){ _Transivity.SetTransition( it->first, it->second, 1 ); } );
  //use transitivity to find closed path from vertex connections
  
}
