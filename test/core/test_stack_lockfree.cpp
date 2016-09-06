#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file

#include <thread>
#include <vector>
#include <iostream>
#include <mutex>
#include <set>

#include "catch.hpp"
#include "stack_lockfree.hpp"

using namespace std;

TEST_CASE( "stack_lockfree", "[stack]" ) { 

    stack_lockfree<int> stack;

    SECTION( "push" ) {
	size_t count = stack.size();
	CHECK( 0 == count );
	stack.push(5);
	count = stack.size();
	CHECK( 1 == count );

	SECTION( "pop" ) {
	    int retrieve;
	    bool bRet = stack.pop( retrieve );
	    count = stack.size();
	    CHECK( 0 == count );
	    CHECK( true == bRet );
	    CHECK( 5 == retrieve );
	}

	SECTION( "top" ) {
	    int retrieve;
	    count = stack.size();
	    CHECK( 1 == count );
	    bool bRet = stack.top( retrieve );
	    count = stack.size();
	    CHECK( 1 == count );
	    CHECK( true == bRet );
	    CHECK( 5 == retrieve );
	}
    }    

    SECTION( "pop empty" ) {
	int retrieve;
	size_t count;
	bool bRet = stack.pop( retrieve );
	count = stack.size();
	CHECK( 0 == count );
	CHECK( false == bRet );
    }

    SECTION( "multi-thread push" ) {
	size_t count;
	unsigned int num_threads = 10;
	vector<thread> threads( num_threads );
	for( int i = 0; i < num_threads; ++i ){
	    threads[i] = std::thread( [ &stack, i ](){
		    stack.push( i );
		} );
	}
	for( auto & i : threads ){
	    i.join();
	}
	count = stack.size();
	CHECK( num_threads == count );

	SECTION( "multi-thread pop" ) {
	    set<int> vals_retrieve;
	    mutex mtx;
	    for( auto & i : threads ){
		i = std::thread( [&](){
			int pop_val;
			bool bRet = stack.pop( pop_val );
			mtx.lock();
			if( bRet ){
			    vals_retrieve.insert( pop_val );
			}
			mtx.unlock();
		    } );
	    }
	    for( auto & i : threads ){
		i.join();
	    }
	    count = stack.size();
	    CHECK( 0 == count );
	    for( int i = 0; i < num_threads; ++i ){
		auto it = vals_retrieve.find(i);
		CHECK( vals_retrieve.end() != it );
		if( vals_retrieve.end() != it )
		    vals_retrieve.erase(it);
	    }
	}
    }
}
