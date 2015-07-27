#include "DataTransformPass.h"
#include "DataTransformMetaInfo.h"
#include "DataType.h"

#include <string>
#include <vector>

using namespace std;

bool DataTransformPass::RegisterDataTransformMetaInfo( DataTransformMetaInfo * const meta_info ){
    if( !meta_info ){
	return false;
    }
    _MetaInfo = meta_info;
    return true;
}
bool DataTransformPass::GetDataTransformMetaInfo( DataTransformMetaInfo * & meta_info ) const {
    meta_info = _MetaInfo;
    return true;
}
bool DataTransformPass::GetInputFileFromMetaInfo( DataTransformMetaInfo * meta_info_input, DataType::Enum & type, string & data_path ){
    //TODO: try to retrieve data type and path from meta info
    vector< string > vals;
    if( !meta_info_input->GetMetaInfo( "INPUT_DATATYPE", vals ) ){
	return false;
    }
    auto i = vals.begin();
    if( vals.end() == i ){
	return false;
    }
    if( "FILE_POLYMESH" ==  *i ){
	type = DataType::FILE_POLYMESH;
    }
    else
    {
	type = DataType::FILE_OTHER;
    }
    if( !meta_info_input->GetMetaInfo( "INPUT_DATAPATH", vals ) ){
	return false;
    }
    i = vals.begin();
    if( vals.end() == i ){
	return false;
    }
    data_path = *i;
    return true;
}
