#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file

#include "catch.hpp"

#include "stack_lockfree_split_reference.hpp"
#include "basic_pool_test.hpp"

TEST_CASE( "stack_lockfree_split_reference", "[basic]" ) {
    e2::dsc::stack_lockfree_split_reference<int> stack;
    unsigned int num_threads = 10;
    basic_pool_test::stack_put_get_int( num_threads, stack );
}
