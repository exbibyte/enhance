#ifndef INTERSECTION_HPP
#define INTERSECTION_HPP

#include "ray.hpp"
#include "sphere.hpp"
#include "plane.hpp"

class intersection {
public:
    static bool ray_sphere( ray const & r, sphere const & s, float & t, bool & hit_inside ); //returns t distance in direction of ray r
    static bool ray_plane( ray const & r, plane const & p, float & t ); //returns t distance in direction of ray r
    static Vec coord_from_ray( ray const & r, float & t ); //returns offset coordinates in direction of ray
    static bool intersect_info_ray_sphere( ray const & r, sphere const & s, Vec & intersect_pos, Vec & sphere_normal );
    static bool intersect_info_ray_plane( ray const & r, plane const & p, Vec & intersect_pos, Vec & plane_normal );
    static bool intersect_rays( ray const & a, ray const & b, float & t );
};

#endif
