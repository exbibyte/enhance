#ifndef DATA_TRANSFORM_METAINFO_COMBINER_H
#define DATA_TRANSFORM_METAINFO_COMBINER_H

class DataTransformMetaInfo;

class DataTransformMetaInfoCombiner {
    bool BuildAggregate( vector< DataTransformMetaInfo * > inputs, DataTransformMetaInfo * & aggregate );
};
#endif
