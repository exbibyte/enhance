#ifndef EN_COMPFACT_CLOCK0_H
#define EN_COMPFACT_CLOCK0_H

#include "enComponentMeta.h"

class enCompFact_Clock0 : public enComponentMeta {
public:  
    enCompFact_Clock0 : enComponentMeta( enComponentType::FACTORY_CLOCK ){
	
    }
    template<class Concrete >
    gen_clock( Concrete * clock ){
	
    }
};

#endif
