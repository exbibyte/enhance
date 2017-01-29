#include "PolyMeshInterface.h"
#include "Vec.hpp"
#include <list>
#include <set>
#include <vector>
#include <functional>
using namespace std;

namespace PolyMesh {

    bool SetPolyMeshStatus( unsigned int * uiStatus, PolyMeshStatus::Enum const StatusType, bool const bSet ){	 
	
	switch( StatusType ){
	case PolyMeshStatus::Count:
	    return false;
	default:
	    *uiStatus = bSet << StatusType;
	    break;
	}
	return true;
    }
    
     bool GetPolyMeshStatus( unsigned int const * uiStatus, PolyMeshStatus::Enum const StatusType, bool * bVal ){
       
	switch( StatusType ){
	case PolyMeshStatus::Count:
	    return false;
	default:
	    unsigned int iTemp = ( *uiStatus >> StatusType ) & 1;
	    *bVal = ( iTemp == 1 );
	}
	return true;
    }

     bool IsThisPolyMesh( PolyMeshBase * Obj, PolyMeshType::Enum const PolyType, int const Id ){
     	
	bool bStatusNotInit;
	if( !GetPolyMeshStatus( &Obj->_Status, PolyMeshStatus::NotInit, &bStatusNotInit ) ){
	    return false;
	}else{
	    if( bStatusNotInit == false ){
		return  ( PolyType == Obj->_PolyType ) && ( Id == Obj->_Id );
	    }else{
		return false;
	    }
	}       
    }   

     bool ConnectPolyMeshObjects( PolyMeshVertex * V, PolyMeshEdge * E ){

	if( nullptr == V || nullptr == E ){
	    return false;
	}
	V->_Edges.insert( E );
	E->_Vertices.insert( V );
	return true;
    }

    bool ConnectPolyMeshObjects( PolyMeshVertex * V, PolyMeshFace * F ){
	
	if( nullptr == V || nullptr == F ){
	    return false;
	}
	V->_Faces.insert( F );
	F->_Vertices.insert( V );
	return true;
    }

    bool ConnectPolyMeshObjects( PolyMeshEdge * E, PolyMeshFace * F ){

	if( nullptr == E || nullptr == F ){
	    return false;
	}
	E->_Faces.insert( F );
	F->_Edges.insert( E );
	return true;
    }

    bool DisconnectPolyMeshObjects( PolyMeshVertex * V, PolyMeshEdge * E ){

	if( nullptr == V || nullptr == E ){
	    return false;
	}
	V->_Edges.erase( E );
	E->_Vertices.erase( V );
	return true;
    }

    bool DisconnectPolyMeshObjects( PolyMeshVertex * V, PolyMeshFace * F ){
	
	if( nullptr == V || nullptr == F ){
	    return false;
	}
	V->_Faces.erase( F );
	F->_Vertices.erase( V );
	return true;
    }

    bool DisconnectPolyMeshObjects( PolyMeshEdge * E, PolyMeshFace * F ){

	if( nullptr == E || nullptr == F ){
	    return false;
	}
	E->_Faces.erase( F );
	F->_Edges.erase( E );
	return true;
    }

    bool DisconnectPolyMeshObjectFrom( PolyMeshVertex * V_Source, PolyMeshEdge * E_Remove ){
	if( nullptr == V_Source || nullptr == E_Remove ){
	    return false;
	}
	V_Source->_Edges.erase( E_Remove );
	return true;
    }
    
    bool DisconnectPolyMeshObjectFrom( PolyMeshVertex * V_Source, PolyMeshFace * F_Remove ){
	if( nullptr == V_Source || nullptr == F_Remove ){
	    return false;
	}
	V_Source->_Faces.erase( F_Remove );
	return true;
    }
    
    bool DisconnectPolyMeshObjectFrom( PolyMeshEdge * E_Source, PolyMeshVertex * V_Remove ){
	if( nullptr == E_Source || nullptr == V_Remove ){
	    return false;
	}
	E_Source->_Vertices.erase( V_Remove );
	return true;
    }
    
    bool DisconnectPolyMeshObjectFrom( PolyMeshEdge * E_Source, PolyMeshFace * F_Remove ){
	if( nullptr == E_Source || nullptr == F_Remove ){
	    return false;
	}
	E_Source->_Faces.erase( F_Remove );
	return true;
    }
    
    bool DisconnectPolyMeshObjectFrom( PolyMeshFace * F_Source, PolyMeshVertex * V_Remove ){
	if( nullptr == F_Source || nullptr == V_Remove ){
	    return false;
	}
	F_Source->_Vertices.erase( V_Remove );
	return true;
    }
    
    bool DisconnectPolyMeshObjectFrom( PolyMeshFace * F_Source, PolyMeshEdge * E_Remove ){
	if( nullptr == F_Source || nullptr == E_Remove ){
	    return false;
	}
	F_Source->_Edges.erase( E_Remove );
	return true;
    }
    
    bool MarkForCleanUp( PolyMeshBase * Obj ){
	if( Obj == nullptr ){
	    return false;
	}
	return Obj->MarkForCleanUp();
    }
    
    bool Delete( PolyMeshBase * Obj ){
	if( Obj == nullptr ){
	    return false;
	}
	delete Obj;
	Obj = nullptr;
	return true;
    }
}
