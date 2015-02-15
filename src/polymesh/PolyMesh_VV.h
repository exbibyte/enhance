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
#include <set>
#include <map>
using namespace std;

class PolyMesh_VV{
public:
    bool SetVertices( vector< Vec > & vVert, vector< set< int > > & vConnection );
    bool ChangeVertex( int iIndex, Vec v );
    bool ChangeVertex( int iIndex, set< int > vConnect );
    bool ChangeVertex( int iIndex, Vec v, set< int > vConnect );
    bool GetVec( int iIndex, Vec & v );
    bool GetAllIndex( set< int > & index );
    bool GetAllVec( map< int, Vec > & v );
    bool GetAllConnection( map< int, set< int > > & c );
    bool GetConnection( int iIndex, set< int > & c );
    bool CalcFaces( map< int, set<int> > & MapFaceList, map< int, set<int> > & MapVertexList, map< int, Vec > & MapVec );     
private:
    map< int, set< int > > _MapConnect;    //stores all vertice connections
    map< int, Vec >  _MapVec;              //stores all Vec elements
    set< int > _SetIndex;                  //stores all vertex indexes
};

#endif
