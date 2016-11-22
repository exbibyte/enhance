#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file

#include <thread>
#include <vector>
#include <iostream>
#include <mutex>
#include <set>
#include "catch.hpp"
#include "exchanger_lockfree.hpp"

#include <chrono>
#include <thread>

using namespace std;

TEST_CASE( "exchanger_lockfree", "[exchanger]" ) { 
    exchanger_lockfree<int> ex;
    long timeout_us = 10000000;

    int val1 = 10;
    int val2 = 20;
    bool ret1 = false;
    bool ret2 = false;

    thread t1 = std::thread( [&](){
	    ret1 = ex.exchange( val1, timeout_us );
	} );
    std::this_thread::sleep_for(std::chrono::milliseconds(3000));
    thread t2 = std::thread( [&](){
	    ret2 = ex.exchange( val2, timeout_us );
	} );

    t1.join();
    t2.join();
    std::cout << "threads joined." << std::endl;
    CHECK( true == ret1 );
    CHECK( true == ret2 );
    CHECK( 20 == val1 );
    CHECK( 10 == val2 );
}
