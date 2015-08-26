#ifndef PASS_PARSE_POLYMESH_H
#define PASS_PARSE_POLYMESH_H

#include "DataTransformPass.h"

class PassParsePolyMesh : public DataTransformPass {
    bool ExecutePass( void * & data_in, void * & data_out );
};

#endif
