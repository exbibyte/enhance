#ifndef PASS_CONVERT_POLYMESH_DATASTRUCT_TO_ARRAY_H
#define PASS_CONVERT_POLYMESH_DATASTRUCT_TO_ARRAY_H

#include "DataTransformPass.h"

class PolyMesh_Data_Arrays;

class PassConvertPolyMeshDataStructToArray : public DataTransformPass {
public:
    bool ExecutePass( void * & data_in, void * & data_out );
    bool CleanPass();
private:
    PolyMesh_Data_Arrays * _ArrayData;
};

#endif
