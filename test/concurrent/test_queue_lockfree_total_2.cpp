#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file

#include "queue_lockfree_total.hpp"

#include "catch.hpp"

#include <thread>
#include <vector>
#include <iostream>
#include <mutex>
#include <set>
#include <cassert>
#include <chrono>

using namespace std;

TEST_CASE( "queue_lockfree_total multithread push pop", "[push pop]" ) {
    
    queue_lockfree_total<int> queue;
    unsigned int num_threads = 11700;
    vector<thread> threads2( num_threads );
    vector<thread> threads( num_threads );    
    int count_loop = 5;
    while( --count_loop >=0 ){
	size_t count;
	auto t0 = std::chrono::high_resolution_clock::now();
	for( int i = 0; i < num_threads; ++i ){
	    threads[i] = std::thread( [ &, i ](){
		    int val = i;
		    queue.put( val );
		} );
	}
	count = queue.size();
	// assert( 0 <= count && "expected queue size after push not matched." );
	CHECK( 0 <= count );
	for( int i = 0; i < num_threads; ++i ){
	    threads2[i] = std::thread( [&](){
		    int pop_val;
		    bool bRet = queue.get( pop_val );
		    if( bRet ){
			// std::cout << pop_val << " ";
		    }
		} );
	}
	for( auto & i : threads ){
	    i.join();
	}
	for( int i = 0; i < num_threads; ++i ){
	    threads2[i].join();
	}
	cout << endl;
	count = queue.size();
	// assert( (0 ==  count ) && "expected queue size after pop not matched." );
	CHECK( 0 ==  count );
	auto t1 = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double> dur = t1 - t0;
	auto dur_ms = std::chrono::duration_cast<std::chrono::milliseconds>(dur);
	cout << "duration: " << dur_ms.count() << " ms. rate: " <<  (double)num_threads/dur_ms.count()*1000.0 << " push-pop/sec." << endl;
    }

}
