#include "DataTransformPass.h"

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
