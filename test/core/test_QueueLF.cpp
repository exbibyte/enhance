#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file

#include <thread>
#include <vector>
#include <iostream>
#include <mutex>
#include <set>
#include <chrono>

#include "catch.hpp"
#include "QueueLF.h"

using namespace std;

TEST_CASE( "QueueLF", "[queue]" ) { 

    QueueLF<int> queue;

    SECTION( "push" ) {
	size_t count = queue.size();
	CHECK( 0 == count );
	queue.push(5);
	count = queue.size();
	CHECK( 1 == count );

	SECTION( "pop" ) {
	    int retrieve;
	    bool bRet = queue.pop( retrieve );
	    count = queue.size();
	    CHECK( 0 == count );
	    CHECK( true == bRet );
	    CHECK( 5 == retrieve );
	}
    }    

    SECTION( "pop empty" ) {
    	int retrieve;
    	size_t count;
    	bool bRet = queue.pop( retrieve );
    	count = queue.size();
    	CHECK( 0 == count );
    	CHECK( false == bRet );
    }

    
    SECTION( "multi-thread push" ) {
    	size_t count;
    	unsigned int num_threads = 10;
    	vector<thread> threads( num_threads );
    	for( int i = 0; i < num_threads; ++i ){
    	    threads[i] = std::thread( [ &, i ](){
    		    queue.push( i );
    		} );
	    std::this_thread::sleep_for (std::chrono::milliseconds(75)); //timed enforcement of insertion of items in numerical order
    	}
    	for( auto & i : threads ){
    	    i.join();
    	}
    	count = queue.size();
    	CHECK( num_threads == count );

        SECTION( "multi-thread pop" ) {
	    vector<int> vals_retrieve;
    	    mutex mtx;
    	    for( auto & i : threads ){
    		i = std::thread( [&](){
    			int pop_val;
    			bool bRet = queue.pop( pop_val );
    			mtx.lock();
    			if( bRet ){
    			    vals_retrieve.push_back( pop_val );
    			}
    			mtx.unlock();
    		    } );
    	    }
    	    for( auto & i : threads ){
    		i.join();
    	    }
    	    count = queue.size();
    	    CHECK( 0 == count );
            REQUIRE( num_threads == vals_retrieve.size() );
    	    for( int i = 0; i < num_threads; ++i ){
    	        int val_retrieve = vals_retrieve[i];
		CHECK( i == val_retrieve );
	    }
         }
     }
}
