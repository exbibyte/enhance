//==================================
//
// Summary:      Vertex-Vertex List
// Notes:
// Dependencies: Vec, TransMatrix
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
  //input: list of vertices, list of adjacent vertices per vertex
  bool SetVertices( vector< Vec > & vVert, vector< vector< int > > & vConnection );
  //Get list of all vertices
  bool GetVertices( vector< tuple< int, Vec, vector< int > > > & VV );
  //Changes existing vertice and/or their adjacent connections
  bool ChangeVertex( int iIndex, Vec v );
  bool ChangeVertex( int iIndex, vector< int > vConnect );
  bool ChangeVertex( int iIndex, Vec v, vector< int > vConnect );
  //Get one vertex at an existing index
  bool GetVertex( int iIndex, Vec & v );
  //Computes faces list, vertex list, and vertices
  bool CalcFaces( vector< vector<int> > & vFaceList, vector< vector<int> > & vVerticeList, vector< Vec > & vVertices );
private:
  vector< tuple< int, Vec, vector< int > > >  _VV;
};

#endif
