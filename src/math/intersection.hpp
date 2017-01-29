#ifndef INTERSECTION_HPP
#define INTERSECTION_HPP

#include "ray.hpp"
#include "sphere.hpp"
#include "plane.hpp"

class intersection {
public:
    static bool ray_sphere( ray const & r, sphere const & s, float & t, bool & hit_inside ); //returns t distance in direction of ray r
    static bool ray_plane( ray const & r, plane const & p, float & t ); //returns t distance in direction of ray r
};

#endif
