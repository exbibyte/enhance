#ifndef DATA_TRANSFORM_PASS_H
#define DATA_TRANSFORM_PASS_H

#include "DataType.h"

#include <string>

class DataTransformMetaInfo;

// purpose:
//   base class for implementing data transform pass
// expectation:
//   derived class is expected to implement pass functionality
class DataTransformPass {
public:
    virtual bool ExecutePath( DataTransformMetaInfo * meta_info_input, DataTransformMetaInfo * meta_info_output ){ return false; }
    bool RegisterDataTransformMetaInfo( DataTransformMetaInfo * const meta_info );
    bool GetDataTransformMetaInfo( DataTransformMetaInfo * & meta_info ) const;
    bool GetInputFileFromMetaInfo( DataTransformMetaInfo * meta_info_input, DataType::Enum & type, std::string & data_path );
private:
    DataTransformMetaInfo * _MetaInfo;
};

#endif
