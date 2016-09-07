#ifndef RENDERCOMPUTE0_H
#define RENDERCOMPUTE0_H

#include "IRendercompute.hpp"
#include "RenderData.hpp"

#include "Vec.h"

#include <vector>
#include <list>

class Rendercompute0 : public IRendercompute {
public:
    char const * get_id(){ return "rendercompute0"; }
    RenderData compute( std::list< IRendercompute::RenderDataPack > render_data );
};

#endif
