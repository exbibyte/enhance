#ifndef TRANSITION_MATRIX_H
#define TRANSITION_MATRIX_H

#include <map>
#include <utility>
#include <iterator>
#include <set>
using namespace std;

template< typename KeyType > 
class TransMatrix {
public:
    TransMatrix(){}
    bool GetTransition( KeyType Current, KeyType Next, int & iCost );                              //to be called after UpdateClosure(), false if closure is empty, else return path cost
    bool SetTransition( KeyType Current, KeyType Next, int iCost );                                //sets transition and replaces any existing one
    bool UpdateClosure();                                                                          //computes transitive closure and caches result
    bool GetClosure( KeyType Start, KeyType End, int & iCost, vector< KeyType > & vPathSequence ); //to be called after UpdateClosure(), false if closure is empty, else returns path cost and sequence
    bool Clear();                                                                                  //empties transitions and closures
private:
    map< pair<KeyType, KeyType>, KeyType > _MapPathNext;                                           //stores the best next path step
    map< pair<KeyType, KeyType>, bool > _MapClosure;                                               //boolean map indicating closure existence
    map< pair< KeyType, KeyType >, int > _MapTransition;                                           //cost map of closure
};

template< typename KeyType >
bool TransMatrix< KeyType >::GetTransition( KeyType Current, KeyType Next, int & iCost ){
    auto it_MapFind = _MapTransition.find( std::make_pair(Current, Next) );
    if(  it_MapFind != _MapTransition.end() ){
        iCost = it_MapFind->second;
        return true;
    }
    else{
        return false;
    }
}

template< typename KeyType >
bool TransMatrix< KeyType >::SetTransition( KeyType Current, KeyType Next, int iCost ){
    _MapTransition[ std::make_pair(Current, Next) ] = iCost;
    return true;
}

template< typename KeyType >
bool TransMatrix< KeyType >::UpdateClosure(){
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

    //Bellman-Ford-Moore algorithm for compute closure for all pairs
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
		    int iCostA = _MapTransition[ TransitionPair_A ];
		    int iCostB = _MapTransition[ TransitionPair_B ];
		    int iCostAB = iCostA + iCostB;
		    auto it_MapTransitionFind = _MapTransition.find( TransitionPair_AB );
		    //if not exist, or if we found a path that cost less, replace the existing one
		    if( it_MapTransitionFind == _MapTransition.end() || iCostAB < it_MapTransitionFind->second ){		    
		        _MapTransition[ TransitionPair_AB ] = iCostAB;
			_MapPathNext[ TransitionPair_AB ] = _MapPathNext[ TransitionPair_A ]; //update next path
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


template< typename KeyType >
bool TransMatrix< KeyType >::GetClosure( KeyType Start, KeyType End, int & iCost, vector< KeyType > & vPathSequence ){
    bool bRet;
    vPathSequence.clear();
    if( _MapClosure.find( std::make_pair( Start, End ) ) != _MapClosure.end() ){
        KeyType Current = Start;
        vPathSequence.push_back( Current );
        do{
	    Current = _MapPathNext.find( std::make_pair( Current, End ) )->second;
	    vPathSequence.push_back( Current );
	}while( Current != End );
	auto TransitionPair = std::make_pair( Start, End );
	iCost = _MapTransition[ TransitionPair ];
        bRet = true;
    }else{
        bRet = false;
    }
    return bRet;
}

template< typename KeyType >
bool TransMatrix< KeyType >::Clear(){
    _MapTransition.clear();
    _MapClosure.clear();
    _MapPathNext.clear();
    return true;
}

#endif
