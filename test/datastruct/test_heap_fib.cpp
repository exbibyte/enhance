#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include "catch.hpp"

#include "heap_fib.hpp"
#include <cassert>
#include <vector>
#include <utility>
#include <list>

using namespace std;

TEST_CASE( "fib_tree", "[fib_tree]" ) {

    heap_fib<int> h;

}
