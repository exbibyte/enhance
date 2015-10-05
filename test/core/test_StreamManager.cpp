#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include "catch.hpp"

#include "StreamChannel.h"
#include "StreamManager.h"

#include <list>
#include <utility>
#include <string>
#include <iostream>
using namespace std;

TEST_CASE( "StreamManager", "[StreamManager]" ) {
    StreamManager stream_manager;
    StreamChannel * stream_ui = nullptr;
    StreamChannel * stream_logic = nullptr;
    StreamChannel * stream_graphics = nullptr;
    SECTION( "Get stream channels" ) {
	CHECK( stream_manager.SelectStream( StreamType::UI, stream_ui ) == true );
	CHECK( stream_manager.SelectStream( StreamType::LOGIC, stream_logic ) == true );
	CHECK( stream_manager.SelectStream( StreamType::GRAPHICS, stream_graphics ) == true );
	CHECK( stream_ui != nullptr );
	CHECK( stream_logic != nullptr );
	CHECK( stream_graphics != nullptr );
    }
}
