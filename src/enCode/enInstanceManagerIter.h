#ifndef EN_INSTANCEMANAGER_ITER_H
#define EN_INSTANCEMANAGER_ITER_H

#include <utility>
#include <map>
#include <vector>

#include "enAssetManager.h"
#include "InstanceType.h"
#include "InstanceManagerIter.h"

class enInstanceManagerIterPackage {
public:
    using enInstanceManagerIter = InstanceManagerIter< eInstanceType, enAssetManager >;
    ~enInstanceManagerIterPackage(){
	for( auto & i : _map_manager ){
	    delete i.second;
	}
    }
    bool CreateManager( eInstanceType manager_instance_type, std::vector< eInstanceType > attributes ){
	if( _map_manager.end() != _map_manager.find( manager_instance_type ) ){
	    //already exists
	    return false;
	}else{
	    _map_manager[ manager_instance_type ] = new enInstanceManagerIter( attributes );
	}
	return true;
    }
    bool LinkManagers(){ //link all existing managers
	for( auto & i :_map_manager ){
	    std::vector< eInstanceType > attributes;
	    i.second->GetDataTypes( attributes );
	    std::vector< std::pair<eInstanceType, enInstanceManagerIter * > > attributes_to_link;
	    for( auto & j : attributes ){
		auto it_find_manager = _map_manager.find( j );
		if( _map_manager.end() != it_find_manager ){ //only link found managers
		    std::pair< eInstanceType, enInstanceManagerIter * > link_info  { j, it_find_manager->second };
		    attributes_to_link.push_back( link_info );
		}
	    }
	    i.second->LinkExternalInstanceManager( attributes_to_link );
	}
	return true;
    }
    bool GetManager( eInstanceType manager_instance_type, enInstanceManagerIter * & manager ){
	auto it_find_manager = _map_manager.find( manager_instance_type );
	if( _map_manager.end() == it_find_manager ){
	    //doesn't exist
	    return false;
	}else{
	    manager = it_find_manager->second;
	}
	return true;
    }
private:
    std::map< eInstanceType, enInstanceManagerIter * > _map_manager;
};

#endif
