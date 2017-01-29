#ifndef INTERSECTION_HPP
#define INTERSECTION_HPP

#include "ray.hpp"
#include "sphere.hpp"

class intersection {
public:
    static bool ray_sphere( ray const & r, sphere const & s, float & t ); //returns t distance in direction of ray r
};

#endif
