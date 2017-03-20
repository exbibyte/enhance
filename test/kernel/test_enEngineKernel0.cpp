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

#include "Funwrap3.hpp"

#include <vector>
#include <iostream>
#include <chrono>
#include <thread>
#include <ratio>
#include <memory>

#include "GLIncludes.hpp"

#include "PassParsePolyMesh.h"
#include "PassConvertPolyMeshDataStructToArray.h"
#include "DataTransformDriver.h"
#include "PolyMesh_Data_Arrays.h"

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
    SECTION( "thread0, thread" ) {
	//thread
	vector<enComponentMeta*> threads;
	engine_kernel.get_components_by_type( enComponentType::THREAD, threads );
	REQUIRE( threads.size() == 1 );
	COMPONENT_INSTANCE( thread0, enComponentThread0, threads.front() );
	CHECK( IThread::State::STOPPED == thread0->getstate() );
	{
	    for(int j = 0; j < 3; ++j ){
		bool i = false;
		CHECK( i == false );
		Funwrap3 f;
		f.set( FunCallType::ASYNC, [&]{ i = true; } );
		std::function<void()> fun_set_bool = [&](){
		    f.apply();
		};
		thread0->settask( fun_set_bool );
		while( !thread0->setaction( IThread::Action::START ) );
		std::this_thread::sleep_for(std::chrono::milliseconds(10));
		while( !thread0->setaction( IThread::Action::END ) );
		CHECK( i == true );
	    }
	}
    }
    SECTION( "scheduler0, scheduler" ) {

	//scheduler
	vector<enComponentMeta*> schedulers;
	engine_kernel.get_components_by_type( enComponentType::SCHEDULER, schedulers );
	REQUIRE( schedulers.size() == 1 );
	COMPONENT_INSTANCE( scheduler0, enComponentScheduler0, schedulers.front() );
	{
	    scheduler0->run();
	    Funwrap3 f;
	    std::function<void()> fun_print = []()->bool{
		cout << "function called." << endl;
		return true;
	    };
	    f.set( FunCallType::ASYNC, fun_print );
	    scheduler0->add( f );

	}
	Funwrap3 g;
	std::this_thread::sleep_for(std::chrono::milliseconds(10));
	scheduler0->get( g );
	g.apply();
	scheduler0->stop();
    }
    SECTION( "stat0, stat" ) {
	//stat
	vector<enComponentMeta*> stats;
	engine_kernel.get_components_by_type( enComponentType::STAT, stats );
	REQUIRE( stats.size() == 1 );
	COMPONENT_INSTANCE( stat0, enComponentStat0, stats.front() );
	CHECK( 0 < stat0->getstat().size() );
	std::cout << stat0->getstat() << std::endl;
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
    SECTION( "parserpolymesh0", "parser" ){
	//parserpolymesh0
	vector<enComponentMeta*> parserpolymeshes;
	engine_kernel.get_components_by_type( enComponentType::PARSER, parserpolymeshes );
	assert( parserpolymeshes.size() == 2 );
	COMPONENT_INSTANCE( parserpolymesh0, enComponentParserPolymesh0, parserpolymeshes.front() );
	COMPONENT_INSTANCE( parsermd5, enComponentParserMd5, parserpolymeshes[1] );
    }

    engine_kernel.deinit();
}

