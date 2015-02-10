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
    bool GetTransition( KeyType Current, KeyType Next, int & iCost, ValueType & Val ); //false if non-existent
    bool SetTransition( KeyType Current, KeyType Next, int iCost, ValueType Val ); //false if unsuccesful
    bool UpdateClosure(); //computes transitive closure and caches result
    bool GetClosure( KeyType Start, KeyType End, int & iCost, vector< KeyType > & vPathSequence ); //false if closure is empty, else returns path cost and sequence
    bool Clear();
private:
    map< pair<KeyType, KeyType>, KeyType > _MapPathNext; // next path
    map< pair<KeyType, KeyType>, bool > _MapClosure;
    map< pair< KeyType, KeyType >, pair< int, ValueType > > _MapTransition; //transition containing transition cost, and a stored value
};

template< typename KeyType, typename ValueType >
bool TransMatrix< KeyType, ValueType>::GetTransition( KeyType Current, KeyType Next, int & iCost, ValueType & Val ){
    typename map< pair< KeyType, KeyType>, pair<bool, ValueType> >::iterator it_MapFind = _MapTransition.find( std::make_pair(Current, Next) );
    if(  it_MapFind != _MapTransition.end() ){
        iCost = it_MapFind->first;
        Val = it_MapFind->second;
        return true;
    }
    else{
        return false;
    }
}

template< typename KeyType, typename ValueType >
bool TransMatrix< KeyType, ValueType>::SetTransition( KeyType Current, KeyType Next, int iCost, ValueType Val ){
    _MapTransition[ std::make_pair(Current, Next) ] = std::make_pair( iCost, Val );
    return true;
}

template< typename KeyType, typename ValueType >
bool TransMatrix< KeyType, ValueType>::UpdateClosure(){
    auto it_MapTransitionStart = _MapTransition.begin();
    _MapPathNext.clear();
    _MapClosure.clear();
    typename std::set< KeyType > KeySet;
    while( it_MapTransitionStart != _MapTransition.end() ){
        KeyType TransitionStart = it_MapTransitionStart->first.first;
	KeyType TransitionEnd = it_MapTransitionStart->first.second;
        //copy current transition to closure table
        _MapClosure[ it_MapTransitionStart->first ] = true;
	//save current path to next state
	_MapPathNext[ it_MapTransitionStart->first ] = TransitionEnd;
        //save unique keys
        KeySet.insert( TransitionStart );
        KeySet.insert( TransitionEnd );
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
		auto TransitionPair_A = std::make_pair( TempKeyStart, TempKeyIntermediate );
		auto TransitionPair_B = std::make_pair( TempKeyIntermediate, TempKeyEnd );
		auto TransitionPair_AB = std::make_pair( TempKeyStart, TempKeyEnd );
		if( _MapClosure.find( TransitionPair_A ) != _MapClosure.end() &&
		    _MapClosure.find( TransitionPair_B ) != _MapClosure.end() ){ //check transivity
    
	  	    _MapClosure[ TransitionPair_AB ] = true;
		    int iCostA = _MapTransition[ TransitionPair_A ].first;
		    int iCostB = _MapTransition[ TransitionPair_B ].first;
		    int iCostAB = iCostA + iCostB;
		    ValueType empty;
		    if( _MapTransition.find( TransitionPair_AB ) == _MapTransition.end() ){ //if not exist, make a transition
		        _MapTransition[ TransitionPair_AB ] = std::make_pair( iCostAB, empty );
			_MapPathNext[ TransitionPair_AB ] = _MapPathNext[ TransitionPair_A ]; //update next path
		    }else{
		      int iExistingCost = _MapTransition.find( TransitionPair_AB )->second.first; //if current found cost less, replace the existing one
		        if( iCostAB < iExistingCost ){
		            _MapTransition[ TransitionPair_AB ] = std::make_pair( iCostAB, empty );
			    _MapPathNext[ TransitionPair_AB ] = _MapPathNext[ TransitionPair_A ];
		      }
		    }		   
                }
                it_KeySetEnd++;
            }
            it_KeySetStart++;
        }
        it_KeySetIntermediate++;   
    }
    return true;
}


template< typename KeyType, typename ValueType >
bool TransMatrix< KeyType, ValueType>::GetClosure( KeyType Start, KeyType End, int & iCost, vector< KeyType > & vPathSequence ){
    bool bRet;
    vPathSequence.clear();
    if( _MapClosure.find( std::make_pair( Start, End ) ) != _MapClosure.end() ){
        KeyType Current = Start;
        vPathSequence.push_back( Current );
        while( Current != End ){
	    Current = _MapPathNext.find( std::make_pair( Current, End ) )->second;
	    vPathSequence.push_back( Current );
	}
        bRet = true;
    }else{
        bRet = false;
    }
    return bRet;
}

template< typename KeyType, typename ValueType >
bool TransMatrix< KeyType, ValueType>::Clear(){
    _MapTransition.clear();
    _MapClosure.clear();
    _MapPathNext.clear();
    return true;
}

#endif
