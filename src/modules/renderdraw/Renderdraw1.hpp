#ifndef RENDERDRAW1_H
#define RENDERDRAW1_H

#include "IRenderdraw.hpp"
#include "RenderData.hpp"

class Renderdraw1 : public IRenderdraw {
public:
    char const * get_id(){ return "renderdraw1"; }
    bool render( RenderData );
};

#endif
