#ifndef IRENDERCOMPUTE_H
#define IRENDERCOMPUTE_H

#include <vector>

#include "RenderData.hpp"

class IRendercompute {
public:
    virtual ~IRendercompute(){}
    virtual RenderData compute( std::vector<double> vert_coord, std::vector<double> vert_normal ) = 0;
};

#endif
