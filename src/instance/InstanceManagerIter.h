#ifndef INSTANCEMANAGER_ITER_H
#define INSTANCEMANAGER_ITER_H

//==================================
//
// Summary:      Data manager for instance tag type of DataType, and with leaf data storage policy as defined in AssetManagerType (defaults to key type of int and value type of vector< double >
// Notes:
// Dependencies: AssetManager, InstanceType
//==================================

#include <vector>
#include <map>
#include <utility>
#include <algorithm>

#include "InstanceType.h"
#include "AssetManager.h"

template< typename DataType, typename AssetManagerType = AssetManager< int, std::vector< double > > >
class InstanceManagerIter {
public:
    InstanceManagerIter( std::vector< DataType > datatypes ) : _types( datatypes ), _num_types( datatypes.size() ){
	for( unsigned int i = 0; i < _types.size(); ++i ){
	    _type_index[_types[i]] = i;
	}
    }
    bool QueryData( unsigned int id, DataType arg, int & val){
	auto it_find_id = _data.find( id );
	if( _data.end() == it_find_id ){
	    return false;
	}
	auto it_find_index = _type_index.find( arg );
	if( _type_index.end() == it_find_index ){
	    return false;
	}
	val = it_find_id->second[ it_find_index->second ];
	return true;
    }
    template< typename LeafDataType >
    bool QueryData( int id, LeafDataType & data ){
	bool bRet = _asset_manager.GetData( id, data );
	return bRet;
    }
    bool SetData( unsigned int id, DataType arg, int val ){
	auto it_find_index = _type_index.find( arg );
	if( _type_index.end() == it_find_index ){
	    return false;
	}
	auto it_find_id = _data.find( id );
	if( _data.end() == it_find_id ){ //create new entry if not existing
	    _data[ id ] = std::vector< int >( _num_types, 0 );
	}
	//modify existing entry
	_data[ id ][ it_find_index->second ] = val;
	return true;
    }
    template< typename LeafDataType >
    bool SetData( int id, LeafDataType data ){
	bool bRet = _asset_manager.AddData( id, data );
	return bRet;
    }
    bool LinkExternalInstanceManager( std::vector< std::pair<DataType, InstanceManagerIter<DataType, AssetManagerType> * > > external_managers ){
	for( auto & i : external_managers ){
	    _external_manager[ i.first ] = i.second;
	}
	return true;
    }
    bool GetExternalInstanceManager( DataType datatype, InstanceManagerIter<DataType, AssetManagerType> * & manager ){
	auto it_find = _external_manager.find( datatype );
	if( _external_manager.end() == it_find ){
	    return false;
	}
	manager = it_find->second;
	return true;
    }
    bool GetDataTypes( std::vector< DataType > & data_types ){
	data_types = _types;
	return true;
    }
    bool GetLinkedAttributes( std::vector< DataType > & attributes ){
	std::vector< DataType > data_types;
        GetDataTypes( data_types );
	for( auto & i : data_types ){
	    attributes.push_back( i );
	    auto it_external_manager = _external_manager.find( i );
	    if( _external_manager.end() != it_external_manager ){
		it_external_manager->second->GetLinkedAttributes( attributes );
	    }
	}
	return true;
    }
    bool QueryLinkedAttributeVal( std::vector<std::pair<unsigned int, DataType> > attribute_keys, int & query_val ){
	std::vector< DataType > data_types;
        GetDataTypes( data_types );
	if( attribute_keys.size() >= 1 ){
	    unsigned int entry_id = attribute_keys[0].first;
	    DataType entry_attribute = attribute_keys[0].second;
	    if( !QueryData( entry_id, entry_attribute, query_val ) ){ //can't find entry with that attribute
		return false;
	    }
	    if( attribute_keys.size() == 1 ){ //found final attribute
		return true;
	    }else{ //continue finding next attribute
		attribute_keys.erase( attribute_keys.begin() );
		InstanceManagerIter<DataType, AssetManagerType> * manager;
		if( !GetExternalInstanceManager( entry_attribute, manager ) ){ //can't find external manager for the given attribute
		    return false;
		}else{
		    return manager->QueryLinkedAttributeVal( attribute_keys, query_val );
		}
	    }
	}else{
	    return false;
	}
	return false;
    }
    template< typename LeafDataType >
    bool QueryLinkedAttributeLeafData( std::vector<std::pair<unsigned int, DataType> > attribute_keys, int query_id, LeafDataType & query_val ){
	std::vector< DataType > data_types;
        GetDataTypes( data_types );
	if( attribute_keys.size() >= 1 ){
	    unsigned int entry_id = attribute_keys[0].first;
	    DataType entry_attribute = attribute_keys[0].second;
	    int temp_query_val;
	    if( !QueryData( entry_id, entry_attribute, temp_query_val ) ){ //can't find entry with that attribute
		return false;
	    }
            //continue finding next attribute
	    attribute_keys.erase( attribute_keys.begin() );
	    InstanceManagerIter<DataType, AssetManagerType> * manager;
	    if( !GetExternalInstanceManager( entry_attribute, manager ) ){ //can't find external manager for the given attribute
		return false;
	    }else{
		return manager->QueryLinkedAttributeLeafData( attribute_keys, query_id, query_val );
	    }
	}else{ //try query leaf data
	    return QueryData( query_id, query_val );
	}
	return false;
    }
    bool GetLinkedAttributeManager( std::vector<std::pair<unsigned int, DataType> > attribute_keys, InstanceManagerIter<DataType, AssetManagerType> * & manager ){
	std::vector< DataType > data_types;
        GetDataTypes( data_types );
	if( attribute_keys.size() >= 1 ){
	    unsigned int entry_id = attribute_keys[0].first;
	    DataType entry_attribute = attribute_keys[0].second;
	    int temp_query_val;
	    if( !QueryData( entry_id, entry_attribute, temp_query_val ) ){ //can't find entry with that attribute
		return false;
	    }
            //continue finding next attribute
	    attribute_keys.erase( attribute_keys.begin() );
	    if( !GetExternalInstanceManager( entry_attribute, manager ) ){ //can't find external manager for the given attribute
		return false;
	    }else{
		return manager->GetLinkedAttributeManager( attribute_keys, manager );
	    }
	}
	return true;
    }
private:
    const int _num_types;
    std::vector< DataType > _types;
    std::map< DataType, unsigned int > _type_index;
    std::map< unsigned int, std::vector< int > > _data;
    std::map< DataType, InstanceManagerIter<DataType, AssetManagerType> * > _external_manager;
    AssetManagerType _asset_manager;
};

#endif
