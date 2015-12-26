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


template< >
class InstanceManager< int > : public AssetManager< unsigned int, int > {
public:
    using DataType = int;
    using AssetManager< unsigned int, DataType >::AddData;
    using AssetManager< unsigned int, DataType >::GetData;
    static constexpr int _num_types = 0;
    InstanceManager(){}
    bool QueryValue( int id, DataType & val ){
	if( !GetData( id, val ) ){
	    assert( 0 && "InstanceManager::GetData() failed" );
	    return false;
	}
	return true;
    }
    bool SetValue( int id, DataType val ){
	AddData( id, val ); //save data
	return true;
    }
};

template< >
class InstanceManager< double > : public AssetManager< unsigned int, double > {
public:
    using DataType = double;
    using AssetManager< unsigned int, DataType >::AddData;
    using AssetManager< unsigned int, DataType >::GetData;
    static constexpr int _num_types = 0;
    InstanceManager(){}
    bool QueryValue( int id, DataType & val ){
	if( !GetData( id, val ) ){
	    assert( 0 && "InstanceManager::GetData() failed" );
	    return false;
	}
	return true;
    }
    bool SetValue( int id, DataType val ){
	AddData( id, val ); //save data
	return true;
    }
};

template< >
class InstanceManager< float > : public AssetManager< unsigned int, float > {
public:
    using DataType = float;
    using AssetManager< unsigned int, DataType >::AddData;
    using AssetManager< unsigned int, DataType >::GetData;
    static constexpr int _num_types = 0;
    InstanceManager(){}
    bool QueryValue( int id, DataType & val ){
	if( !GetData( id, val ) ){
	    assert( 0 && "InstanceManager::GetData() failed" );
	    return false;
	}
	return true;
    }
    bool SetValue( int id, DataType val ){
	AddData( id, val ); //save data
	return true;
    }
};

template< >
class InstanceManager< std::vector<int> > : public AssetManager< unsigned int, std::vector<int> > {
public:
    using DataType = std::vector<int>;
    using AssetManager< unsigned int, DataType >::AddData;
    using AssetManager< unsigned int, DataType >::GetData;
    using AssetManager< unsigned int, DataType >::GetDataArray;
    static constexpr int _num_types = 0;
    InstanceManager(){}
    bool QueryValue( int id, DataType & val ){
	if( !GetData( id, val ) ){
	    assert( 0 && "InstanceManager::GetData() failed" );
	    return false;
	}
	return true;
    }
    bool SetValue( int id, DataType val ){
	AddData( id, val ); //save data
	return true;
    }
};

template< >
class InstanceManager< std::vector<double> > : public AssetManager< unsigned int, std::vector<double> > {
public:
    using DataType = std::vector<double>;
    using AssetManager< unsigned int, DataType >::AddData;
    using AssetManager< unsigned int, DataType >::GetData;
    using AssetManager< unsigned int, DataType >::GetDataArray;
    static constexpr int _num_types = 0;
    InstanceManager(){}
    bool QueryValue( int id, DataType & val ){
	if( !GetData( id, val ) ){
	    assert( 0 && "InstanceManager::GetData() failed" );
	    return false;
	}
	return true;
    }
    bool SetValue( int id, DataType val ){
        AddData( id, val ); //save data
	return true;
    }
};

template< >
class InstanceManager< std::vector<float> > : public AssetManager< unsigned int, std::vector<float> > {
public:
    using DataType = std::vector<float>;
    using AssetManager< unsigned int, DataType >::AddData;
    using AssetManager< unsigned int, DataType >::GetData;
    using AssetManager< unsigned int, DataType >::GetDataArray;
    static constexpr int _num_types = 0;
    InstanceManager(){}
    bool QueryValue( int id, DataType & val ){
	if( !GetData( id, val ) ){
	    assert( 0 && "InstanceManager::GetData() failed" );
	    return false;
	}
	return true;
    }
    bool SetValue( int id, DataType val ){
        AddData( id, val ); //save data
	return true;
    }
};

#endif
