#include "enEngineKernel0.hpp"

#include "enComponentClock.hpp"
#include "enComponentLogger.hpp"

void enEngineKernel0::init(){
    register_component( new enComponentClock0( new Clock0 ) );
    register_component( new enComponentLoggerStdout( new LoggerStdout ) );
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
