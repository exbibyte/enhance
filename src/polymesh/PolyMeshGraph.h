#ifndef POLYMESH_GRAPH_H
#define POLYMESH_GRAPH_H

#include "PolyMeshInterface.h"
#include "Vec.h"
#include <vector>
#include <set>
#include <list>
#include <map>

class PolyMeshGraph {
public:
    /// Interface delegated by PolyMeshInterface class
    bool SetVertices( std::vector< Vec > & vVert, std::vector< std::set< int > > & vConnection );
    bool CreatePolyMesh();
    bool WalkNeighbourFace( PolyMesh::PolyMeshVertex * PolyMeshObj, std::function<bool( PolyMesh::PolyMeshVertex * )> Func );
    bool WalkNeighbourFace( PolyMesh::PolyMeshEdge * PolyMeshObj, std::function<bool( PolyMesh::PolyMeshVertex * )> Func );
    bool WalkNeighbourVertex( PolyMesh::PolyMeshEdge * PolyMeshObj, std::function<bool( PolyMesh::PolyMeshVertex * )> Func );
    bool WalkNeighbourVertex( PolyMesh::PolyMeshFace * PolyMeshObj, std::function<bool( PolyMesh::PolyMeshVertex * )> Func );
    bool WalkNeighbourEdge( PolyMesh::PolyMeshVertex * PolyMeshObj, std::function<bool( PolyMesh::PolyMeshVertex * )> Func );
    bool WalkNeighbourEdge( PolyMesh::PolyMeshFace * PolyMeshObj, std::function<bool( PolyMesh::PolyMeshVertex * )> Func );
    bool GetLinkedPolyMeshById( PolyMesh::PolyMeshBase * PolyMeshObjSource, PolyMesh::PolyMeshType::Enum PolyType, int Id, PolyMesh::PolyMeshBase * & PolyMeshFound );
    bool WalkNeighbourVertexToVertexViaEdge( PolyMesh::PolyMeshVertex * PolyMeshSource, std::function<bool( PolyMesh::PolyMeshVertex * & Neighbour, PolyMesh::PolyMeshEdge * & PathEdge )> Func );
    /// helper methods
    bool MarkForCleanUpAll();
    bool CleanUpAll();
private:
    std::map< int, PolyMesh::PolyMeshFace * > _GraphFaces;
    std::map< int, PolyMesh::PolyMeshVertex * > _GraphVertices;
    std::map< int, PolyMesh::PolyMeshEdge * > _GraphEdges;
};

#endif
