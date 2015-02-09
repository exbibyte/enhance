#ifndef TRANSMATRIX_H
#define TRANSMATRIX_H

#include <map>
#include <utility>
#include <iterator>
#include <set>
using namespace std;

template< typename KeyType >
class TransMatrixDefaultComparator
{
public:
    bool operator() ( pair< KeyType, KeyType > A, pair< KeyType, KeyType > B ){
        if( A.first < B.first ){
            return true;
        }
        else if( A.second < B.second ){
            return true;
        }
        return false;
    }
};

template< typename KeyType, typename ValueType > 
class TransMatrix {
public:
    TransMatrix(){}
    bool GetTransition( KeyType Current, KeyType Next, bool & bHasVal, ValueType & Val ); //false if non-existent, bHas == false if value not set
    bool SetTransition( KeyType Current, KeyType Next, ValueType Val ); //false if unsuccesful
    bool GetClosure( KeyType Start, KeyType End ); //false if closure is empty
private:
    map< pair<KeyType, KeyType>, bool > _MapClosure;
    map< pair< KeyType, KeyType >, pair< bool, ValueType > > _MapTransition;
};

template< typename KeyType, typename ValueType >
bool TransMatrix< KeyType, ValueType>::GetTransition( KeyType Current, KeyType Next, bool & bHasVal, ValueType & Val ){
    typename map< pair< KeyType, KeyType>, pair<bool, ValueType> >::iterator it_MapFind = _MapTransition.find( std::make_pair(Current, Next) );
    if(  it_MapFind != _MapTransition.end() ){
        bHasVal = it_MapFind->first;
        Val = it_MapFind->second;
        return true;
    }
    else{
        return false;
    }
}

template< typename KeyType, typename ValueType >
bool TransMatrix< KeyType, ValueType>::SetTransition( KeyType Current, KeyType Next, ValueType Val ){
    _MapTransition[ std::make_pair(Current, Next) ] = std::make_pair( true, Val );
    _MapClosure[ std::make_pair(Current, Next) ] = true;
    return true;
}

template< typename KeyType, typename ValueType >
bool TransMatrix< KeyType, ValueType>::GetClosure( KeyType Start, KeyType End ){

    bool bRet;
    
    typename map< pair<KeyType, KeyType>, pair< bool, ValueType> >::iterator it_MapTransitionStart = _MapTransition.begin();

    _MapClosure.clear();
    typename std::set< KeyType > KeySet;
    while( it_MapTransitionStart != _MapTransition.end() ){
        //copy transitions to closure table
        _MapClosure[ it_MapTransitionStart->first ] = true;
        //get unique keys
        KeySet.insert( it_MapTransitionStart->first.first );
        KeySet.insert( it_MapTransitionStart->first.second );
        it_MapTransitionStart++;
    }
    
    typename set< KeyType >::iterator it_KeySetIntermediate = KeySet.begin();
    while( it_KeySetIntermediate != KeySet.end() ){
        typename set< KeyType >::iterator it_KeySetStart = KeySet.begin();
        while( it_KeySetStart != KeySet.end() ){
            typename set< KeyType >::iterator it_KeySetEnd = KeySet.begin();
            while( it_KeySetEnd != KeySet.end() ){
                KeyType TempKeyStart = *it_KeySetStart;
                KeyType TempKeyIntermediate = *it_KeySetIntermediate;
                KeyType TempKeyEnd = *it_KeySetEnd;
                if( _MapClosure.find( std::make_pair( TempKeyStart, TempKeyEnd ) ) != _MapClosure.end() ){
                    _MapClosure[ std::make_pair( TempKeyStart, TempKeyEnd ) ] = true;
                }
                else if( _MapClosure.find( std::make_pair( TempKeyStart, TempKeyIntermediate ) ) != _MapClosure.end() &&
                         _MapClosure.find( std::make_pair( TempKeyIntermediate, TempKeyEnd ) ) != _MapClosure.end() ){
                    _MapClosure[ std::make_pair( TempKeyStart, TempKeyEnd ) ] = true;
                }
                it_KeySetEnd++;
            }
            it_KeySetStart++;
        }
        it_KeySetIntermediate++;   
    }

    if( _MapClosure.find( std::make_pair( Start, End ) ) != _MapClosure.end() ){
        bRet = true;
    }else{
        bRet = false;
    }
    return bRet;
}
#endif
