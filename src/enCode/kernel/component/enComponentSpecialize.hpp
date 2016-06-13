#ifndef EN_COMPONENT_SPECIALIZE_H
#define EN_COMPONENT_SPECIALIZE_H

#include <chrono>

#include "enComponentMeta.hpp"
#include "enComponentType.hpp"

template< class AbstractType, class ConcreteType >
class enComponentSpecialize: public enComponentMeta {
public:
    enComponentSpecialize( ConcreteType * instance, enComponentType type ) : enComponentMeta( type ){
	_instance = instance;
    }
    ~enComponentSpecialize(){
	if( _instance != nullptr ){
	    delete _instance;
	}
    }
private:
    AbstractType * _instance;
};

#endif
