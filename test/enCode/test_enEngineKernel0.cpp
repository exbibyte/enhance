#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include "catch.hpp"

#include "enEngineKernel0.hpp"
#include "enComponentType.hpp"
#include "enComponentMeta.hpp"
#include "enComponentClock.hpp"
#include "enComponentLogger.hpp"

#include <vector>

using namespace std;

TEST_CASE( "EnEngineKernel0", "[EnEngineKernel0]" ) {
    SECTION( "RegisterComponent, GetComponent" ) {

	enEngineKernel0 engine_kernel;
	CHECK( engine_kernel.get_num_components() == 0 );

	engine_kernel.init();
	CHECK( engine_kernel.get_num_components() == 2 );

	//clock
	vector<enComponentMeta*> clocks;
	engine_kernel.get_components_by_type( enComponentType::CLOCK, clocks );
	REQUIRE( clocks.size() == 1 );

	enComponentClock0 * clock0 = dynamic_cast< enComponentClock0 * >( clocks[0] );
	CHECK( nullptr != clock0 );

	//logger
	vector<enComponentMeta*> loggers;
	engine_kernel.get_components_by_type( enComponentType::LOGGER, loggers );
	REQUIRE( loggers.size() == 1 );

	COMPONENT_INSTANCE( logger_stdout, enComponentLoggerStdout, loggers.front() );
	logger_stdout->Log( "Engine Kernel {%s} initialized.", "0" );

	
	engine_kernel.deinit();
    }
}
