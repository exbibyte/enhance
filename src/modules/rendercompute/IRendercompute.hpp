#ifndef IRENDERCOMPUTE_H
#define IRENDERCOMPUTE_H

#include "RenderData.hpp"

#include "Vec.h"

#include <vector>

class IRendercompute {
public:
    virtual ~IRendercompute(){}
    virtual RenderData compute( std::vector<double> vert_coord, std::vector<double> vert_normal, Vec orient_axis, double orient_angle ) = 0;
};

#endif
