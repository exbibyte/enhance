#include "enEngineKernel0.hpp"

#include "enComponentClock.hpp"

void enEngineKernel0::init(){
    register_component( new enComponentClock0( new Clock0 ) );
}

void enEngineKernel0::deinit(){
    std::vector< enComponentMeta * > accum;
    accumulate_components( [](enComponentMeta * x, std::vector<enComponentMeta*>& y){
	    if( x != nullptr )
	        y.push_back(x);
	}, accum );
    for( auto & i : accum ){
	delete i;
    }
    remove_component_all();
}
