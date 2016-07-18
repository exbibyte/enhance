#ifndef EN_COMPONENT_RENDERDRAW_H
#define EN_COMPONENT_RENDERDRAW_H

#include "enComponentSpecialize.hpp"
#include "IRenderdraw.hpp"
#include "Renderdraw0.hpp"

class enComponentRenderdraw0 : public enComponentSpecialize<IRenderdraw, Renderdraw0 > {
public:
    enComponentRenderdraw0( Renderdraw0 * instance ) : enComponentSpecialize<IRenderdraw, Renderdraw0 >( instance, enComponentType::RENDERDRAW ){}
};

#endif
