#include "TreeRb.h"
#include <cassert>
#include <vector>
#include <utility>
using namespace std;

class DataContainer{
    int data;
};

int FuncCompareInt(int a, int b){
    if(a < b){
        return 1;
    }else if( a > b){
        return -1;
    }else{
        return 0;
    }
}

int main(){

    int (*FuncCompare)(int,int) = &FuncCompareInt;
    
    DataContainer container;
    DataContainer container1;
    DataContainer container2;

    CTreeNodeRb<int, DataContainer> * pNode = new CTreeNodeRb<int, DataContainer>( 3, &container1 );
    CTreeNodeRb<int, DataContainer> * pNode1 = new CTreeNodeRb<int, DataContainer>( 4, &container1 );
    CTreeNodeRb<int, DataContainer> * pNode2 = new CTreeNodeRb<int, DataContainer>( 6, &container2 );
    CTreeNodeRb<int, DataContainer> * pNode3 = new CTreeNodeRb<int, DataContainer>( 1, &container2 );
    CTreeNodeRb<int, DataContainer> * pNode4 = new CTreeNodeRb<int, DataContainer>( 5, &container2 );
    CTreeNodeRb<int, DataContainer> * pNode5 = new CTreeNodeRb<int, DataContainer>( 0, &container2 );
    
    vector< CTreeNodeRb<int, DataContainer> * > v_pNodes { pNode, pNode1, pNode2, pNode3, pNode4, pNode5 };

    CTreeNodeRb<int, DataContainer>::ConvertToRoot(pNode);
    
    CTreeNodeRb<int, DataContainer>::SetFuncRbCompareKey( FuncCompare );

    CTreeNodeRb<int, DataContainer> * pNodeFound;
    bool bFound = CTreeNodeRb<int, DataContainer>::Find( pNode, 3, pNodeFound);

    assert( bFound );

    bFound = CTreeNodeRb<int, DataContainer>::Find( pNode, 2, pNodeFound);
    assert( !bFound );

    bool bInsert = CTreeNodeRb<int, DataContainer>::Insert( pNode, pNode1 ); //4
    assert( bInsert );
    bInsert = CTreeNodeRb<int, DataContainer>::Insert( pNode, pNode2 ); //6
    assert( bInsert );
    bInsert = CTreeNodeRb<int, DataContainer>::Insert( pNode, pNode3 );
    assert( bInsert );
    bInsert = CTreeNodeRb<int, DataContainer>::Insert( pNode, pNode4 );
    assert( bInsert );
    bInsert = CTreeNodeRb<int, DataContainer>::Insert( pNode, pNode5 );
    assert( bInsert );

    bFound = CTreeNodeRb<int, DataContainer>::Find( pNode, 4, pNodeFound);
    assert( bFound );
    bFound = CTreeNodeRb<int, DataContainer>::Find( pNode, 5, pNodeFound);
    assert( bFound );
    bFound = CTreeNodeRb<int, DataContainer>::Find( pNode, 1, pNodeFound);
    assert( bFound );
    bFound = CTreeNodeRb<int, DataContainer>::Find( pNode, 6, pNodeFound);
    assert( bFound );
    bFound = CTreeNodeRb<int, DataContainer>::Find( pNode, 0, pNodeFound);
    assert( bFound );

    CTreeNodeRb<int, DataContainer> * NodeRoot;
    bFound = CTreeNodeRb<int, DataContainer>::GetRoot( pNode, NodeRoot );
    vector< pair<int, eRbColour> > KeysResult;
    bool bGet = CTreeNodeRb<int, DataContainer>::GetKeysSubTreeInOrder( NodeRoot, KeysResult );    
    assert( KeysResult[0].first == 0 );
    assert( KeysResult[1].first == 1 );
    assert( KeysResult[2].first == 3 );
    assert( KeysResult[3].first == 4 );
    assert( KeysResult[4].first == 5 );
    assert( KeysResult[5].first == 6 );

    int RootKey;
    eRbColour RootColour;
    NodeRoot->GetKeyColour( RootKey, RootColour );
    cout << "Root key: " << RootKey << ", colour: " << ( (eRbColour_Black == RootColour)?"Black":"Red" ) << endl;
    for( auto i : KeysResult ){
        cout << "Key: " << i.first << ", Colour:" << ( (i.second == eRbColour_Black)?"Black":"Red" ) << endl; 
    }

    KeysResult.clear();
    bool bRemove = CTreeNodeRb<int, DataContainer>::Remove( pNode3 ); //remove 1 
    bRemove = CTreeNodeRb<int, DataContainer>::Remove( pNode2 ); //remove 6
    bFound = CTreeNodeRb<int, DataContainer>::GetRoot( pNode, NodeRoot );
    bGet = CTreeNodeRb<int, DataContainer>::GetKeysSubTreeInOrder( NodeRoot, KeysResult );    
    assert( KeysResult[0].first == 0 );
    assert( KeysResult[1].first == 3 );
    assert( KeysResult[2].first == 4 );
    assert( KeysResult[3].first == 5 );

    for( auto * i : v_pNodes ){
        delete i;
        i = 0;
    }
    v_pNodes.clear();

    return 0;
}
