#ifndef EN_ENGINE_CORE_ABSTRACT_H
#define EN_ENGINE_CORE_ABSTRACT_H

#include "enComponentType.h"

#include <map>
#include <functional>

//interface for core engine
class enEngineCoreAbstract {
public:
    enEngineCoreAbstract(){}
    ~enEngineCoreAbstract(){}
    template< class ComponentClass >
    bool RegisterComponent( enComponentType type, ComponentClass * component );
    template< class ComponentClass >
    ComponentClass * GetComponent( enComponentType type );
    bool RemoveComponentAll();
    enEngineCoreAbstract * GetCoreEngine();
    std::map< enComponentType, void * > _components;
};

template< class ComponentClass >
bool enEngineCoreAbstract::RegisterComponent( enComponentType type, ComponentClass * component  ){
    if( !component )
	return false;
    _components[ type ] = (void*) component;
    return true;
}

template< class ComponentClass >
ComponentClass * enEngineCoreAbstract::GetComponent( enComponentType type ){
    auto it_find = _components.find( type );
    if( _components.end() == it_find )
	return nullptr;
    return (ComponentClass *) it_find->second;
}

bool enEngineCoreAbstract::RemoveComponentAll(){
    _components.clear();
    return true;
}

enEngineCoreAbstract * enEngineCoreAbstract::GetCoreEngine(){
    return this;
}

#endif
