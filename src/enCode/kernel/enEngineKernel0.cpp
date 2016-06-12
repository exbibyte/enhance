#include "enEngineKernel0.hpp"

enEngineKernel0::Init(){
    
}

enEngineKernel0::Deinit(){
    for( auto & i : _components ){
	if( i.second != nullptr ){
	    delete i.second;
	}
    }
}
