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
    virtual bool ExecutePass( void * & data_in, void * & data_out ){ return false; }
    bool RegisterDataTransformMetaInfo( DataTransformMetaInfo * const meta_info );
    bool GetDataTransformMetaInfo( DataTransformMetaInfo * & meta_info ) const;
    bool QueryFromMetaInfo( DataTransformMetaInfo * meta_info, DataQuery::Enum query_type, DataType::Enum & data_type, std::string & data_detail );
private:
    DataTransformMetaInfo * _MetaInfo;
};

#endif
