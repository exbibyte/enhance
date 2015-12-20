#ifndef INSTANCEMANAGER_H
#define INSTANCEMANAGER_H

#include "AssetManager.h"
#include "GenSeq.h"
#include "IdentityGen.h"

#include <cassert>
#include <map>
#include <vector>

#include "GenSeq.h"
#include "Injection.h"
#include "IdentityGen.h"

// template< typename InstType >
// class InstanceManager { };

template< typename... InstTypes >
class InstanceManager : public AssetManager< unsigned int, std::vector< int > > {
public:
    using AssetManager< unsigned int, std::vector< int > >::AddData;
    using AssetManager< unsigned int, std::vector< int > >::GetData;
    using AssetManager< unsigned int, std::vector< int > >::GetDataArray;
    Inject< IdentityGen< InstTypes >... > _inject_instance_types;
    static constexpr int _num_types = sizeof...(InstTypes);
    InstanceManager() : _inject_instance_types( GenSeq::GenSequence< Inject >( IdentityGen< InstTypes >()... ) ){
    }
    template< typename QueryInstType >
    bool QueryValue( int id, int & val ){
	int index_in_entry = _inject_instance_types.GetIdSeq(IdentityGen<QueryInstType>::_identity);
	std::vector< int > entry;
	if( !GetData( id, entry ) ){
	    assert( 0 && "InstanceManager::GetData() failed" );
	    return false;
	}
	if( entry.size() <= index_in_entry ){
	    assert( 0 && "InstanceManager::GetData() retreived entry size too small for lookup index" );
	    return false;
	}
	val = entry.at( index_in_entry );
	return true;
    }
    template< typename QueryInstType >
    bool SetValue( int id, int val ){
	int index_in_entry = _inject_instance_types.GetIdSeq(IdentityGen<QueryInstType>::_identity);
	std::vector< int > entry;
	entry.resize( _num_types );
	GetData( id, entry ); //obtain existing data if any or else use newly initialized entry
	if( entry.size() <= index_in_entry ){
	    assert( 0 && "InstanceManager::GetData() retreived entry size too small for lookup index" );
	    return false;
	}
	entry[index_in_entry] = val;
	AddData( id, entry ); //save data
	return true;
    }
};

#endif
