#ifndef RENDERCOMPUTE0_H
#define RENDERCOMPUTE0_H

#include <vector>

#include "IRendercompute.hpp"
#include "RenderData.hpp"

class Rendercompute0 : public IRendercompute {
public:
    char const * get_id(){ return "rendercompute0"; }
    RenderData compute( std::vector<double> vert_coord, std::vector<double> vert_normal );
    double _rotation_angle = 0;
};

#endif
