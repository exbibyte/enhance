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
    bool SetVertices( std::vector< Vec > & vVert, std::vector< std::set< int > > & vConnection );
    bool CreatePolyMesh();
    bool GetNeighbourFace( PolyMesh::PolyMeshBase * PolyMeshObj, std::set< PolyMesh::PolyMeshFace * > & sFace );
    bool GetNeighbourVertex( PolyMesh::PolyMeshBase * PolyMeshObj, std::set< PolyMesh::PolyMeshVertex * > & sVertex );
    bool GetNeighbourEdge( PolyMesh::PolyMeshBase * PolyMeshObj, std::set< PolyMesh::PolyMeshEdge * > & sEdge );
    bool GetLinkedPolyMeshById( PolyMesh::PolyMeshBase * PolyMeshObjSource, PolyMesh::PolyMeshType::Enum PolyType, int Id, PolyMesh::PolyMeshBase * & PolyMeshFound );

private:
    std::map< int, PolyMesh::PolyMeshFace * > _Faces;
    std::map< int, PolyMesh::PolyMeshVertex * > _Vertices;
    std::map< int, PolyMesh::PolyMeshEdge * > _Edges;
};

#endif
