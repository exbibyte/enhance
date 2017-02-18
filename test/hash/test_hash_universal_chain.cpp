#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file

#include <thread>
#include <vector>
#include <iostream>
#include <mutex>
#include <set>

#include "catch.hpp"
#include "hash_universal_chain.hpp"

using namespace std;

TEST_CASE( "hash_universal_chain", "[hash_universal_chain]" ) { 
    SECTION( "add" ) {
	hash_universal_chain< int, int > hashtable;
	for( int i = 0; i < 1000; ++i ){
	    bool bRet = hashtable.insert( i, i*10 );
	    CHECK( bRet );
	    int val_query;
	    bRet = hashtable.find( i, val_query );
	    CHECK( bRet );
	    CHECK( (i*10 == val_query) );
	}
    }
    SECTION( "erase" ) {
	hash_universal_chain< int, int > hashtable;
	int val_query;
        bool bRet = hashtable.insert( 14, 100 );
	bRet = hashtable.find( 14, val_query );
	CHECK( bRet );
	CHECK( 100 == val_query );
	
	bRet = hashtable.erase( 14 );
	CHECK( bRet );

	bRet = hashtable.find( 14, val_query );
	CHECK( false == bRet );
    }
}
