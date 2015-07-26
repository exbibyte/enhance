#ifndef DATA_TRANSFORM_PASS_H
#define DATA_TRANSFORM_PASS_H

// purpose:
//   base class for implementing data transform pass
// expectation:
//   derived class is expected to implement pass functionality
class DataTransformPass {
public:
    virtual bool ExecutePath( DataTransformMetaInfo * meta_info ){ return false; }
    bool RegisterDataTransformMetaInfo( DataTransformMetaInfo * const meta_info );
    bool GetDataTransformMetaInfo( DataTransformMetaInfo * & meta_info ) const;
private:
    DataTransformMetaInfo * _MetaInfo;
};

#endif
