#ifndef EN_COMPONENT_RENDERDRAW_H
#define EN_COMPONENT_RENDERDRAW_H

#include "enComponentSpecialize.hpp"
#include "IRenderdraw.hpp"
#include "Renderdraw0.hpp"
#include "Renderdraw1.hpp"

class enComponentRenderdraw0 : public enComponentSpecialize<IRenderdraw, Renderdraw0 > {
public:
    enComponentRenderdraw0( Renderdraw0 * instance ) : enComponentSpecialize<IRenderdraw, Renderdraw0 >( instance, enComponentType::RENDERDRAW ){}
};

class enComponentRenderdraw1 : public enComponentSpecialize<IRenderdraw, Renderdraw1 > {
public:
    enComponentRenderdraw1( Renderdraw1 * instance ) : enComponentSpecialize<IRenderdraw, Renderdraw1 >( instance, enComponentType::RENDERDRAW ){}
};

#endif
