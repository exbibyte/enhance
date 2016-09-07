#ifndef IRENDERCOMPUTE_H
#define IRENDERCOMPUTE_H

#include "RenderData.hpp"

#include "Vec.h"

#include <vector>
#include <list>

class IRendercompute {
public:
    class RenderDataPack {
    public:
	std::vector<double> vert_coord;
	std::vector<double> vert_normal;
	Vec orient_axis;
	double orient_angle;
    };
    virtual ~IRendercompute(){}
    virtual RenderData compute( std::list< IRendercompute::RenderDataPack > render_data ) = 0;
};

#endif
