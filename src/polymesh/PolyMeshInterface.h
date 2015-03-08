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
using namespace std;

namespace PolyMesh {

    class PolyMeshStatus{
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
	/// PolyMesh type enumeration.
	enum Enum {
	    Vertex = 0,
	    Edge,
	    Face,
	    Count
	};
    };
    
    bool SetPolyMeshStatus( unsigned int * uiStatus, PolyMeshStatus::Enum const StatusType, bool const bSet ){

        /// Sets the provided PolyMeshStatus bit for the provided status data.
	///
	/// @param uiStatus   Provided status data to set.
	/// @param StatusType Provided status type to set.
	/// @param bVal       Provided bit value to set.
	///
	/// @returns          True if successful.
	
	switch( StatusType ){
	case Count:
	    return false;
	default:
	    *uiStatus = bSet << StatusType;
	    break;
	}
	return true;
    }
    bool GetPolyMeshStatus( unsigned int const * uiStatus, PolyMeshStatus::Enum const StatusType, bool * bVal ){

        /// Gets the provided PolyMeshStatus bit of the provided status data.
	///
	/// @param uiStatus   Provided status data to check.
	/// @param StatusType Provided status type to check.
	/// @param bVal       Retrieved bit value.
	///
	/// @returns          True if successful.
       
	switch( StatusType ){
	case Count:
	    return false;
	default:
	    unsigned int iTemp = ( uiStatus >> StatusType ) & 1;
	    *bVal = ( iTemp == 1 );
	}
	return true;
    }
    bool IsThisPolyMesh( PolyMeshBase * Obj, PolyMeshType::Enum const PolyType, int const Id ){

        /// Checks the status of the PolyMesh object and returns true if the PolyMeshType and ID of the object matches the expected.
	///
	/// @param Obj      PolyMesh object to check.
	/// @param PolyType Expected PolyMeshType.
	/// @param Id       Expected id.
	///
	/// @returns        True if Vertex status is clear and Id matches the Vertex's _Id
	
	bool bStatusNotInit;
	if( !GetPolyMeshStatus( Obj->_Status, PolyMeshStatus::NotInit, &bStatusNotInit ) ){
	    return false;
	}else{
	    if( bStatusNotInit == false ){
		return  ( PolyType == Obj->_PolyType ) && ( Id == Obj->_Id );
	    }else{
		return false;
	    }
	}       
    }

    class PolyMeshBase {
    pubic:
	virtual ~PolyMeshBase(){};
	PolyMeshBase( int const Id, PolyMeshType::Enum const PolyType ) : _Id( Id ), _PolyType( PolyType ), _Status( 0 ) {}
	int _Id;                /// Id of the specific PolyMeshType object
	unsigned int _Status;   /// status of the PolyMesh
	PolyMeshType _PolyType; /// type of the object
	virtual bool MarkForCleanUp(){};
    };
    class PolyMeshFace : PolyMeshBase {
    public:
	~PolyMeshFace();
	PolyMeshFace( int const Id ) : PolyMeshBase( Id, PolyMeshType::Face ){}
	set< PolyMeshVertex * > _Vertices;
	set< PolyMeshEdge * > _Edges;
	bool MarkForCleanUp(){
	    if( !SetPolyMeshStatus( this->_Status, PolyMeshStatus::CleanUp, true ) ){
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
    class PolyMeshVertex : PolyMeshBase {
    public:
	~PolyMeshVertex();
	PolyMeshVertex( int const Id ) : PolyMeshBase( Id, PolyMeshType::Vertex ){}
	Vec * _Vec;
	set< PolyMeshFace * > _Faces;
	set< PolyMeshEdge * > _Edges;
	bool MarkForCleanUp(){
	    if( !SetPolyMeshStatus( this->_Status, PolyMeshStatus::CleanUp, true ) ){
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
    class PolyMeshEdge : PolyMeshBase {
    pubic:
	~PolyMeshEdge();
	PolyMeshEdge( int const Id ) : PolyMeshBase( Id, PolyMeshType::Edge ){}
	set< PolyMeshVertex * > _Vertices;
	set< PolyMeshFace * > _Faces;
	bool MarkForCleanUp(){
	    if( !SetPolyMeshStatus( this->_Status, PolyMeshStatus::CleanUp, true ) ){
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

    bool ConnectPolyMeshObjects( PolyMeshVertex * V, PolyMeshEdge * E ){

	/// Connects PolyMeshVertex to PolyMeshEdge
	///
	/// @param V Target PolyMeshVertex.
	/// @param E Target PolyMeshEdge.
	///
	/// @returns True if successful.

	if( nullptr == V || nullptr == E ){
	    return false;
	}
	V->_Edges.insert( E );
	E->_Vertices.insert( V );
	return true;
    }
    bool ConnectPolyMeshObjects( PolyMeshVertex * V, PolyMeshFace * F ){

        /// Connects PolyMeshVertex to PolyMeshFace
	///
	/// @param V Target PolyMeshVertex.
	/// @param F Target PolyMeshFace.
	///
	/// @returns True if successful.
	
	if( nullptr == V || nullptr == F ){
	    return false;
	}
	V->_Faces.insert( F );
	F->_Vertices.insert( V );
	return true;
    }
    bool ConnectPolyMeshObjects( PolyMeshEdge * E, PolyMeshFace * F ){

        /// Connects PolyMeshVertex to PolyMeshFace
	///
	/// @param E Target PolyMeshEdge.
	/// @param F Target PolyMeshFace.
	///
	/// @returns True if successful.

	if( nullptr == E || nullptr == F ){
	    return false;
	}
	E->_Faces.insert( F );
	F->_Edges.insert( E );
	return true;
    }
    bool DisconnectPolyMeshObjects( PolyMeshVertex * V, PolyMeshEdge * E ){

	/// Disconnects PolyMeshVertex to PolyMeshEdge
	///
	/// @param V Target PolyMeshVertex.
	/// @param E Target PolyMeshEdge.
	///
	/// @returns True if successful.

	if( nullptr == V || nullptr == E ){
	    return false;
	}
	V->_Edges.erase( E );
	E->_Vertices.erase( V );
	return true;
    }
    bool DisconnectPolyMeshObjects( PolyMeshVertex * V, PolyMeshFace * F ){

        /// Disconnects PolyMeshVertex to PolyMeshFace
	///
	/// @param V Target PolyMeshVertex.
	/// @param F Target PolyMeshFace.
	///
	/// @returns True if successful.
	
	if( nullptr == V || nullptr == F ){
	    return false;
	}
	V->_Faces.erase( F );
	F->_Vertices.erase( V );
	return true;
    }
    bool DisconnectPolyMeshObjects( PolyMeshEdge * E, PolyMeshFace * F ){

        /// Disconnects PolyMeshVertex to PolyMeshFace
	///
	/// @param E Target PolyMeshEdge.
	/// @param F Target PolyMeshFace.
	///
	/// @returns True if successful.

	if( nullptr == E || nullptr == F ){
	    return false;
	}
	E->_Faces.erase( F );
	F->_Edges.erase( E );
	return true;
    }
    bool MarkForCleanUp( PolyMeshBase * Obj ){
	if( Obj == nullptr ){
	    return false;
	}
	return Obj->MarkForCleanUp();
    }
    bool CleanUp( PolyMeshBase * Obj ){
	if( Obj == nullptr ){
	    return false;
	}
	delete Obj;
	Obj = nullptr;
	return true;
    }
    
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
	bool GetNeighbourFace( PolyMeshBase * PolyMeshObj, set< PolyMeshFace * > & sFace ){

        /// Gets all neightbouring faces of the PolyMeshObj.
	///
	/// @param PolyMeshObj PolyMesh object of interest.
	/// @param sFace       Returning set of neighbouring PolyMeshFaces.
	///
	/// @returns           True if success.
	    
	    return Impl::GetNeighbourFace( PolyMeshObj, sFace );
	}
	bool GetNeighbourVertex( PolyMeshBase * PolyMeshObj, set< PolyMeshVertex * > & sVertex ){

	/// Gets all neightbouring vertices of the PolyMeshObj.
	///
	/// @param PolyMeshObj PolyMesh object of interest.
	/// @param sFace       Returning set of neighbouring PolyMeshVertices.
	///
	/// @returns           True if success.
	    
	    return Impl::GetNeighbourVertex( PolyMeshObj, sVertex );
	}
	bool GetNeighbourEdge( PolyMeshBase * PolyMeshObj, set< PolyMeshEdge * > & sEdge ){

	/// Gets all neightbouring edges of the PolyMeshObj.
	///
	/// @param PolyMeshObj PolyMesh object of interest.
	/// @param sFace       Returning set of neighbouring PolyMeshEdges.
	///
	/// @returns           True if success.
	    
	    return Impl::GetNeighbourEdge( PolyMeshObj, sEdge );
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
    };
    
} // end of PolyMesh namespace

#endif
