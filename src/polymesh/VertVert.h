//==================================
//
// Summary:      Vertex-Vertex List
// Notes:
// Dependencies: Vec
//==================================
#ifndef POLYMESH_VERTVERT_H
#define POLYMESH_VERTVERT_H

#include "../Math/Vec.h"
#include <vector>
#include <tuple>

using namespace std;

class PolyMesh_VV{
public:
  enum eAccess_VV{
    IND = 0,
    VEC = 1,
    CON = 2
  };
  bool SetVertices( vector< Vec > & vVert, vector< vector< int > > & vConnection );
  bool GetVertices( vector< tuple< int, Vec, vector< int > > > & VV );
  bool ChangeVertex( int iIndex, Vec v );
  bool ChangeVertex( int iIndex, vector< int > vConnect );
  bool ChangeVertex( int iIndex, Vec v, vector< int > vConnect );
  bool GetVertex( int iIndex, Vec & v );
  bool CalcFaces(){ return false; }
private:
  vector< tuple< int, Vec, vector< int > > >  _VV;
};

#endif
