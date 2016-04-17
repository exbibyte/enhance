#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file

#include <thread>
#include <vector>
#include <iostream>
#include <mutex>
#include <set>

#include "catch.hpp"
#include "QueueLF.h"

using namespace std;

TEST_CASE( "QueueLF", "[queue]" ) { 

    QueueLF<int> queue;

    SECTION( "push" ) {
	size_t count = queue.size();
	CHECK( 0 == count );
	int val = 5;
	queue.push(val);
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
	while(1){
    	size_t count;
    	unsigned int num_threads = 100;
    	vector<thread> threads( num_threads );
    	for( int i = 0; i < num_threads; ++i ){
    	    threads[i] = std::thread( [ &, i ](){
		    int val = i;
    		    queue.push( val );
    		} );
    	}
    	// count = queue.size();
    	// CHECK( num_threads == count );

        // SECTION( "multi-thread pop" ) {
    	vector<thread> threads2( num_threads );
	set<int> vals_retrieve;
	    // mutex mtx;
	    // for( auto & i : threads ){
	    for( int i = 0; i < num_threads * 0.1; ++i ){
		// i = std::thread( [&](){
		threads2[i] = std::thread( [&](){
			int pop_val;
			bool bRet = queue.pop( pop_val );
			// mtx.lock();
			if( bRet ){
			    // vals_retrieve.insert( pop_val );
			    std::cout << pop_val << std::endl;
			}
			// mtx.unlock();
		    } );
	    }
	    // for( auto & i : threads ){
	    for( int i = 0; i < num_threads * 0.1; ++i ){
		// i.join();
		threads2[i].join();
	    }
	    for( auto & i : threads ){
		i.join();
	    }
	    // count = queue.size();
	    // CHECK( 0 == count );
	    // for( int i = 0; i < num_threads; ++i ){
	    // 	auto it = vals_retrieve.find(i);
	    // 	CHECK( vals_retrieve.end() != it );
	    // 	if( vals_retrieve.end() != it )
	    // 	    vals_retrieve.erase(it);
	    // }
         // }
	}
     }
}
