#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file

#include <thread>
#include <vector>
#include <iostream>
#include <mutex>
#include <set>

#include "catch.hpp"

#include "basic_pool_test.hpp"
#include "stack_lockfree_total_simple.hpp"

using namespace std;
using namespace e2::dsc;

TEST_CASE( "stack_lockfree_total_simple", "[stack]" ) { 

    stack_lockfree_total_simple<int> stack;
    unsigned int num_threads = 10;
    basic_pool_test::stack_put_get_int( num_threads, stack );
}
