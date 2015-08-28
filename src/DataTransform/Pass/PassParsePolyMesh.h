#ifndef PASS_PARSE_POLYMESH_H
#define PASS_PARSE_POLYMESH_H

#include "DataTransformPass.h"

class Filter_ParsePolyMesh;

class PassParsePolyMesh : public DataTransformPass {
public:
    bool ExecutePass( void * & data_in, void * & data_out );
    bool CleanPass();
private:
    Filter_ParsePolyMesh * _FilterPolyMesh;
};

#endif
