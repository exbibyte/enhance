#ifndef ASSETMANAGER_H
#define ASSETMANAGER_H

#include <vector>
#include <memory>
#include <string>
#include <utility>
#include <algorithm>
#include <cassert>

template< class AssetType >
class AssetManager {
public:
    bool AddData( std::string strId, AssetType asset ){
	_MapAsset.emplace( strId, asset );
	return true;
    }
    bool GetData( std::string strId, AssetType & asset ){
	auto it_find = _MapAsset.find( strId );
	if( _MapAsset.end() == it_find ){
	    return false;
	}
	asset = it_find->second;
	return true;
    }
private:
    std::map< std::string, AssetType > _MapAsset;
};

template< >
class AssetManager< std::vector< double > > {
public:
    using AssetType = std::vector< double >;
    bool AddData( std::string strId, AssetType asset ){
	_MapAsset.emplace( strId, asset );
	return true;
    }
    bool GetData( std::string strId, AssetType & asset ){
	auto it_find = _MapAsset.find( strId );
	if( _MapAsset.end() == it_find ){
	    return false;
	}
	asset = it_find->second;
	return true;
    }
    bool GetDataArray( std::string strId, std::shared_ptr< double > & data_array, int & size ){
	auto it_find = _MapAsset.find( strId );
	if( _MapAsset.end() == it_find ){
	    return false;
	}
	int size_array = it_find->second.size();
	size = size_array;
	data_array = std::shared_ptr< double >( new double[ size_array ], std::default_delete<double[]>() );
	try {   
	    std::copy( it_find->second.begin(), it_find->second.end(), data_array.get() );
	}catch( ... ){
	    return false;
	}
	return true;
    }
private:
    std::map< std::string, AssetType > _MapAsset;
};

template< >
class AssetManager< std::vector< int > > {
public:
    using AssetType = std::vector< int >;
    bool AddData( std::string strId, AssetType asset ){
	_MapAsset.emplace( strId, asset );
	return true;
    }
    bool GetData( std::string strId, AssetType & asset ){
	auto it_find = _MapAsset.find( strId );
	if( _MapAsset.end() == it_find ){
	    return false;
	}
	asset = it_find->second;
	return true;
    }
    bool GetDataArray( std::string strId, std::shared_ptr< int > & data_array, int & size ){
	auto it_find = _MapAsset.find( strId );
	if( _MapAsset.end() == it_find ){
	    return false;
	}
	int size_array = it_find->second.size();
	size = size_array;
	data_array = std::shared_ptr< int >( new int[ size_array ], std::default_delete<int[]>() );
	try {   
	    std::copy( it_find->second.begin(), it_find->second.end(), data_array.get() );
	}catch( ... ){
	    return false;
	}
	return true;
    }
private:
    std::map< std::string, AssetType > _MapAsset;
};

#endif
