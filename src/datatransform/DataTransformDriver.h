#ifndef DATA_TRANSFORM_DRIVER_H
#define DATA_TRANSFORM_DRIVER_H

#include "DataTransformMetaInfoCombiner.h"
#include "DataTransformMetaInfo.h"

#include <string>

class DataTransformPass;

// purpose:
//   base class for implementing data transform given input/output data meta info, transform pass
// expectation:
//   derived class is expected to implement driver behaviour
class DataTransformDriver {
public:
    DataTransformDriver();
    bool RegisterPass( DataTransformPass * pass );
    bool ExecutePasses( void * & data_in, void * & data_out );
    bool CleanUpPasses();
private:
    std::vector< DataTransformPass * > _Passes;
};

#endif
 
