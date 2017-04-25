#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file

#include "catch.hpp"

#include "queue_lockfree_sync.hpp"
#include "basic_pool_test.hpp"

using namespace e2::dsc;

TEST_CASE( "queue_lockfree_sync bulk operations", "[bulk]" ) { 
    queue_lockfree_sync<int> queue;
    unsigned int num_threads = 100;
    basic_pool_test::queue_sync_bulk_operation( num_threads, queue );
}
TEST_CASE( "queue_lockfree_sync bulk operations reversed", "[bulk_rev]" ) { 
    queue_lockfree_sync<int> queue;
    unsigned int num_threads = 100;
    basic_pool_test::queue_sync_bulk_operation_reversed( num_threads, queue );
}
TEST_CASE( "queue_lockfree_sync interleaved operations", "[interleaved]" ) {
    queue_lockfree_sync<int> queue;
    unsigned int num_threads = 100;
    basic_pool_test::queue_sync_interleaved_operation( num_threads, queue );
}
