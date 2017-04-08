#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include "catch.hpp"

#include "red_black_tree.hpp"
#include <cassert>
#include <vector>
#include <utility>
using namespace std;

class DataContainer{
    int data;
};

int FuncCompareInt(int const & a, int const & b){
    if(a < b){
        return 1;
    }else if( a > b){
        return -1;
    }else{
        return 0;
    }
}

TEST_CASE( "red_black_tree", "[red_black_tree]" ) {

    int (*FuncCompare)(int const & ,int const & ) = &FuncCompareInt;
    
    DataContainer container;
    DataContainer container1;
    DataContainer container2;

    tree_node_rb<int, DataContainer> * pNode = new tree_node_rb<int, DataContainer>( 3, &container1 );
    tree_node_rb<int, DataContainer> * pNode1 = new tree_node_rb<int, DataContainer>( 4, &container1 );
    tree_node_rb<int, DataContainer> * pNode2 = new tree_node_rb<int, DataContainer>( 6, &container2 );
    tree_node_rb<int, DataContainer> * pNode3 = new tree_node_rb<int, DataContainer>( 1, &container2 );
    tree_node_rb<int, DataContainer> * pNode4 = new tree_node_rb<int, DataContainer>( 5, &container2 );
    tree_node_rb<int, DataContainer> * pNode5 = new tree_node_rb<int, DataContainer>( 0, &container2 );
    
    vector< tree_node_rb<int, DataContainer> * > v_pNodes { pNode, pNode1, pNode2, pNode3, pNode4, pNode5 };

    tree_node_rb<int, DataContainer>::ConvertToRoot(pNode);
    
    tree_node_rb<int, DataContainer>::SetFuncRbCompareKey( FuncCompare );

    tree_node_rb<int, DataContainer> * pNodeFound;
    bool bFound = tree_node_rb<int, DataContainer>::Find( pNode, 3, pNodeFound);

    CHECK( bFound );

    bFound = tree_node_rb<int, DataContainer>::Find( pNode, 2, pNodeFound);
    CHECK( !bFound );

    bool bInsert = tree_node_rb<int, DataContainer>::Insert( pNode, pNode1 ); //4
    CHECK( bInsert );
    bInsert = tree_node_rb<int, DataContainer>::Insert( pNode, pNode2 ); //6
    CHECK( bInsert );
    bInsert = tree_node_rb<int, DataContainer>::Insert( pNode, pNode3 );
    CHECK( bInsert );
    bInsert = tree_node_rb<int, DataContainer>::Insert( pNode, pNode4 );
    CHECK( bInsert );
    bInsert = tree_node_rb<int, DataContainer>::Insert( pNode, pNode5 );
    CHECK( bInsert );

    bFound = tree_node_rb<int, DataContainer>::Find( pNode, 4, pNodeFound);
    CHECK( bFound );
    bFound = tree_node_rb<int, DataContainer>::Find( pNode, 5, pNodeFound);
    CHECK( bFound );
    bFound = tree_node_rb<int, DataContainer>::Find( pNode, 1, pNodeFound);
    CHECK( bFound );
    bFound = tree_node_rb<int, DataContainer>::Find( pNode, 6, pNodeFound);
    CHECK( bFound );
    bFound = tree_node_rb<int, DataContainer>::Find( pNode, 0, pNodeFound);
    CHECK( bFound );

    tree_node_rb<int, DataContainer> * NodeRoot;
    bFound = tree_node_rb<int, DataContainer>::GetRoot( pNode, NodeRoot );
    vector< pair<int, eRbColour> > KeysResult;
    bool bGet = tree_node_rb<int, DataContainer>::GetKeysSubTreeInOrder( NodeRoot, KeysResult );    
    CHECK( KeysResult[0].first == 0 );
    CHECK( KeysResult[1].first == 1 );
    CHECK( KeysResult[2].first == 3 );
    CHECK( KeysResult[3].first == 4 );
    CHECK( KeysResult[4].first == 5 );
    CHECK( KeysResult[5].first == 6 );

    int RootKey;
    eRbColour RootColour;
    NodeRoot->GetKeyColour( RootKey, RootColour );
    cout << "Root key: " << RootKey << ", colour: " << ( (eRbColour_Black == RootColour)?"Black":"Red" ) << endl;
    for( auto i : KeysResult ){
        cout << "Key: " << i.first << ", Colour:" << ( (i.second == eRbColour_Black)?"Black":"Red" ) << endl; 
    }

    KeysResult.clear();
    bool bRemove = tree_node_rb<int, DataContainer>::Remove( pNode3 ); //remove 1 
    bRemove = tree_node_rb<int, DataContainer>::Remove( pNode2 ); //remove 6
    bFound = tree_node_rb<int, DataContainer>::GetRoot( pNode, NodeRoot );
    bGet = tree_node_rb<int, DataContainer>::GetKeysSubTreeInOrder( NodeRoot, KeysResult );    
    CHECK( KeysResult[0].first == 0 );
    CHECK( KeysResult[1].first == 3 );
    CHECK( KeysResult[2].first == 4 );
    CHECK( KeysResult[3].first == 5 );

    for( auto * i : v_pNodes ){
        delete i;
        i = 0;
    }
    v_pNodes.clear();

}
