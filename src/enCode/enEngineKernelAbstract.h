#ifndef EN_ENGINE_KERNEL_ABSTRACT_H
#define EN_ENGINE_KERNEL_ABSTRACT_H

#include "enComponentType.h"

#include <map>
#include <functional>
#include <cstdint>
#include <vector>

//interface for core engine
class enEngineKernelAbstract {
public:
    enEngineKernelAbstract(){}
    ~enEngineKernelAbstract(){}
    template< class ComponentClass >
    bool RegisterComponent( ComponentClass * component, uint_fast32_t id = 0 );
    template< class ComponentClass >
    ComponentClass * GetComponent( enComponentType type, uint_fast32_t id = 0 );
    bool RemoveComponentAll();
    enEngineKernelAbstract * GetCoreEngine();
    std::map< std::pair<enComponentType,uint_fast32_t>, void * > _components;
    uint_fast32_t _cid = 0; //component id

    //stages for custom actions starts-----
    virtual void Init(){}
    virtual void Deinit(){}
    //stages for custom actions ends-----

    //API section starts-----
    void send( enComponentType src, uint_fast32_t src_id, enComponentType dest, uint_fast32_t dest_id, std::vector<uint_fast32_t> msg ){}
    //API section ends-----
};

template< class ComponentClass >
bool enEngineKernelAbstract::RegisterComponent( ComponentClass * component, uint_fast32_t id  ){
    if( !component )
	return false;
    component->SaveKernelInfo( this, _cid );
    _components[ std::pair<enComponentType,uint_fast32_t>( component->_component_type, _cid) ] = (void*) component;
    return true;
}

template< class ComponentClass >
ComponentClass * enEngineKernelAbstract::GetComponent( enComponentType type, uint_fast32_t id ){
  auto it_find = _components.find( std::pair<enComponentType, uint_fast32_t>( type, id) );
    if( _components.end() == it_find )
	return nullptr;
    return (ComponentClass *) it_find->second;
}

bool enEngineKernelAbstract::RemoveComponentAll(){
    _components.clear();
    return true;
}

enEngineKernelAbstract * enEngineKernelAbstract::GetCoreEngine(){
    return this;
}

#endif
