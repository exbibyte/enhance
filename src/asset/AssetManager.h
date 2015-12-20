#ifndef ASSETMANAGER_H
#define ASSETMANAGER_H

#include <vector>
#include <memory>
#include <string>
#include <utility>
#include <algorithm>
#include <cassert>

template< class IdType, class Asset >
class AssetManager {
public:
    using AssetType = Asset;
    bool AddData( IdType id, Asset asset ){
        _MapAsset[ id ] = asset;
	return true;
    }
    bool GetData( IdType id, Asset & asset ){
	auto it_find = _MapAsset.find( id );
	if( _MapAsset.end() == it_find ){
	    return false;
	}
	asset = it_find->second;
	return true;
    }
private:
    std::map< IdType, AssetType > _MapAsset;
};

template< class IdType >
class AssetManager< IdType, std::vector< double > > {
public:
    using AssetType = std::vector< double >;
    using ElementType = double;
    bool AddData( IdType id, AssetType asset ){
	_MapAsset[ id ] = asset;
	return true;
    }
    bool GetData( IdType id, AssetType & asset ){
	auto it_find = _MapAsset.find( id );
	if( _MapAsset.end() == it_find ){
	    return false;
	}
	asset = it_find->second;
	return true;
    }
    bool GetDataArray( IdType id, std::shared_ptr< ElementType > & data_array, int & size ){
	auto it_find = _MapAsset.find( id );
	if( _MapAsset.end() == it_find ){
	    return false;
	}
	int size_array = it_find->second.size();
	size = size_array;
	data_array = std::shared_ptr< ElementType >( new ElementType[ size_array ], std::default_delete<double[]>() );
	try {   
	    std::copy( it_find->second.begin(), it_find->second.end(), data_array.get() );
	}catch( ... ){
	    return false;
	}
	return true;
    }
private:
    std::map< IdType, AssetType > _MapAsset;
};

template< class IdType >
class AssetManager< IdType, std::vector< int > > {
public:
    using AssetType = std::vector< int >;
    using ElementType = int;
    bool AddData( IdType id, AssetType asset ){
	_MapAsset[ id ] = asset;
	return true;
    }
    bool GetData( IdType id, AssetType & asset ){
	auto it_find = _MapAsset.find( id );
	if( _MapAsset.end() == it_find ){
	    return false;
	}
	asset = it_find->second;
	return true;
    }
    bool GetDataArray( IdType id, std::shared_ptr< ElementType > & data_array, int & size ){
	auto it_find = _MapAsset.find( id );
	if( _MapAsset.end() == it_find ){
	    return false;
	}
	int size_array = it_find->second.size();
	size = size_array;
	data_array = std::shared_ptr< ElementType >( new ElementType[ size_array ], std::default_delete<int[]>() );
	try {   
	    std::copy( it_find->second.begin(), it_find->second.end(), data_array.get() );
	}catch( ... ){
	    return false;
	}
	return true;
    }
private:
    std::map< IdType, AssetType > _MapAsset;
};

template< class IdType >
class AssetManager< IdType, std::vector< float > > {
public:
    using AssetType = std::vector< float >;
    using ElementType = float;
    bool AddData( IdType id, AssetType asset ){
	_MapAsset[ id ] = asset;
	return true;
    }
    bool GetData( IdType id, AssetType & asset ){
	auto it_find = _MapAsset.find( id );
	if( _MapAsset.end() == it_find ){
	    return false;
	}
	asset = it_find->second;
	return true;
    }
    bool GetDataArray( IdType id, std::shared_ptr< ElementType > & data_array, int & size ){
	auto it_find = _MapAsset.find( id );
	if( _MapAsset.end() == it_find ){
	    return false;
	}
	int size_array = it_find->second.size();
	size = size_array;
	data_array = std::shared_ptr< ElementType >( new ElementType[ size_array ], std::default_delete<ElementType[]>() );
	try {   
	    std::copy( it_find->second.begin(), it_find->second.end(), data_array.get() );
	}catch( ... ){
	    return false;
	}
	return true;
    }
private:
    std::map< IdType, AssetType > _MapAsset;
};

#endif
