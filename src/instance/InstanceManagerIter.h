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
    bool LinkExternalInstanceManager( std::vector< std::pair<DataType, InstanceManagerIter<DataType> * > > external_managers ){
	for( auto & i : external_managers ){
	    _external_manager[ i.first ] = i.second;
	}
	return true;
    }
    bool GetExternalInstanceManager( DataType datatype, InstanceManagerIter<DataType> * & manager ){
	auto it_find = _external_manager.find( datatype );
	if( _external_manager.end() == it_find ){
	    return false;
	}
	manager = it_find->second;
	return true;
    }
private:
    const int _num_types;
    std::vector< DataType > _types;
    std::map< DataType, unsigned int > _type_index;
    std::map< unsigned int, std::vector< int > > _data;
    std::map< DataType, InstanceManagerIter<DataType> * > _external_manager;
    AssetManagerType _asset_manager;
};

#endif
