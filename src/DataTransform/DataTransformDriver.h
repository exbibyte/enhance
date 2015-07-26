#ifndef DATA_TRANSFORM_DRIVER_H
#define DATA_TRANSFORM_DRIVER_H

// purpose:
//   base class for implementing data transform given input/output data meta info, transform pass
// expectation:
//   derived class is expected to implement driver behaviour
class DataTransformDriver {
public:
    bool RegisterPass( DataTransformPass * pass );
    bool AddTransformInputInfo( DataTransformMetaInfo * in );
    bool RemoveTransformInputInfo( string id );
    bool RemoveTransformInputInfoAll();
    bool AddTransformOutputInfo( DataTransformMetaInfo * out );
    bool RemoveTransformOutputInfo( string id );
    bool RemoveTransformOutputInfoAll();
    bool BuildTransformInfoAggregate();
    bool FindSuitablePath( DataTransformPass * & pass );
    bool ExecuteSuitablePath();
private:
    DataTransformMetaInfoCombiner _MetaInfoCombiner;
    std::vector< DataTransformPass * > _Passes;
    std::vector< DataTransformMetaInfo * > _InputMetaInfos;
    std::vector< DataTransformMetaInfo * > _OutputMetaInfos;
    DataTransformMetaInfo * _InputMetaInfoAggregate;
    DataTransformMetaInfo * _OutputMetaInfoAggregate;
};

#endif
 
