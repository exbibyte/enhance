#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file

#include <thread>
#include <vector>
#include <iostream>
#include <mutex>
#include <set>

#include "catch.hpp"
#include "hashtable_lock_striped.hpp"

using namespace std;

TEST_CASE( "hashtable_lock_striped", "[hashtable_lock_striped]" ) { 
    SECTION( "add" ) {
	hashtable_lock_striped< int, int > hashtable(200, 0.33);
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
    	hashtable_lock_striped< int, int > hashtable(200, 0.33);
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
    SECTION( "resize" ) {
    	hashtable_lock_striped< int, int > hashtable(10, 0.33);
    	for( int i = 0; i < 10; ++i ){
    	    bool bRet = hashtable.insert( i, i*10 );
    	    CHECK( bRet );
    	    int val_query;
    	    bRet = hashtable.find( i, val_query );
    	    CHECK( bRet );
    	    CHECK( (i*10 == val_query) );
    	}
    	CHECK( (true == hashtable.resize(2)) );
    	for( int i = 0; i < 10; ++i ){
    	    int val_query;
    	    bool bRet = hashtable.find( i, val_query );
    	    CHECK( bRet );
    	    CHECK( (i*10 == val_query) );
    	}
    }
    SECTION( "load factor" ) {
    	hashtable_lock_striped< int, int > hashtable(200, 0.33);
    	double error = 0.0001;
    	CHECK( hashtable.get_load_factor() >= 0 - error );
    	CHECK( hashtable.get_load_factor() <= 0 + error );
	
    	for( int i = 0; i < 1000; ++i ){
    	    hashtable.insert( i, i*10 );
    	}

    	CHECK( hashtable.get_load_factor() >= 5 - error );
    	CHECK( hashtable.get_load_factor() <= 5 + error );

    	CHECK( (true == hashtable.resize(100)) );

    	CHECK( hashtable.get_load_factor() >= 10 - error );
    	CHECK( hashtable.get_load_factor() <= 10 + error );
    }
}

TEST_CASE( "hashtable_lock_striped multithread", "[hashtable_lock_striped]" ) { 
    SECTION( "add" ) {
	hashtable_lock_striped< int, int > hashtable(200, 0.33);
	size_t n = 1000;
	std::vector<int> inserted( n, -1 );
	std::function<void(int)> f = [&](int i){
	    bool bRet = hashtable.insert( i, i );
	};
	std::vector< std::thread > t(n);
	for( size_t j = 0; j < n; ++j ){
	    t[j] = std::thread( f, j );
	}
	for( size_t j = 0; j < n; ++j ){
	    t[j].join();
	}
	for( size_t j = 0; j < n; ++j ){
	    int val_query;
	    bool bRet = hashtable.find( j, val_query );
	    CHECK( val_query == j );
	}
    }
    SECTION( "add and erase" ) {
    	hashtable_lock_striped< int, int > hashtable(200, 0.33);
    	size_t n = 1000;
    	std::function<void(int)> f = [&](int i){
    	    bool bRet = hashtable.insert( i, i );
    	};
    	std::function<void(int)> f_remove = [&](int i){
    	    bool bRet = hashtable.erase( i );
    	};
    	std::vector< std::thread > t(n);
    	for( size_t j = 0; j < n; ++j ){
    	    t[j] = std::thread( f, j );
    	}
    	for( size_t j = 0; j < n; ++j ){
    	    t[j].join();
    	}
    	for( size_t j = 0; j < n; ++j ){
    	    t[j] = std::thread( f_remove, j );
    	}
	for( size_t j = 0; j < n; ++j ){
    	    t[j].join();
    	}
    	std::vector<int> inserted( n, -1 );
	int count = 0;
    	for( size_t j = 0; j < n; ++j ){
    	    int val_query;
    	    bool bRet = hashtable.find( j, val_query );
    	    if( val_query != j ){
		++count;
	    }
    	}
	std::cout << "removed % of inserted: " << (double)count*100.0/n << std::endl;
    }
    // SECTION( "resize" ) {
    // 	hashtable_lock_striped< int, int > hashtable(200, 0.33);
    // 	size_t n = 1000;
    // 	std::function<void(int)> f = [&](int i){
    // 	    bool bRet = hashtable.insert( i, i );
    // 	};
    // 	std::vector< std::thread > t(n);
    // 	for( size_t j = 0; j < n; ++j ){
    // 	    t[j] = std::thread( f, j );
    // 	}
    // 	std::thread t_resize = std::thread( [&](){ hashtable.resize(100); } );
    // 	for( size_t j = 0; j < n; ++j ){
    // 	    t[j].join();
    // 	}
    // 	t_resize.join();
    // 	for( size_t j = 0; j < n; ++j ){
    // 	    int val_query;
    // 	    bool bRet = hashtable.find( j, val_query );
    // 	    CHECK( val_query == j );
    // 	}
    // 	double error = 0.0001;
    // 	CHECK( hashtable.get_load_factor() >= 10 - error );
    // 	CHECK( hashtable.get_load_factor() <= 10 + error );
    // }
}
