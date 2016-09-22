#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include "catch.hpp"

#include "avl_tree.hpp"
#include <cassert>
#include <vector>
#include <utility>
#include <list>

using namespace std;

TEST_CASE( "avl_tree", "[avl_tree]" ) {

    list< tree_node_avl<int, int> * > gc{};
    tree_node_avl<int, int> * root = new tree_node_avl<int, int>( 10, 100 );
    gc.push_back(root);
    tree_node_avl<int, int> * found = nullptr;
    bool bRet = tree_node_avl<int, int>::find( root, 10, found );
    CHECK( bRet );
    CHECK( nullptr != found );
    if( bRet && found ){
	int key = found->get_key();
	CHECK( 10 == key );
	int val = found->get_val();
	CHECK( 100 == val );
    }
    
    found = nullptr;
    bRet = tree_node_avl<int, int>::get_root( root, found );
    CHECK( bRet );
    CHECK( nullptr != found );
    CHECK( found == root );

    tree_node_avl<int, int> * n1 = new tree_node_avl<int, int>( 5, 25 );
    gc.push_back(n1);
    tree_node_avl<int, int> * n2 = new tree_node_avl<int, int>( 15, 225 );
    gc.push_back(n2);

    bRet = tree_node_avl<int, int>::insert( root, n1 );
    CHECK( bRet );
    bRet = tree_node_avl<int, int>::insert( root, n2 );
    CHECK( bRet );

    //expected to be found
    found = nullptr;
    bRet = tree_node_avl<int, int>::find( root, 15, found );
    CHECK( bRet );
    CHECK( nullptr != found );
    if( bRet && found ){
	int key = found->get_key();
	CHECK( 15 == key );
	int val = found->get_val();
	CHECK( 225 == val );

	//expect next of 15 to be unsuccessful
	tree_node_avl<int,int> * next;
	bRet = tree_node_avl<int, int>::next( found, next );
	CHECK( !bRet );
	//expect prev of 15 to be successful and return node of 10
	tree_node_avl<int,int> * prev;
	bRet = tree_node_avl<int, int>::prev( found, prev );
	CHECK( bRet );
	REQUIRE( nullptr != prev );
	int key_prev = prev->get_key();
	CHECK( 10 == key_prev );
    }

    //expected to be found
    found = nullptr;
    bRet = tree_node_avl<int, int>::find( root, 5, found );
    CHECK( bRet );
    CHECK( nullptr != found );
    if( bRet && found ){
	int key = found->get_key();
	CHECK( 5 == key );
	int val = found->get_val();
	CHECK( 25 == val );

	//expect next of 5 to be successful and return node of 10
	tree_node_avl<int,int> * next;
	bRet = tree_node_avl<int, int>::next( found, next );
	CHECK( bRet );
	REQUIRE( nullptr != next );
	int key_next = next->get_key();
	CHECK( 10 == key_next );
	//expect prev of 5 to be unsuccessful
	tree_node_avl<int,int> * prev;
	bRet = tree_node_avl<int, int>::prev( found, prev );
	CHECK( !bRet );
    }

    //check 6 is absent
    found = nullptr;
    bRet = tree_node_avl<int, int>::find( root, 6, found );
    CHECK( !bRet );
    CHECK( nullptr == found );

    //check root is valid
    bRet = tree_node_avl<int, int>::get_root( root, found );
    CHECK( bRet );
    CHECK( root == found );

    //remove non-existent node in tree
    tree_node_avl<int, int> * n3 = new tree_node_avl<int, int>();
    gc.push_back( n3 );
    bRet = tree_node_avl<int, int>::remove( root, n3 );
    CHECK( !bRet );

    // //remove 5
    // bRet = tree_node_avl<int, int>::find( root, 5, found );
    // CHECK( bRet );
    // CHECK( nullptr != found );
    // bRet = tree_node_avl<int, int>::remove( root, found );
    // CHECK( bRet );

    // //check 5 is absent
    // bRet = tree_node_avl<int, int>::find( root, 5, found );
    // CHECK( !bRet );
    // CHECK( nullptr == found );

    //insert 20
    tree_node_avl<int, int> * n4 = new tree_node_avl<int, int>( 20, 400 );
    gc.push_back(n4);
    bRet = tree_node_avl<int, int>::insert( root, n4 );
    bRet = tree_node_avl<int, int>::find( root, 20, found );
    CHECK( bRet );
    CHECK( nullptr != found );

    // //remove 15
    // bRet = tree_node_avl<int, int>::find( root, 15, found );
    // CHECK( bRet );
    // CHECK( nullptr != found );
    // bRet = tree_node_avl<int, int>::remove( root, found );
    // CHECK( bRet );

    // //checek 15 is absent
    // bRet = tree_node_avl<int, int>::find( root, 15, found );
    // CHECK( !bRet );
    // CHECK( nullptr == found );
    //check 20 is present
    bRet = tree_node_avl<int, int>::find( root, 20, found );
    CHECK( bRet );
    CHECK( nullptr != found );

    //remove 10
    bRet = tree_node_avl<int, int>::find( root, 10, found );
    CHECK( bRet );
    CHECK( nullptr != found );
    bRet = tree_node_avl<int, int>::remove( root, found );
    CHECK( bRet );
    //check 10 is absent
    bRet = tree_node_avl<int, int>::find( root, 10, found );
    CHECK( !bRet );
    CHECK( nullptr == found );
    //check 20 is present
    bRet = tree_node_avl<int, int>::find( root, 20, found );
    CHECK( bRet );
    CHECK( nullptr != found );

    for( auto & i : gc ){
	delete i;
    }
}
