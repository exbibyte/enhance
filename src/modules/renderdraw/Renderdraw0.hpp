#ifndef RENDERDRAW0_H
#define RENDERDRAW0_H

#include "IRenderdraw.hpp"

class Renderdraw0 : public IRenderdraw {
public:
    char const * get_id(){ return "renderdraw0"; }
    bool render( RenderData );
};

#endif
