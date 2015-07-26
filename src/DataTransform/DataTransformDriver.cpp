#include "DataTransformDriver.h"
#include "DataTransformPass.h"

#include <vector>
#include <algorithm>

using namespace std;

bool DataTransformDriver::RegisterPass( DataTransformPass * pass ){
    if( !pass ){
	return false;
    }
    _Passes.push_back( pass );
    return true;
}

bool DataTransformDriver::AddTransformInputInfo( DataTransformMetaInfo * in ){
    if( !in ){
	return false;
    }
    _InputMetaInfos.push_back( in );
}
bool DataTransformDriver::RemoveTransformInputInfo( string id ){
    auto end_after_remove = std::remove_if( _InputMetaInfos.begin(), _InputMetaInfos.end(), [ id ]( DataTransformMetaInfo * const item ){ return id == item->_id; } );
    auto start = _InputMetaInfo.begin();
    int iCount = 0;
    while( start != end_after_remove ){
	++iCount;
	++start;	
    }
    _InputMetaInfo.resize( iCount );
    return true;
}
bool DataTransformDriver::RemoveTransformInputInfoAll(){
    _InputMetaInfos.clear();
    return true;
}
bool DataTransformDriver::AddTransformOutputInfo( DataTransformMetaInfo * out ){
    if( !out ){
	return false;
    }
    _OutputMetaInfos.push_back( out );
}
bool DataTransformDriver::RemoveTransformOutputInfo( string id ){
    auto end_after_remove = std::remove_if( _OutputMetaInfos.begin(), _OutputMetaInfos.end(), [ id ]( DataTransformMetaInfo * const item ){ return id == item->_id; } );
    auto start = _OutputMetaInfo.begin();
    int iCount = 0;
    while( start != end_after_remove ){
	++iCount;
	++start;	
    }
    _OutputMetaInfo.resize( iCount );
    return true;
}
bool DataTransformDriver::RemoveTransformOutputInfoAll(){
    _OutputMetaInfos.clear();
    return true;
}
bool DataTransformDriver::BuildTransformInfoAggregate(){
    if( !_MetaInfoCombiner.BuildAggregate( _InputMetaInfos, _InputMetaInfoAggregate ) ){
	return false;
    }
    if( !_MetaInfoCombiner.BuildAggregate( _OutputMetaInfos, _OutputMetaInfoAggregate ) ){
	return false;
    }
    
    return true;
}
bool DataTransformDriver::FindSuitablePath( DataTransformPass * & pass ){
    //TODO: find the most suitable pass based on aggregate input and output meta info    
    for( auto & i : _Passes ){
	DataTransformMetaInfo * meta_info;
	if( !GetDataTransformMetaInfo( meta_info ) ){
	    continue;
	}
    }    
    return true;
}

bool DataTransformDriver::ExecuteSuitablePath(){
    DataTransformPass * suitable_pass;
    if( !FindSuitablePath( suitable_pass ) ){
	return false;
    }
    suitable_pass->ExecutePath();
    return true;
}
