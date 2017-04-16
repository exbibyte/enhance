#include <cassert>

#include "enEngineKernel0.hpp"

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
#include "enComponentUi.hpp"
#include "enComponentFilter.hpp"
#include "enComponentOrientationmanip.hpp"
#include "enComponentCam.hpp"

void enEngineKernel0::init(){
    //TODO: consider bulk memory allocation
    register_component( new enComponentClock0( new Clock0 ) );
    register_component( new enComponentLoggerStdout( new LoggerStdout ) );

    //create and run scheduler
    Scheduler0 * sch = new Scheduler0;
    //3 pools for different priorities
    std::list<e_scheduler_priority> p { e_scheduler_priority::low, e_scheduler_priority::medium, e_scheduler_priority::high };
    std::pair<bool,void*> r = sch->set_pools( p );
    assert( true == r.first );
    //3 threads
    std::list<e_scheduler_priority> t { e_scheduler_priority::low, e_scheduler_priority::medium, e_scheduler_priority::high };
    r = sch->set_threads( t );
    assert( true == r.first );
    register_component( new enComponentScheduler0( sch ) );
    auto ret = sch->run();
    assert( ret.first );
	
    register_component( new enComponentStat0( new Stat0 ) );
    register_component( new enComponentThread0( new Thread0 ) );
    register_component( new enComponentInitGL( new InitGL ) );
    register_component( new enComponentRenderdraw0( new Renderdraw0 ) );
    register_component( new enComponentRendercompute0( new Rendercompute0 ) );
    register_component( new enComponentRenderserver0( new Renderserver0 ) );
    register_component( new enComponentParserMd5( new ParserMd5 ) );
    register_component( new enComponentParserObj( new ParserObj ) );
    register_component( new enComponentUi0( new Ui0 ) );
    register_component( new enComponentFilterUiDrag( new FilterUiDrag ) );
    register_component( new enComponentOrientationmanip0( new Orientationmanip0 ) );
    register_component( new enComponentCam0( new cam0 ) );
}

void enEngineKernel0::deinit(){
    std::vector< enComponentMeta * > accum;
    accumulate_components( [](enComponentMeta * x){
                               if( x != nullptr )
				   return true;
			       else
				   return false;
	                     },
	                     accum );
    for( auto & i : accum ){
        delete i;
    }
    remove_component_all();
}
