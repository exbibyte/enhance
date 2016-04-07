#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file

#include <thread>
#include <vector>
#include <iostream>
#include <mutex>
#include <set>

#include "catch.hpp"
#include "HashTable.h"

using namespace std;

TEST_CASE( "HashTable", "[hashtable]" ) { 
    SECTION( "add" ) {
	HashTable<int,unsigned int> hashtable;
	hashtable.SetDefault();
	hashtable.SelectRandomHashFunc();
	for( int i = 0; i < 1000; ++i ){
	    bool bRet = hashtable.Insert( i, i*10 );
	    CHECK( bRet );
	    unsigned int val_query;
	    bRet = hashtable.Find( i, val_query );
	    CHECK( bRet );
	    CHECK( (i*10 == val_query) );
	}
    }
    SECTION( "erase" ) {
	HashTable<int,unsigned int> hashtable;
	hashtable.SetDefault();
	hashtable.SelectRandomHashFunc();
	unsigned int val_query;
        bool bRet = hashtable.Insert( 14, 100 );
	bRet = hashtable.Find( 14, val_query );
	CHECK( bRet );
	CHECK( 100 == val_query );
	
	bRet = hashtable.Erase( 14 );
	CHECK( bRet );

	bRet = hashtable.Find( 14, val_query );
	CHECK( false == bRet );
    }
}
