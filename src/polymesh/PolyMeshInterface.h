//==================================
//
// Summary:      PolyMesh definitions and high level interfaces.
// Notes:
// Dependencies: Vec
//==================================
#ifndef POLYMESH_INTERFACE_H
#define POLYMESH_INTERFACE_H

#include "Vec.hpp"
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

    bool DisconnectPolyMeshObjectFrom( PolyMeshVertex * V_Source, PolyMeshEdge * E_Remove );

	/// Disconnects PolyMeshEdge from PolyMeshVertex
	///
	/// @param V_Source Source PolyMeshVertex.
	/// @param E_Remove Target PolyMeshEdge to remove from V_Source.
	///
	/// @returns True if successful.

    bool DisconnectPolyMeshObjectFrom( PolyMeshVertex * V_Source, PolyMeshFace * F_Remove );

	/// Disconnects PolyMeshFace from PolyMeshVertex
	///
	/// @param V_Source Source PolyMeshVertex.
	/// @param F_Remove Target PolyMeshFace to remove from V_Source.
	///
	/// @returns True if successful.

    bool DisconnectPolyMeshObjectFrom( PolyMeshEdge * E_Source, PolyMeshVertex * V_Remove );

	/// Disconnects PolyMeshVertex from PolyMeshEdge
	///
	/// @param E_Source Source PolyMeshEdge.
	/// @param V_Remove Target PolyMeshVertex to remove from E_Source.
	///
	/// @returns True if successful.

    bool DisconnectPolyMeshObjectFrom( PolyMeshEdge * E_Source, PolyMeshFace * F_Remove );

	/// Disconnects PolyMeshFace from PolyMeshEdge
	///
	/// @param E_Source Source PolyMeshEdge.
	/// @param F_Remove Target PolyMeshFace to remove from E_Source.
	///
	/// @returns True if successful.

    bool DisconnectPolyMeshObjectFrom( PolyMeshFace * F_Source, PolyMeshVertex * V_Remove );

	/// Disconnects PolyMeshVertex from PolyMeshFace
	///
	/// @param F_Source Source PolyMeshFace.
	/// @param V_Remove Target PolyMeshVertex to remove from F_Source.
	///
	/// @returns True if successful.

    bool DisconnectPolyMeshObjectFrom( PolyMeshFace * F_Source, PolyMeshEdge * E_Remove );

	/// Disconnects PolyMeshEdge from PolyMeshFace
	///
	/// @param F_Source Source PolyMeshFace.
	/// @param E_Remove Target PolyMeshEdge to remove from F_Source.
	///
	/// @returns True if successful.
    
    bool MarkForCleanUp( PolyMeshBase * Obj );
    
    bool Delete( PolyMeshBase * Obj );

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
	    for( auto i : _Vertices ){
		DisconnectPolyMeshObjectFrom( i, this );
	    }
	    for( auto i : _Edges ){
		DisconnectPolyMeshObjectFrom( i, this );
	    }
	    _Vertices.clear();
	    _Edges.clear();
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
	    for( auto i : _Edges ){
		DisconnectPolyMeshObjectFrom( i, this );
	    }
	    for( auto i : _Faces ){
		DisconnectPolyMeshObjectFrom( i, this );
	    }
	    _Edges.clear();
	    _Faces.clear();
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
	    for( auto i : _Vertices ){
		DisconnectPolyMeshObjectFrom( i, this );
	    }
	    for( auto i : _Faces ){
		DisconnectPolyMeshObjectFrom( i, this );
	    }
	    _Vertices.clear();
	    _Faces.clear();
	    return true;
	}
    };    
    
    template< typename Impl >
    class PolyMeshInterface : public Impl {
    public:
	~PolyMeshInterface(){}	
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
	bool DeleteCleanUp(){

	/// Delete all PolyMesh objects in the CleanUp queue.
	///
	/// @returns True if successful.

	    return Impl::DeleteCleanUp();
	}

	bool CleanUp(){

	/// Moves all PolyMesh objects that are marked for clean-up to a CleanUp queue.
	///
	/// @returns True if successful.
	    
	    return Impl::CleanUp();
	}	

    }; // end of PolyMeshInterface class
    
} // end of PolyMesh namespace

#endif
