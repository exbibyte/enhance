#ifndef POLYMESH_GRAPH_H
#define POLYMESH_GRAPH_H

#include "PolyMeshInterface.h"
#include "Vec.h"
#include <vector>
#include <set>
#include <list>
#include <map>
#include <tuple>

#define DEBUG_POLYMESH_GRAPH

class PolyMeshGraph {
public:
    /// Interface delegated by PolyMeshInterface class
    virtual ~PolyMeshGraph();
    bool SetVertices( std::vector< Vec > & vVert, std::vector< std::set< int > > & vConnection );
    bool CreatePolyMesh();
    bool WalkNeighbourFace( PolyMesh::PolyMeshVertex * PolyMeshObj, std::function<bool( PolyMesh::PolyMeshFace * )> Func );
    bool WalkNeighbourFace( PolyMesh::PolyMeshEdge * PolyMeshObj, std::function<bool( PolyMesh::PolyMeshFace * )> Func );
    bool WalkNeighbourVertex( PolyMesh::PolyMeshEdge * PolyMeshObj, std::function<bool( PolyMesh::PolyMeshVertex * )> Func );
    bool WalkNeighbourVertex( PolyMesh::PolyMeshFace * PolyMeshObj, std::function<bool( PolyMesh::PolyMeshVertex * )> Func );
    bool WalkNeighbourEdge( PolyMesh::PolyMeshVertex * PolyMeshObj, std::function<bool( PolyMesh::PolyMeshEdge * )> Func );
    bool WalkNeighbourEdge( PolyMesh::PolyMeshFace * PolyMeshObj, std::function<bool( PolyMesh::PolyMeshEdge * )> Func );
    bool GetLinkedPolyMeshById( PolyMesh::PolyMeshBase * PolyMeshObjSource, PolyMesh::PolyMeshType::Enum PolyType, int Id, PolyMesh::PolyMeshBase * & PolyMeshFound );
    bool WalkNeighbourVertexToVertexViaEdge( PolyMesh::PolyMeshVertex * PolyMeshSource, bool bEndOnTrue, std::function<bool( PolyMesh::PolyMeshVertex * PathVertex, PolyMesh::PolyMeshEdge * PathEdge )> Func );
    bool WalkToFindTriangle( PolyMesh::PolyMeshVertex * PolyMeshSource, std::map< std::tuple< int, int, int >, std::pair< std::list < PolyMesh::PolyMeshVertex * >, std::list < PolyMesh::PolyMeshEdge * > > >  & FoundTriangles );
    bool GetFaces( std::list< PolyMesh::PolyMeshFace * > & Faces );
    bool GetVertices( std::list< PolyMesh::PolyMeshVertex * > & Vertices );
    bool GetEdges( std::list< PolyMesh::PolyMeshEdge * > & Edges );
    
    bool CleanUp();
    bool DeleteCleanUp();

    //helper
    bool MarkAllForCleanUp();
private:

    /// mapping of PolyMesh _Id and data
    
    std::map< int, PolyMesh::PolyMeshFace * > _GraphFaces;
    std::map< int, PolyMesh::PolyMeshVertex * > _GraphVertices;
    std::map< int, PolyMesh::PolyMeshEdge * > _GraphEdges;

    /// CleanUp queue for deleting PolyMesh objects
    
    std::list< PolyMesh::PolyMeshBase * > _GraphCleanUp;
};

#endif
