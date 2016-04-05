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
    HashTable<int> hashtable;
    SECTION( "test" ) {
	hashtable.Insert(99);
	HashTable<int>::iterator it = hashtable.find(99);
	CHECK( 99 == *it );
    }
}
