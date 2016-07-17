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

#include "Funwrap3.hpp"

#include <vector>
#include <iostream>
#include <chrono>
#include <thread>

#include "GLIncludes.hpp"

using namespace std;

TEST_CASE( "EnEngineKernel0", "[EnEngineKernel0]" ) {

    enEngineKernel0 engine_kernel;
    CHECK( engine_kernel.get_num_components() == 0 );

    engine_kernel.init();
    CHECK( engine_kernel.get_num_components() > 0 );
	
    SECTION( "clock, clock" ) {
	//clock
	vector<enComponentMeta*> clocks;
	engine_kernel.get_components_by_type( enComponentType::CLOCK, clocks );
	REQUIRE( clocks.size() == 1 );

	enComponentClock0 * clock0 = dynamic_cast< enComponentClock0 * >( clocks[0] );
	CHECK( nullptr != clock0 );
    }

    SECTION( "logger, logger" ) {
	//logger
	vector<enComponentMeta*> loggers;
	engine_kernel.get_components_by_type( enComponentType::LOGGER, loggers );
	REQUIRE( loggers.size() == 1 );

	COMPONENT_INSTANCE( logger_stdout, enComponentLoggerStdout, loggers.front() );
	logger_stdout->Log( "Engine Kernel {%s} initialized.", "0" );

	enComponentLoggerStdout * logger = dynamic_cast< enComponentLoggerStdout * >( loggers.front() );
	CHECK( nullptr != logger );

    }
    SECTION( "thread, thread" ) {
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
    SECTION( "scheduler, scheduler" ) {

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
    SECTION( "stat, stat" ) {
	//stat
	vector<enComponentMeta*> stats;
	engine_kernel.get_components_by_type( enComponentType::STAT, stats );
	REQUIRE( stats.size() == 1 );
	COMPONENT_INSTANCE( stat0, enComponentStat0, stats.front() );
	CHECK( 0 < stat0->getstat().size() );
	std::cout << stat0->getstat() << std::endl;
    }
    SECTION( "initGL, initGL" ) {
	//initGL
	vector<enComponentMeta*> inits;
	engine_kernel.get_components_by_type( enComponentType::INIT, inits );
	REQUIRE( inits.size() == 1 );
	COMPONENT_INSTANCE( initGL, enComponentInitGL, inits.front() );
	bool bret = initGL->init();
	CHECK( true == bret );

	std::this_thread::sleep_for(std::chrono::milliseconds(2000));

	SECTION( "GLSLProgram, GLSLProgram" ) {
	    GLSLProgram * glslprogram = new GLSLProgram;
	    std::cout << "glslprogram allocated." << std::endl;
	    delete glslprogram;
	}
    }
    // SECTION( "renderdraw0, renderdraw0" ) {
    // 	//renderdraw0
    // 	vector<enComponentMeta*> renderdraws;
    // 	engine_kernel.get_components_by_type( enComponentType::RENDERDRAW, renderdraws );
    // 	REQUIRE( renderdraws.size() == 1 );
    // 	COMPONENT_INSTANCE( renderdraw0, enComponentRenderdraw0, renderdraws.front() );
    // 	renderdraw0->render( RenderData() );
    // }
    engine_kernel.deinit();
}
