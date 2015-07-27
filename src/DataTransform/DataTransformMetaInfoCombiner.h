#ifndef DATA_TRANSFORM_METAINFO_COMBINER_H
#define DATA_TRANSFORM_METAINFO_COMBINER_H

#include <vector>

class DataTransformMetaInfo;

class DataTransformMetaInfoCombiner {
public:
    bool BuildAggregate( std::vector< DataTransformMetaInfo * > inputs, DataTransformMetaInfo * & aggregate );
};
#endif
