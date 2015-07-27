#include "DataTransformMetaInfo.h"

#include <utility>
#include <map>
#include <string>
#include <iterator>
#include <algorithm>

using namespace std;

DataTransformMetaInfo::DataTransformMetaInfo( string id ){
    _id = id;
}
bool DataTransformMetaInfo::AddMetaInfo( string key, string val ) {
    _MetaInfo.insert( pair< string, string >( key, val ) );
    return true;
}
bool DataTransformMetaInfo::GetMetaInfo( string key, vector< string > & vals ) const {
    vals.clear();
    auto i = _MetaInfo.equal_range( key );
    for( auto j  = i.first; j != i.second; ++j ){
	vals.push_back( j->second );
    }
    return true;
}
bool DataTransformMetaInfo::RemoveMetaInfo( string key ) {    
    _MetaInfo.erase( key );
    return true;
}
bool DataTransformMetaInfo::RemoveMetaInfoAll() {
    _MetaInfo.clear();
    return true;
}
bool DataTransformMetaInfo::GetId( string & id ) const {
    id = _id;
    return true;
}
bool DataTransformMetaInfo::AddMetaInfo( DataTransformMetaInfo & meta_info ){
    for( auto & i : meta_info._MetaInfo ){
	this->AddMetaInfo( i.first, i.second );
    }
    return true;
}
