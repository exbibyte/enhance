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
bool DataTransformPass::QueryFromMetaInfo( DataTransformMetaInfo * meta_info, DataQuery::Enum query_type, DataType::Enum & data_type, string & data_detail ){
    //TODO: try to retrieve data type and path from meta info
    vector< string > vals;

    string strQueryDataType;
    string strQueryDataDetail;
    
    switch( query_type ){
    case DataQuery::INPUT:
    {
        strQueryDataType = "INPUT_DATATYPE";
	strQueryDataDetail = "INPUT_DATAPATH";
	if( !meta_info->GetMetaInfo( strQueryDataType, vals ) ){
	    return false;
	}
	auto i = vals.begin();
	if( vals.end() == i ){
	    return false;
	}
	if( "FILE_POLYMESH" ==  *i ){
	    data_type = DataType::FILE_POLYMESH;
	    if( !meta_info->GetMetaInfo( strQueryDataDetail, vals ) ){
		return false;
	    }
	    i = vals.begin();
	    if( vals.end() == i ){
		return false;
	    }
	    data_detail = *i;
	}
	else if( "DATASTRUCT_POLYMESH" ==  *i ){
	    data_type = DataType::DATASTRUCT_POLYMESH;
	}
	else if( "DATASTRUCT_POLYMESH_ARRAY" ==  *i ){
	    data_type = DataType::DATASTRUCT_POLYMESH_ARRAY;
	}
	else
	{
	    data_type = DataType::FILE_OTHER;
	}
	break;
    }
    case DataQuery::OUTPUT:
    {
	strQueryDataType = "OUTPUT_DATATYPE";
	strQueryDataDetail = "OUTPUT_DATAPATH";
	if( !meta_info->GetMetaInfo( strQueryDataType, vals ) ){
	    return false;
	}
	auto i = vals.begin();
	if( vals.end() == i ){
	    return false;
	}
	if( "FILE_POLYMESH" ==  *i ){
	    data_type = DataType::FILE_POLYMESH;
	    if( !meta_info->GetMetaInfo( strQueryDataDetail, vals ) ){
		return false;
	    }
	    i = vals.begin();
	    if( vals.end() == i ){
		return false;
	    }
	    data_detail = *i;
	}
	else if( "DATASTRUCT_POLYMESH" ==  *i ){
	    data_type = DataType::DATASTRUCT_POLYMESH;
	}
	else if( "DATASTRUCT_POLYMESH_ARRAY" ==  *i ){
	    data_type = DataType::DATASTRUCT_POLYMESH_ARRAY;
	}
	else
	{
	    data_type = DataType::FILE_OTHER;
	}
	break;
    }
    default:
	return false;
    }
    
    return true;
}
