#include "PolyMesh_Data_Arrays.h"
#include "GLBufferInfo.h"

#include <string>
#include <map>
using namespace std;

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
bool PolyMesh_Data_Arrays::SetBufferInfo( GLBufferInfo * buffer_info ){
    bool bRet = true;
    if( !buffer_info ){
	return false;
    }
    _Map_BufferInfo[ buffer_info->_Name ] = buffer_info;
    return bRet;
}
bool PolyMesh_Data_Arrays::GetBufferInfo( string strName, GLBufferInfo * & buffer_info ){
    bool bRet = true;
    auto it = _Map_BufferInfo.find( strName );
    if( _Map_BufferInfo.end() == it ){
	return false;
    }
    buffer_info = it->second;
    return bRet;
}
bool PolyMesh_Data_Arrays::GetMapBufferInfo( std::map< std::string, GLBufferInfo * > & map_buffer_info ){
    bool bRet = true;
    map_buffer_info = _Map_BufferInfo;
    return bRet;
}
bool PolyMesh_Data_Arrays::SetBufferInfoSequence( GLBufferInfoSequence * buffer_info_sequence ){
    bool bRet = true;
    if( !buffer_info_sequence ){
	return false;
    }
    _Map_BufferInfoSequence[ buffer_info_sequence->_Name ] = buffer_info_sequence;
    return bRet;
}
bool PolyMesh_Data_Arrays::GetBufferInfoSequence( string strName, GLBufferInfoSequence * & buffer_info_sequence ){
    bool bRet = true;
    auto it = _Map_BufferInfoSequence.find( strName );
    if( _Map_BufferInfoSequence.end() == it ){
	return false;
    }
    buffer_info_sequence = it->second;
    return bRet;
}

bool PolyMesh_Data_Arrays::GetMapBufferInfoSequence( map< string, GLBufferInfoSequence * > & map_buffer_info_sequence ){
    bool bRet = true;
    map_buffer_info_sequence = _Map_BufferInfoSequence;
    return bRet;
}
