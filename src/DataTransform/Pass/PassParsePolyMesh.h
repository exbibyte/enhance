#ifndef PASS_PARSE_POLYMESH_H
#define PASS_PARSE_POLYMESH_H

#include "DataTransformPass.h"

class PassParsePolyMesh : public DataTransformPass {
    bool ExecutePath( DataTransformMetaInfo * meta_info_input, DataTransformMetaInfo * meta_info_output );
};

#endif
