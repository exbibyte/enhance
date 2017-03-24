#ifndef RENDERCOMPUTE0_H
#define RENDERCOMPUTE0_H

#include "IRendercompute.hpp"
#include "RenderData.hpp"

#include "Vec.hpp"

#include <vector>
#include <list>

class Rendercompute0 : public IRendercompute {
public:
    char const * get_id(){ return "rendercompute0"; }
    RenderData compute( light l, camera c, std::list< IRendercompute::RenderDataPack > render_data );
};

#endif
