//==================================
//
// Summary:      PolyMesh definitions and high level interfaces.
// Notes:
// Dependencies: Vec
//==================================
#ifndef POLYMESH_INTERFACE_H
#define POLYMESH_INTERFACE_H

#include "Vec.h"
#include <list>
#include <set>
#include <vector>
#include <functional>
using namespace std;

namespace PolyMesh {

    class PolyMeshBase;
    class PolyMeshVertex;
    class PolyMeshEdge;
    class PolyMeshFace;
				    
    class PolyMeshStatus{
    public:
	/// PolyMesh status enumeration, used for bitwise offset index for PolyMesh status data.
	enum Enum {
	    NotInit = 0,
	    DirtyVertex,
	    DirtyFace,
	    DirtyEdge,
	    DirtyVec,
	    CleanUp,
	    Count
	};
    };

    class PolyMeshType{
    public:
	/// PolyMesh type enumeration.
	enum Enum {
	    Vertex = 0,
	    Edge,
	    Face,
	    Count
	};
    };   

    bool SetPolyMeshStatus( unsigned int * uiStatus, PolyMeshStatus::Enum const StatusType, bool const bSet );
	    
        /// Sets the provided PolyMeshStatus bit for the provided status data.
	///
	/// @param uiStatus   Provided status data to set.
	/// @param StatusType Provided status type to set.
	/// @param bVal       Provided bit value to set.
	///
	/// @returns          True if successful.
	
    bool GetPolyMeshStatus( unsigned int const * uiStatus, PolyMeshStatus::Enum const StatusType, bool * bVal );

        /// Gets the provided PolyMeshStatus bit of the provided status data.
	///
	/// @param uiStatus   Provided status data to check.
	/// @param StatusType Provided status type to check.
	/// @param bVal       Retrieved bit value.
	///
	/// @returns          True if successful.

    bool IsThisPolyMesh( PolyMeshBase * Obj, PolyMeshType::Enum const PolyType, int const Id );

        /// Checks the status of the PolyMesh object and returns true if the PolyMeshType and ID of the object matches the expected.
	///
	/// @param Obj      PolyMesh object to check.
	/// @param PolyType Expected PolyMeshType.
	/// @param Id       Expected id.
	///
	/// @returns        True if Vertex status is clear and Id matches the Vertex's _Id
    
    bool ConnectPolyMeshObjects( PolyMeshVertex * V, PolyMeshEdge * E );
    
	/// Connects PolyMeshVertex to PolyMeshEdge
	///
	/// @param V Target PolyMeshVertex.
	/// @param E Target PolyMeshEdge.
	///
	/// @returns True if successful.

    bool ConnectPolyMeshObjects( PolyMeshVertex * V, PolyMeshFace * F );

        /// Connects PolyMeshVertex to PolyMeshFace
	///
	/// @param V Target PolyMeshVertex.
	/// @param F Target PolyMeshFace.
	///
	/// @returns True if successful.

    bool ConnectPolyMeshObjects( PolyMeshEdge * E, PolyMeshFace * F );

        /// Connects PolyMeshVertex to PolyMeshFace
	///
	/// @param E Target PolyMeshEdge.
	/// @param F Target PolyMeshFace.
	///
	/// @returns True if successful.

    bool DisconnectPolyMeshObjects( PolyMeshVertex * V, PolyMeshEdge * E );

	/// Disconnects PolyMeshVertex to PolyMeshEdge
	///
	/// @param V Target PolyMeshVertex.
	/// @param E Target PolyMeshEdge.
	///
	/// @returns True if successful.

    bool DisconnectPolyMeshObjects( PolyMeshVertex * V, PolyMeshFace * F );

        /// Disconnects PolyMeshVertex to PolyMeshFace
	///
	/// @param V Target PolyMeshVertex.
	/// @param F Target PolyMeshFace.
	///
	/// @returns True if successful.

    bool DisconnectPolyMeshObjects( PolyMeshEdge * E, PolyMeshFace * F );
    
        /// Disconnects PolyMeshVertex to PolyMeshFace
	///
	/// @param E Target PolyMeshEdge.
	/// @param F Target PolyMeshFace.
	///
	/// @returns True if successful.

    bool MarkForCleanUp( PolyMeshBase * Obj );
    
    bool CleanUp( PolyMeshBase * Obj );

    class PolyMeshBase {
    public:
	virtual ~PolyMeshBase(){};
	PolyMeshBase( int const Id, PolyMeshType::Enum const PolyType ) : _Id( Id ), _PolyType( PolyType ), _Status( 0 ) {}
	int _Id;                /// Id of the specific PolyMeshType object
	unsigned int _Status;   /// status of the PolyMesh
	PolyMeshType::Enum _PolyType; /// type of the object
	virtual bool MarkForCleanUp(){ return false; };
    };

    class PolyMeshFace : public PolyMeshBase {
    public:
	~PolyMeshFace(){};
	PolyMeshFace( int const Id ) : PolyMeshBase( Id, PolyMeshType::Face ){}
	set< PolyMeshVertex * > _Vertices;
	set< PolyMeshEdge * > _Edges;
	bool MarkForCleanUp(){
	    if( !SetPolyMeshStatus( &this->_Status, PolyMeshStatus::CleanUp, true ) ){
		return false;
	    }
	    for( auto & i : _Vertices ){
		DisconnectPolyMeshObjects( i, this );
	    }
	    for( auto & i : _Edges ){
		DisconnectPolyMeshObjects( i, this );
	    }
	    return true;
	};
    };
    class PolyMeshVertex : public PolyMeshBase {
    public:
	~PolyMeshVertex(){};
	PolyMeshVertex( int const Id, Vec VecData ) : PolyMeshBase( Id, PolyMeshType::Vertex ), _Vec( VecData ) {}
	Vec _Vec;
	set< PolyMeshFace * > _Faces;
	set< PolyMeshEdge * > _Edges;
	bool MarkForCleanUp(){
	    if( !SetPolyMeshStatus( &this->_Status, PolyMeshStatus::CleanUp, true ) ){
		return false;
	    }
	    for( auto & i : _Faces ){
		DisconnectPolyMeshObjects( this, i );
	    }
	    for( auto & i : _Edges ){
		DisconnectPolyMeshObjects( this, i );
	    }
	    return true;
	}
    };
    class PolyMeshEdge : public PolyMeshBase {
    public:
	~PolyMeshEdge(){};
	PolyMeshEdge( int const Id ) : PolyMeshBase( Id, PolyMeshType::Edge ){}
	set< PolyMeshVertex * > _Vertices;
	set< PolyMeshFace * > _Faces;
	bool MarkForCleanUp(){
	    if( !SetPolyMeshStatus( &this->_Status, PolyMeshStatus::CleanUp, true ) ){
		return false;
	    }
	    for( auto & i : _Faces ){
		DisconnectPolyMeshObjects( this, i );
	    }
	    for( auto & i : _Vertices ){
		DisconnectPolyMeshObjects( i, this );
	    }
	    return true;
	}
    };    
    
    template< typename Impl >
    class PolyMeshInterface : public Impl {
    public:
	bool SetVertices( vector< Vec > & vVert, vector< set< int > > & vConnection ){

	/// Initialization of vertex data.
	///
	/// @param vVert       Vector of vertex data.
	/// @param vConnection Vector of sets of connections for each vertex.
	///
	/// @returns           True if successful.
	    
	    return Impl::SetVertices( vVert, vConnection );
	}
	bool CreatePolyMesh(){

	/// Creation of PolyMesh data structure from initilized vertex data by SetVertices().
	///
	/// @returns           True if successful.
	    
	    return Impl::CreatePolyMesh();
	}
	bool WalkNeighbourFace( PolyMeshVertex * PolyMeshObj, std::function<bool( PolyMeshFace * )> Func ){

        /// Operates a function on all neightbouring faces of the PolyMeshObj.
	///
	/// @param PolyMeshObj PolyMeshVertex object of interest.
	/// @param Func        Function to operate on each neighbouring PolyMeshFace.
	///
	/// @returns           True if success.
	    
	    return Impl::GetNeighbourFace( PolyMeshObj, Func );
	}
	bool WalkNeighbourFace( PolyMeshEdge * PolyMeshObj, std::function<bool( PolyMeshFace * )> Func ){

        /// Operates a function on all neightbouring faces of the PolyMeshObj.
	///
	/// @param PolyMeshObj PolyMeshEdge object of interest.
	/// @param Func        Function to operate on each neighbouring PolyMeshFace.
	///
	/// @returns           True if success.
	    
	    return Impl::GetNeighbourFace( PolyMeshObj, Func );
	}
	bool WalkNeighbourVertex( PolyMeshEdge * PolyMeshObj, std::function<bool( PolyMeshVertex * )> Func ){

        /// Operates a function on all neightbouring vertices of the PolyMeshObj.
	///
	/// @param PolyMeshObj PolyMeshEdge object of interest.
	/// @param Func        Function to operate on each neighbouring PolyMeshVertex.
	///
	/// @returns           True if success.
	    
	    return Impl::GetNeighbourVertex( PolyMeshObj, Func );
	}
	bool WalkNeighbourVertex( PolyMeshFace * PolyMeshObj, std::function<bool( PolyMeshVertex * )> Func ){

        /// Operates a function on all neightbouring vertices of the PolyMeshObj.
	///
	/// @param PolyMeshObj PolyMeshFace object of interest.
	/// @param Func        Function to operate on each neighbouring PolyMeshVertex.
	///
	/// @returns           True if success.
	    
	    return Impl::GetNeighbourVertex( PolyMeshObj, Func );
	}
	bool WalkNeighbourEdge( PolyMeshVertex * PolyMeshObj, std::function<bool( PolyMeshEdge * )> Func ){

        /// Operates a function on all neightbouring edges of the PolyMeshObj.
	///
	/// @param PolyMeshObj PolyMeshVertex object of interest.
	/// @param Func        Function to operate on each neighbouring PolyMeshEdge.
	///
	/// @returns           True if success.
	    
	    return Impl::GetNeighbourEdge( PolyMeshObj, Func );
	}
	bool WalkNeighbourEdge( PolyMeshFace * PolyMeshObj, std::function<bool( PolyMeshEdge * )> Func ){

        /// Operates a function on all neightbouring edges of the PolyMeshObj.
	///
	/// @param PolyMeshObj PolyMeshFace object of interest.
	/// @param Func        Function to operate on each neighbouring PolyMeshEdge.
	///
	/// @returns           True if success.
	    
	    return Impl::GetNeighbourEdge( PolyMeshObj, Func );
	}
	bool WalkNeighbourVertexToVertexViaEdge( PolyMeshVertex * PolyMeshSource, std::function<bool( PolyMeshVertex * & Neighbour, PolyMeshEdge * & PathEdge )> Func ){

        /// Gets a non-trivial path from vertex to vertex through edges
	///
	/// @param PolyMeshSource  Source PolyMeshVetex.
	/// @param PolyMeshDest    Destination PolyMeshVetex.
	/// @param PathVertices    An ordered sequence of PolyMeshVertex travelled (includes the starting source)
	/// @param PathEdges       An ordered sequence of PolyMeshEdge travelled.
	///
	/// @returns               True if such a path exists.
     
	    return Impl::WalkNeighbourVertexToVertexViaEdge( PolyMeshSource, Func );
	}
	bool GetLinkedPolyMeshById( PolyMeshBase * PolyMeshObjSource, PolyMeshType::Enum PolyType, int Id, PolyMeshBase * & PolyMeshFound ){

	/// Gets a linked PolyMesh object to the provided PolyMesh object if it exists.
	///
	/// @param PolyMeshObjSource Provided PolyMesh object.
	/// @param PolyType          Type of PolyMesh to search.
	/// @param Id                Id of PolyMesh to search.
	/// @param PolyMeshFound     Found PolyMesh object, if possible.
	///
	/// @returns                 True if PolyMesh object found.

	    return Impl::GetLinkedPolyMeshById( PolyMeshObjSource, PolyType, Id, PolyMeshFound );		
	}
	bool GetFaces( list< PolyMeshFace * > & Faces ){
	    
	/// Gets a list of faces.
	///
	/// @param Faces List of returning PolyMeshFaces.
	///
	/// @returns     True if successful.

	    return Impl::GetFaces( Faces );
	}
	bool GetVertices( list< PolyMeshVertex * > & Vertices ){
	    
	/// Gets a list of vertices.
	///
	/// @param Vertices List of returning PolyMeshVertex.
	///
	/// @returns     True if successful.

	    return Impl::GetVertices( Vertices );
	}
	bool GetEdges( list< PolyMeshEdge * > & Edges ){
	    
	/// Gets a list of edges.
	///
	/// @param Edges List of returning PolyMeshEdge.
	///
	/// @returns     True if successful.

	    return Impl::GetEdges( Edges );
	}
    }; // end of PolyMeshInterface class
    
} // end of PolyMesh namespace

#endif
