#include <functional>
#include <vector>
#include <map>
#include <utility>
#include <cstdint>

#include "enEngineKernelAbstract.hpp"
#include "enComponentMeta.hpp"

bool enEngineKernelAbstract::register_component( enComponentMeta * component, uint_fast32_t cid ){
    if( !component )
	return false;
    component->SaveKernelInfo( this, _cid );
    _components[ std::pair<enComponentType,uint_fast32_t>( component->get_component_type(), _cid) ] = component;
    return true;
}

enComponentMeta * enEngineKernelAbstract::get_component( enComponentType type, uint_fast32_t cid ){
  auto it_find = _components.find( std::pair<enComponentType, uint_fast32_t>( type, cid) );
    if( _components.end() == it_find )
	return nullptr;
    return it_find->second;
}

bool enEngineKernelAbstract::remove_component_all(){
    _components.clear();
    return true;
}

enEngineKernelAbstract * enEngineKernelAbstract::get_core_engine(){
    return this;
}

void enEngineKernelAbstract::accumulate_components( std::function<void(enComponentMeta*,std::vector<enComponentMeta*>&)> f, std::vector< enComponentMeta * > & accum ){
    for( auto & i : _components ){
	f( i.second, accum );
    }
}

int enEngineKernelAbstract::get_num_components() const {
    return _components.size();
}
