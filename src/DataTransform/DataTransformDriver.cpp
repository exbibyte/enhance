#include "DataTransformDriver.h"
#include "DataTransformPass.h"
#include "DataTransformMetaInfo.h"

#include <vector>
#include <algorithm>

using namespace std;

DataTransformDriver::DataTransformDriver(){
    _pInputMetaInfoAggregate = & _InputMetaInfoAggregate;
    _pOutputMetaInfoAggregate = & _OutputMetaInfoAggregate;
}
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
    return true;
}
bool DataTransformDriver::RemoveTransformInputInfo( string id ){
    auto end_after_remove = std::remove_if( _InputMetaInfos.begin(), _InputMetaInfos.end(), [ id ]( DataTransformMetaInfo * const item )->bool { string item_id; item->GetId( item_id ); return id == item_id; } );
    auto start = _InputMetaInfos.begin();
    int iCount = 0;
    while( start != end_after_remove ){
	++iCount;
	++start;	
    }
    _InputMetaInfos.resize( iCount );
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
    return true;
}
bool DataTransformDriver::RemoveTransformOutputInfo( string id ){
    auto end_after_remove = std::remove_if( _OutputMetaInfos.begin(), _OutputMetaInfos.end(), [ id ]( DataTransformMetaInfo * const item )->bool { string item_id; item->GetId( item_id ); return id == item_id; } );
    auto start = _OutputMetaInfos.begin();
    int iCount = 0;
    while( start != end_after_remove ){
	++iCount;
	++start;	
    }
    _OutputMetaInfos.resize( iCount );
    return true;
}
bool DataTransformDriver::RemoveTransformOutputInfoAll(){
    _OutputMetaInfos.clear();
    return true;
}
bool DataTransformDriver::BuildTransformInfoAggregate(){
    if( !_MetaInfoCombiner.BuildAggregate( _InputMetaInfos, _pInputMetaInfoAggregate ) ){
	return false;
    }
    if( !_MetaInfoCombiner.BuildAggregate( _OutputMetaInfos, _pOutputMetaInfoAggregate ) ){
	return false;
    }
    
    return true;
}
bool DataTransformDriver::FindSuitablePath( DataTransformPass * & pass ){
    //TODO: find the most suitable pass based on aggregate input and output meta info    
    for( auto & i : _Passes ){
	DataTransformMetaInfo * meta_info;
	if( !i->GetDataTransformMetaInfo( meta_info ) ){
	    continue;
	}
	else
	{
	    //TODO: determine match with _InputMetaInfoAggregate and _OutputMetaInfoAggregate
	    pass = i;
	    return true;
	}
    }    
    return true;
}

bool DataTransformDriver::ExecuteSuitablePath(){
    bool bRet;
    DataTransformPass * suitable_pass;
    if( !FindSuitablePath( suitable_pass ) ){
	return false;
    }
    bRet = suitable_pass->ExecutePath( _pInputMetaInfoAggregate, _pOutputMetaInfoAggregate );
    return bRet;
}
