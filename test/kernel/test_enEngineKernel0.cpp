#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include "catch.hpp"

#include "enEngineKernel0.hpp"
#include "enComponentType.hpp"
#include "enComponentMeta.hpp"
#include "enComponentClock.hpp"
#include "enComponentLogger.hpp"
#include "enComponentScheduler.hpp"
#include "enComponentStat.hpp"
#include "enComponentThread.hpp"
#include "enComponentInit.hpp"
#include "enComponentRenderdraw.hpp"
#include "enComponentRendercompute.hpp"
#include "enComponentRenderserver.hpp"
#include "enComponentParser.hpp"
#include "enComponentCam.hpp"

#include "Funwrap3.hpp"

#include <vector>
#include <iostream>
#include <chrono>
#include <thread>
#include <ratio>
#include <memory>

#include "GLIncludes.hpp"

#include "imgui.h"
#include "imgui_impl_glfw_gl3.h"

using namespace std;

TEST_CASE( "EnEngineKernel0", "[EnEngineKernel0]" ) {

    enEngineKernel0 engine_kernel;
    CHECK( engine_kernel.get_num_components() == 0 );

    engine_kernel.init();
    CHECK( engine_kernel.get_num_components() > 0 );
	
    SECTION( "clock0, clock" ) {
	//clock
	vector<enComponentMeta*> clocks;
	engine_kernel.get_components_by_type( enComponentType::CLOCK, clocks );
	REQUIRE( clocks.size() == 1 );

	enComponentClock0 * clock0 = dynamic_cast< enComponentClock0 * >( clocks[0] );
	CHECK( nullptr != clock0 );
    }

    SECTION( "logger_stdout, logger" ) {
	//logger
	vector<enComponentMeta*> loggers;
	engine_kernel.get_components_by_type( enComponentType::LOGGER, loggers );
	REQUIRE( loggers.size() == 1 );

	COMPONENT_INSTANCE( logger_stdout, enComponentLoggerStdout, loggers.front() );
	logger_stdout->Log( "Engine Kernel {%s} initialized.", "0" );

	enComponentLoggerStdout * logger = dynamic_cast< enComponentLoggerStdout * >( loggers.front() );
	CHECK( nullptr != logger );

    }
    //todo: catch causes problems with multithreaded program
    // SECTION( "thread0, thread" ) {
    // 	//thread
    // 	vector<enComponentMeta*> threads;
    // 	engine_kernel.get_components_by_type( enComponentType::THREAD, threads );
    // 	REQUIRE( threads.size() == 1 );
    // 	COMPONENT_INSTANCE( thread0, enComponentThread0, threads.front() );
    // 	CHECK( IThread::State::STOPPED == thread0->getstate() );
    // 	{
    // 	    for(int j = 0; j < 3; ++j ){
    // 		bool i = false;
    // 		CHECK( i == false );
    // 		Funwrap3 f;
    // 		f.set( FunCallType::ASYNC, [&]{ i = true; } );
    // 		std::function<void(int)> fun_set_bool = [&](int){
    // 		    f.apply();
    // 		};
    // 		thread0->settask( fun_set_bool );
    // 		while( !thread0->setaction( IThread::Action::START ) );
    // 		std::this_thread::sleep_for(std::chrono::milliseconds(10));
    // 		while( !thread0->setaction( IThread::Action::END ) );
    // 		CHECK( i == true );
    // 	    }
    // 	}
    // }
    //todo: catch causes problems with multithreaded program
    // SECTION( "scheduler0, scheduler" ) {

	//scheduler
	vector<enComponentMeta*> schedulers;
	engine_kernel.get_components_by_type( enComponentType::SCHEDULER, schedulers );
	REQUIRE( schedulers.size() == 1 );
	COMPONENT_INSTANCE( scheduler0, enComponentScheduler0, schedulers.front() );
	{
	    Funwrap3 f;
	    std::function<void()> fun_print = [](){
		// std::cout << "function called." << std::endl;
	    };
	    f.set( FunCallType::ASYNC, fun_print );
	    scheduler0->add_task( e_scheduler_priority::medium, f );
	    std::this_thread::sleep_for(std::chrono::milliseconds(10));

	    while( scheduler0->get_num_tasks() > 0 ){
		size_t c = scheduler0->get_num_tasks();
		// std::cout << "remaining tasks: " << c << std::endl;
	    }
	}
    // }
    SECTION( "stat0, stat" ) {
	//stat
	vector<enComponentMeta*> stats;
	engine_kernel.get_components_by_type( enComponentType::STAT, stats );
	REQUIRE( stats.size() == 1 );
	COMPONENT_INSTANCE( stat0, enComponentStat0, stats.front() );
    }
    SECTION( "initGL/renderdraw0/renderserver0, init renderdraw renderserver" ) {
	//initGL
	vector<enComponentMeta*> inits;
	engine_kernel.get_components_by_type( enComponentType::INIT, inits );
	REQUIRE( inits.size() == 1 );
	COMPONENT_INSTANCE( initGL, enComponentInitGL, inits.front() );

	//renderdraw0
	vector<enComponentMeta*> renderdraws;
	engine_kernel.get_components_by_type( enComponentType::RENDERDRAW, renderdraws );
	assert( renderdraws.size() == 1 );
	COMPONENT_INSTANCE( renderdraw0, enComponentRenderdraw0, renderdraws.front() );

	//renderserver0
	vector<enComponentMeta*> renderservers;
	engine_kernel.get_components_by_type( enComponentType::RENDERSERVER, renderservers );
	assert( renderservers.size() == 1 );
	COMPONENT_INSTANCE( renderserver0, enComponentRenderserver0, renderservers.front() );
    }
    SECTION( "parsermd5", "parser" ){
	//parsermd5
	vector<enComponentMeta*> parsers;
	engine_kernel.get_components_by_type( enComponentType::PARSER, parsers );
	assert( parsers.size() == 2 );
	COMPONENT_INSTANCE( parsermd5, enComponentParserMd5, parsers.front() );
    }
    SECTION( "parserobj", "parser" ){
	//parserobj
	vector<enComponentMeta*> parsers;
	engine_kernel.get_components_by_type( enComponentType::PARSER, parsers );
	assert( parsers.size() == 2 );
	COMPONENT_INSTANCE( parserobj, enComponentParserObj, parsers[1] );
    }
    SECTION( "cam0", "cam" ){
	//camera with ui
	vector<enComponentMeta*> cams;
	engine_kernel.get_components_by_type( enComponentType::CAM, cams );
	assert( cams.size() == 1 );
	COMPONENT_INSTANCE( cam, enComponentCam0, cams.front() );
    }

    engine_kernel.deinit();
}

