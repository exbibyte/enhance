//==================================
//
// Summary:      Render Dynamic Mesh
// Notes:
// Dependencies: Vec, TransMatrix
//==================================
#ifndef POLYMESH_H
#define POLYMESH_H

#include "Vec.h"
#include <vector>
#include <tuple>
#include <set>
#include <map>
using namespace std;

class PolyMesh{
public:
    bool SetVertices( vector< Vec > & vVert, vector< set< int > > & vConnection );
    bool ChangeVertex( int iIndex, Vec v );
    bool Calculate();
    
    //get all elements:
    bool GetVertexIndices( set< int > & SetVertex );
    bool GetFaceIndices( set< int > & SetFace );
    bool GetEdgeIndices( set< int > & SetEdge );
    bool GetVec( vector< Vec > & vVec );

    //get all connections:
    bool GetVertexToVertex( map< int, set< int > > & MapVertexToVertex );
    bool GetFaceToVertex( map< int, set< int > > & MapFaceToVertex );
    bool GetVertexToFace( map< int, set< int > > & MapVertexToFace );
    bool GetVertexToVec( map< int, Vec > & MapVertexToVec );
    bool GetFaceToEdge( map< int, set< int > > & MapFaceToEdge );
    bool GetEdgeToFace( map< int, set< int > > & MapEdgeToFace );

    //get partial connections:
    bool GetVertexToVertex_Single( int iVertex, set< int > & sVertex );
    bool GetFaceToVertex_Single( int iFace, set< int > & sVertex );
    bool GetVertexToFace_Single( int iVertex, set< int > & sFace );
    bool GetVertexToVec_Single( int iVertex, Vec & vVec );
    bool GetFaceToEdge_Single( int iFace, set< int > & sEdge );
    bool GetEdgeToFace_Single( int iEdge, set< int > & sFace );
    
private:
    map< int, set< int > > _MapVertexVertex;     //stores all vertex-vertex connections
    map< int, Vec >  _MapVertexVec;        //stores all vertex-Vec 1 to 1 mapping
    map< int, set< int > > _MapFaceVertex; //stores all face-vertex connections
    map< int, set< int > > _MapVertexFace; //stores all vertex-face connections
    map< int, set< int > > _MapFaceEdge;   //stores all face-edge connections
    map< int, set< int > > _MapEdgeFace;   //stores all edge-face connections
    map< pair<int,int>, int > _MapVertexPairToEdge; //stores all vertex-pairs - edge connections
	
    set< int > _SetIndexVertex;            //stores all vertex indices
    set< int > _SetIndexFace;              //stores all face indices
    set< int > _SetIndexEdge;              //stores all edge indices
};

#endif
