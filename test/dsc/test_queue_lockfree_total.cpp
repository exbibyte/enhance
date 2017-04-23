
#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file

#include <thread>
#include <vector>
#include <iostream>
#include <mutex>
#include <set>

#include "catch.hpp"

#include "basic_pool_test.hpp"
#include "queue_lockfree_total.hpp"

using namespace std;
using namespace e2::dsc;


TEST_CASE( "queue_lockfree_total put get operations", "[queue]" ) { 
    queue_lockfree_total<int> queue;
    unsigned int num_threads = 100;
    basic_pool_test::queue_put_get( num_threads, queue );
}

TEST_CASE( "queue_lockfree_total bulk operations", "[queue]" ) { 
    queue_lockfree_total<int> queue;
    unsigned int num_threads = 100;
    basic_pool_test::queue_bulk_operation( num_threads, queue );
}
