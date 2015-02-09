//=====================================================================
//
// File:                TransitionTable.h
//
// Subsystem:   
//
// Summary:             Implements a template transition table 
//
// Notes:               Looks up a pair<KeyType Source, KeyType Destination> and gives stored DataType data.
//                      More complex pair structures might need custom comparator in the future. However, standard datatype works ok with default comparator.                  
//=============================================================================

#ifndef TRANSITIONTABLE_H
#define TRANSITIONTABLE_H

#include <map>
#include <utility>

using namespace std;

template< typename KeyType, typename DataType >
class CTransitionTable{
public:
    CTransitionTable();
    ~CTransitionTable();
    bool GetTransition( KeyType Source, KeyType Destination, DataType & Data ) const; // false if transition doesn't exist
    bool SetTransition( KeyType Source, KeyType Destination, DataType Data, int Weight = 1 );
    bool GetWeight( KeyType Source, KeyType Destination, int & Weight ) const; //return false if transition doesn't exist
    bool SetWeight( KeyType Source, KeyType Destination, int Weight );
    bool GetMapWeight( map< pair<KeyType, KeyType> , int > & ) const; //copy transition weight mapping
private:
    map< pair<KeyType, KeyType> , DataType > mMapTransition; //transition table container: Map< Pair<KeyType Source, KeyType Destination > , DataType Data >
    map< pair<KeyType, KeyType> , int > mMapWeight; //transition table container: Map< Pair<KeyType Source, KeyType Destination > , int PathWeight >
};

template< typename KeyType, typename DataType >
CTransitionTable< typename KeyType, typename DataType >::CTransitionTable()
{
    mMapTransition.clear();
}

template< typename KeyType, typename DataType >
CTransitionTable< typename KeyType, typename DataType >::~CTransitionTable()
{
    mMapTransition.clear();
}

template< typename KeyType, typename DataType >
bool CTransitionTable< typename KeyType, typename DataType >::GetWeight( KeyType Source, KeyType Destination, int & Weight ) const
{
    map< pair<KeyType, KeyType> , int >::const_iterator it = mMapWeight.find( std::make_pair( Source, Destination ) );
    if( it == mMapWeight.end() )
    {
        return false;
    }
    else
    {
        Weight = it->second;
        return true;    
    }
}

template< typename KeyType, typename DataType >
bool CTransitionTable< typename KeyType, typename DataType >::SetWeight( KeyType Source, KeyType Destination, int Weight )
{
    mMapWeight[ std::make_pair( Source, Destination ) ] = Weight;
    return true;
}

template< typename KeyType, typename DataType >
bool CTransitionTable< typename KeyType, typename DataType >::GetTransition( KeyType Source, KeyType Destination, DataType & Data ) const
{
    map< pair<KeyType, KeyType> , DataType >::const_iterator it = mMapTransition.find( std::make_pair( Source, Destination ) );
    if( it == mMapTransition.end() )
    {
        return false;
    }
    else
    {
        Data = it->second;
        return true;    
    }
}

template< typename KeyType, typename DataType >
bool CTransitionTable< typename KeyType, typename DataType >::SetTransition( KeyType Source, KeyType Destination, DataType Data, int Weight )
{
    mMapTransition[ std::make_pair( Source, Destination ) ] = Data;
    SetWeight( Source, Destination, Weight );
    return true;
}

template< typename KeyType, typename DataType >
bool CTransitionTable< typename KeyType, typename DataType >::GetMapWeight( map< pair<KeyType, KeyType> , int > & MapWeight ) const
{
    MapWeight = mMapWeight;
    return true;
}

#endif
