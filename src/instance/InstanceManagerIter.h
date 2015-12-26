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

template< typename DataType, typename AssetManagerType = AssetManager< unsigned int, std::vector< double > > >
class InstanceManagerIter {
public:
    InstanceManagerIter( std::vector< DataType > datatypes ) : _types( datatypes ), _num_types( datatypes.size() ){
	for( unsigned int i = 0; i < _types.size(); ++i ){
	    _type_index[_types[i]] = i;
	}
    }
    bool QueryData( unsigned int id, DataType arg, unsigned int & val){
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
    bool QueryDataLeaf( int id, LeafDataType & data ){
	bool bRet = _asset_manager.GetData( id, data );
	return bRet;
    }
    template< typename LeafDataType >
    bool SetDataLeaf( int id, LeafDataType & data ){
	bool bRet = _asset_manager.AddData( id, data );
	return bRet;
    }
    bool SetData( unsigned int id, DataType arg, unsigned int val ){
	auto it_find_index = _type_index.find( arg );
	if( _type_index.end() == it_find_index ){
	    return false;
	}
	auto it_find_id = _data.find( id );
	if( _data.end() == it_find_id ){ //create new entry if not existing
	    _data[ id ] = std::vector< unsigned int >( _num_types, 0 );
	}
	//modify existing entry
	_data[ id ][ it_find_index->second ] = val;
	return true;
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
    bool QueryLinkedAttributeVal( unsigned int id, std::vector< DataType > attributes, unsigned int & query_val ){
	if( attributes.size() == 0 ){
	    return false;
	}else if( attributes.size() == 1 ){
	    return QueryData( id, attributes[0], query_val ); // query attribute
	}else{ // size of 2 or greater
	    unsigned int entry_id = id;
	    DataType entry_attribute = attributes[0];
	    unsigned int next_entry_id;
	    if( !QueryData( entry_id, entry_attribute, next_entry_id ) ){ //can't find entry with that attribute
		return false;
	    }
	    //continue finding next attribute
	    attributes.erase( attributes.begin() );
	    InstanceManagerIter<DataType, AssetManagerType> * manager;
	    if( !GetExternalInstanceManager( entry_attribute, manager ) ){ //can't find external manager for the given attribute
		return false;
	    }else{
		return manager->QueryLinkedAttributeVal( next_entry_id, attributes, query_val );
	    }
	}
	return false;
    }
    bool SetLinkedAttributeVal( unsigned int id, std::vector< DataType > attributes, unsigned int set_val ){
	if( attributes.size() == 0 ){
	    return false;
	}else if( attributes.size() == 1 ){
	    return SetData( id, attributes[0], set_val ); //set attribute val
	}else{ // size of 2 or greater
	    unsigned int entry_id = id;
	    DataType entry_attribute = attributes[0];
	    unsigned int next_entry_id;
	    if( !QueryData( entry_id, entry_attribute, next_entry_id ) ){ //can't find entry with that attribute
		return false;
	    }
	    //continue finding next attribute
	    attributes.erase( attributes.begin() );
	    InstanceManagerIter<DataType, AssetManagerType> * manager;
	    if( !GetExternalInstanceManager( entry_attribute, manager ) ){ //can't find external manager for the given attribute
		return false;
	    }else{
		return manager->SetLinkedAttributeVal( next_entry_id, attributes, set_val ); //recurse
	    }
	}
	return false;
    }
    template< typename LeafDataType >
    bool QueryLinkedAttributeLeafData( unsigned int id, std::vector< DataType > attributes, LeafDataType & query_val ){
	if( attributes.size() == 0 ){ 
	    return QueryDataLeaf( id, query_val ); //query leaf data
	}else{ //size of 1 or greater
	    unsigned int entry_id = id;
	    DataType entry_attribute = attributes[0];
	    unsigned int next_entry_id;
	    if( !QueryData( entry_id, entry_attribute, next_entry_id ) ){ //find entry with that attribute
		return false;
	    }
            //continue finding next attribute
	    attributes.erase( attributes.begin() );
	    InstanceManagerIter<DataType, AssetManagerType> * manager;
	    if( !GetExternalInstanceManager( entry_attribute, manager ) ){ //find external manager for the given attribute
		return false;
	    }else{
		return manager->QueryLinkedAttributeLeafData( next_entry_id, attributes, query_val );
	    }
	}
	return false;
    }
    template< typename LeafDataType >
    bool SetLinkedAttributeLeafData( unsigned int id, std::vector< DataType > attributes, LeafDataType & set_val ){
	if( attributes.size() == 0 ){
	    return SetDataLeaf( id, set_val ); //set leaf data
	}else{ //size of 1 or greater
	    unsigned int entry_id = id;
	    DataType entry_attribute = attributes[0];
	    unsigned int next_entry_id;
	    if( !QueryData( entry_id, entry_attribute, next_entry_id ) ){ //query value of the attribute
		return false;
	    }
            //continue finding next attribute
	    attributes.erase( attributes.begin() );
	    InstanceManagerIter<DataType, AssetManagerType> * manager;
	    if( !GetExternalInstanceManager( entry_attribute, manager ) ){ //find external manager for the given attribute
		return false;
	    }else{
		return manager->SetLinkedAttributeLeafData( next_entry_id, attributes, set_val ); //recurse
	    }
	}
	return false;
    }
    bool GetLinkedAttributeManager( std::vector< DataType > attributes, InstanceManagerIter<DataType, AssetManagerType> * & manager ){
	if( attributes.size() >= 1 ){
	    DataType entry_attribute = attributes[0];
	    if( !GetExternalInstanceManager( entry_attribute, manager ) ){ //find external manager for the given attribute
		return false;
	    }else{
		//continue finding next attribute
		attributes.erase( attributes.begin() );
		return manager->GetLinkedAttributeManager( attributes, manager ); // recurse
	    }
	}
	return true;
    }
private:
    const int _num_types;
    std::vector< DataType > _types;
    std::map< DataType, unsigned int > _type_index;
    std::map< unsigned int, std::vector< unsigned int > > _data;
    std::map< DataType, InstanceManagerIter<DataType, AssetManagerType> * > _external_manager;
    AssetManagerType _asset_manager;
};

#endif
