#include "DataTransformPass.h"
#include "DataTransformMetaInfo.h"
#include "DataType.h"

bool DataTransformPass::RegisterDataTransformMetaInfo( DataTransformMetaInfo * const meta_info ){
    if( !meta ){
	return false;
    }
    _MetaInfo = meta_info;
    return true;
}
bool DataTransformPass::GetDataTransformMetaInfo( DataTransformMetaInfo * & meta_info ) const {
    meta_info = _MetaInfo;
    return true;
}
bool DataTransformPass::GetInputFileFromMetaInfo( DataType & type, string & data_path ){
    //TODO: try to retrieve data type and path from meta info

    return true;
}
