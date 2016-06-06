#ifndef EN_ENGINE_KERNEL_ABSTRACT_H
#define EN_ENGINE_KERNEL_ABSTRACT_H

#include "enComponentType.h"

#include <map>
#include <functional>
#include <cstdint>
#include <vector>

//structure of a transmitted message between components
struct enPacketTransmit {
    enComponentType _src;
    enComponentType _dest;
    uint_fast32_t _src_cid;
    uint_fast32_t _dest_cid;
    std::vector<int_fast32_t> _data;
};

//interface for core engine
class enEngineKernelAbstract {
public:
    enEngineKernelAbstract(){}
    ~enEngineKernelAbstract(){}
    template< class ComponentClass >
    bool RegisterComponent( ComponentClass * component, uint_fast32_t cid = 0 );
    template< class ComponentClass >
    ComponentClass * GetComponent( enComponentType type, uint_fast32_t cid = 0 );
    bool RemoveComponentAll();
    enEngineKernelAbstract * GetCoreEngine();
    std::map< std::pair<enComponentType,uint_fast32_t>, void * > _components;
    uint_fast32_t _cid = 0; //component id

    //stages for custom actions starts-----
    virtual void Init(){}
    virtual void Deinit(){}
    //stages for custom actions ends-----

    //API section starts-----
    void transmit( enPacketTransmit packet ){}
    //API section ends-----
};

template< class ComponentClass >
bool enEngineKernelAbstract::RegisterComponent( ComponentClass * component, uint_fast32_t cid ){
    if( !component )
	return false;
    component->SaveKernelInfo( this, _cid );
    _components[ std::pair<enComponentType,uint_fast32_t>( component->get_component_type(), _cid) ] = (void*) component;
    return true;
}

template< class ComponentClass >
ComponentClass * enEngineKernelAbstract::GetComponent( enComponentType type, uint_fast32_t cid ){
  auto it_find = _components.find( std::pair<enComponentType, uint_fast32_t>( type, cid) );
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
