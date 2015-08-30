#ifndef POLYMESH_DATA_ARRAYS
#define POLYMESH_DATA_ARRAYS

#include <map>
#include <string>

class GLBufferInfo;
class GLBufferInfoSequence;

class PolyMesh_Data_Arrays_Type {
public:
    enum Enum {
	VERTEX,
	NORMAL,
	BUFFERINFO,
	BUFFERINFO_SEQUENCE,
    };
};

class PolyMesh_Data_Arrays {
public:
    PolyMesh_Data_Arrays();
    ~PolyMesh_Data_Arrays();
    bool Set( PolyMesh_Data_Arrays_Type::Enum type, float * data, int iNumData );
    bool Get( PolyMesh_Data_Arrays_Type::Enum type, float * & data, int & iNumData );
    bool SetBufferInfo( GLBufferInfo * buffer_info );
    bool GetBufferInfo( std::string strName, GLBufferInfo * & buffer_info );
    bool GetMapBufferInfo( std::map< std::string, GLBufferInfo * > & map_buffer_info );
    bool SetBufferInfoSequence( GLBufferInfoSequence * buffer_info_sequence );
    bool GetBufferInfoSequence( std::string strName, GLBufferInfoSequence * & buffer_info_sequence );
    bool GetMapBufferInfoSequence( std::map< std::string, GLBufferInfoSequence * > & map_buffer_info_sequence );
    float * _Vertex;
    float * _Normal;
    int _iNumVertex;
    int _iNumNormal;
    std::map< std::string, GLBufferInfo * > _Map_BufferInfo;
    std::map< std::string, GLBufferInfoSequence * > _Map_BufferInfoSequence;
};

#endif
