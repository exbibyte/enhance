//==================================
//
// Summary:      Render Dynamic Mesh
// Notes:
// Dependencies: Vec, TransMatrix
//==================================
#ifndef POLYMESH_INTERFACE_H
#define POLYMESH_INTERFACE_H

#include "Vec.h"
#include "TransMatrix.h"
#include <vector>
#include <tuple>
#include <set>
#include <map>
using namespace std;

template< typename Impl >
class PolyMeshInterface : public Impl {
public:
    //delegated functions
    bool SetVertices( vector< Vec > & vVert, vector< set< int > > & vConnection ){
	return Impl::SetVertices( vVert, vConnection );
    }
    bool ChangeVertex( int iIndex, Vec v ){
	return Impl::ChangeVertex( iIndex, v );
    }
    bool Calculate(){
	return Impl::Calculate();
    }
    bool GetVertexToVertex_Single( int iVertex, set< int > & sVertex ){
	return Impl::GetVertexToVertex_Single( iVertex, sVertex );
    }
    bool GetFaceToVertex_Single( int iFace, set< int > & sVertex ){
	return Impl::GetFaceToVertex_Single( iFace, sVertex );
    }
    bool GetVertexToFace_Single( int iVertex, set< int > & sFace ){
	return Impl::GetVertexToFace_Single( iVertex, sFace );
    }
    bool GetVertexToVec_Single( int iVertex, Vec & vVec ){
	return Impl::GetVertexToVec_Single( iVertex, vVec );
    }
    bool GetFaceToEdge_Single( int iFace, set< int > & sEdge ){
	return Impl::GetFaceToEdge_Single( iFace, sEdge );
    }
    bool GetEdgeToFace_Single( int iEdge, set< int > & sFace ){
	return Impl::GetEdgeToFace_Single( iEdge, sFace );
    }   
    bool VertexToVertex( int iStart, int iDest, vector< int > & vPath ){
	return Impl::VertexToVertex( iStart, iDest, vPath );
    }
    bool EdgeToEdge( int iStart, int iDest, vector< int > & vPath ){
	return Impl::EdgeToEdge( iStart, iDest, vPath );
    }
    bool FaceToFace( int iStart, int iDest, vector< int > & vPath ){
	return Impl::FaceToFace( iStart, iDest, vPath );
    }
};

#endif
