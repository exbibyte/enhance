#ifndef POLYMESH_DATA_ARRAYS
#define POLYMESH_DATA_ARRAYS

#include <vector>

class PolyMesh_Data_Arrays_Type {
public:
    enum Enum {
	VERTEX,
	NORMAL,
    };
};

class PolyMesh_Data_Arrays {
public:
    PolyMesh_Data_Arrays();
    ~PolyMesh_Data_Arrays();
    bool Set( PolyMesh_Data_Arrays_Type::Enum type, float * data, int iNumData );
    bool Get( PolyMesh_Data_Arrays_Type::Enum type, float * & data, int & iNumData );
    float * _Vertex;
    float * _Normal;
    int _iNumVertex;
    int _iNumNormal;
    std::vector< GLBufferInfo * > _BufferSegments;
};

#endif
