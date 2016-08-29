#ifndef RENDERCOMPUTE0_H
#define RENDERCOMPUTE0_H

#include "IRendercompute.hpp"
#include "RenderData.hpp"

#include "Vec.h"

#include <vector>

class Rendercompute0 : public IRendercompute {
public:
    char const * get_id(){ return "rendercompute0"; }
    RenderData compute( std::vector<double> vert_coord, std::vector<double> vert_normal, Vec orient_axis, double orient_angle );
    double _rotation_angle = 0;
};

#endif
