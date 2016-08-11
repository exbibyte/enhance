#include <functional>
#include <vector>
#include <map>
#include <utility>
#include <cstdint>

#include "enEngineKernelAbstract.hpp"
#include "enComponentMeta.hpp"

bool enEngineKernelAbstract::register_component( enComponentMeta * component ){
    if( !component )
	return false;
    component->SaveKernelInfo( this, _cid );
    _components[ std::pair<enComponentType, uint32_t>( component->get_component_type(), _cid) ] = component;
    ++_cid;
    return true;
}

enComponentMeta * enEngineKernelAbstract::get_component( enComponentType type, uint32_t cid ){
  auto it_find = _components.find( std::pair<enComponentType, uint32_t>( type, cid) );
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

void enEngineKernelAbstract::accumulate_components( std::function<bool(enComponentMeta*)> f, std::vector< enComponentMeta * > & accum ){
    for( auto & i : _components ){
	if( f( i.second ) )
	    accum.push_back( i.second );
    }
}

void enEngineKernelAbstract::get_components_by_type( enComponentType type, std::vector< enComponentMeta * > & accum ){
    auto f = [=]( enComponentMeta * x )->bool{
	if( x->get_component_type() == type )
	    return true;
	else
	    return false;
    };
    accumulate_components( f, accum );
}

void enEngineKernelAbstract::get_components_by_strid( char const * strid, std::vector< enComponentMeta * > & accum ){
    auto f = [=]( enComponentMeta * x )->bool{
	if( 0 == strcmp( x->get_strid(), strid ) )
	    return true;
	else
	    return false;
    };
    accumulate_components( f, accum );
}

int enEngineKernelAbstract::get_num_components() const {
    return _components.size();
}
