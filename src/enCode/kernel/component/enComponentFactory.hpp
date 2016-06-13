#ifndef EN_COMPONENT_FACTORY_H
#define EN_COMPONENT_FACTORY_H

#include "enComponentMeta.hpp"

class enComponentFactory : public enComponentMeta {
public:  
    enComponentFactory : enComponentMeta( enComponentType::FACTORY ){}
    template< class Abstract, class Concrete >
    enComponentMeta * gen_component( Concrete * instance ){
	return new Abstract( instance );
    }
};

#endif
