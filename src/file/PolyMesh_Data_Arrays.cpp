#include "PolyMesh_Data_Arrays.h"

PolyMesh_Data_Arrays::PolyMesh_Data_Arrays(){
    _Vertex = nullptr;
    _Normal = nullptr;
}
PolyMesh_Data_Arrays::~PolyMesh_Data_Arrays(){
    if( !_Vertex ){
	delete [] _Vertex;
    }
    if( !_Normal ){
	delete [] _Normal;
    }
    _Vertex = nullptr;
    _Normal = nullptr;
    _iNumVertex = 0;
    _iNumNormal = 0;
}
bool PolyMesh_Data_Arrays::Set( PolyMesh_Data_Arrays_Type::Enum type, float * data, int iNumData ){
    bool bRet = true;
    switch( type ){
    case PolyMesh_Data_Arrays_Type::VERTEX:
    {
	_Vertex = new float[ iNumData ];
	float * current = _Vertex;
	for( int i = 0; i < iNumData; i++ ){
	    *current++ = *data++;
	}
	_iNumVertex = iNumData;
	break;
    }
    case PolyMesh_Data_Arrays_Type::NORMAL:
    {
	_Normal = new float[ iNumData ];
	float * current = _Normal;
	for( int i = 0; i < iNumData; i++ ){
	    *current++ = *data++;
	}
	_iNumNormal = iNumData;
	break;
    }
    default:
	return false;
    }
    return bRet;
}
bool PolyMesh_Data_Arrays::Get( PolyMesh_Data_Arrays_Type::Enum type, float * & data, int & iNumData ){
    bool bRet = true;
    switch( type ){
    case PolyMesh_Data_Arrays_Type::VERTEX:
    {
	iNumData = _iNumVertex;
	if( 0 >= iNumData || !_Vertex ){
	    return false;
	}
	data = new float [ iNumData ];
	float * data_start = data;
	float * current = _Vertex;
	for( int i = 0; i < iNumData; i++ ){
	    *data++ = *current++;
	}
	data = data_start;
	break;
    }
    case PolyMesh_Data_Arrays_Type::NORMAL:
    {
	iNumData = _iNumNormal;
	if( 0 >= iNumData || !_Normal ){
	    return false;
	}
	data = new float [ iNumData ];
	float * data_start = data;
	float * current = _Normal;
	for( int i = 0; i < iNumData; i++ ){
	    *data++ = *current++;
	}
	data = data_start;
	break;
    }
    default:
	return false;
    }
    return bRet;
}
