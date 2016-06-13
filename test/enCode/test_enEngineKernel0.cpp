#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include "catch.hpp"

#include "enEngineKernel0.hpp"
#include "enComponentType.hpp"
#include "enComponentMeta.hpp"
#include "enComponentClock.hpp"

#include <vector>

using namespace std;

TEST_CASE( "EnEngineKernel0", "[EnEngineKernel0]" ) {
    SECTION( "RegisterComponent, GetComponent" ) {

	enEngineKernel0 engine_kernel;
	CHECK( engine_kernel.get_num_components() == 0 );

	engine_kernel.register_component( new enComponentClock0( new Clock0 ) );
	CHECK( engine_kernel.get_num_components() == 1 );

	vector<enComponentMeta*> clocks;
	engine_kernel.get_components_by_type( enComponentType::CLOCK, clocks );
	REQUIRE( clocks.size() == 1 );

	enComponentClock0 * clock0 = dynamic_cast< enComponentClock0 * >( clocks[0] );
	CHECK( nullptr != clock0 );
    }
}
