#ifndef EN_COMPONENT_SPECIALIZE_H
#define EN_COMPONENT_SPECIALIZE_H

#include <chrono>

#include "enComponentMeta.hpp"
#include "enComponentType.hpp"

// #define COMPONENT_INSTANCE( iden, wrapper_component, meta_component ) \
//     wrapper_component * wrapper_##iden = dynamic_cast< wrapper_component * >( meta_component ); \
//     wrapper_component::InterfaceType * iden = nullptr; \
//     if( nullptr == wrapper_##iden ){ \
// 	assert( false && "invalid component cast" );	\
//     }else{ \
// 	iden = wrapper_##iden->get_instance();	\
//     }

#define COMPONENT_INSTANCE( iden, wrapper_component, meta_component ) \
    wrapper_component * wrapper_##iden = dynamic_cast< wrapper_component * >( meta_component ); \
    wrapper_component::InterfaceType * iden = nullptr; \
    if( nullptr == wrapper_##iden ){ \
	assert(true); \
    }else{ \
	iden = wrapper_##iden->get_instance();	\
    }

template< class AbstractType, class ConcreteType >
class enComponentSpecialize: public enComponentMeta {
public:
    using InterfaceType = AbstractType;
    enComponentSpecialize( ConcreteType * instance, enComponentType type ) : enComponentMeta( type, instance->get_id() ){
	_instance = instance;
    }
    ~enComponentSpecialize(){
	if( _instance != nullptr ){
	    delete _instance;
	}
    }
    AbstractType * get_instance() const {
	return _instance;
    };
private:
    AbstractType * _instance;
};

#endif
