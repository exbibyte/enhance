#include "intersection.hpp"
#include "ray.hpp"
#include "sphere.hpp"
#include "Vec.hpp"

#include <cmath>
#include <iostream>
using namespace std;

bool intersection::ray_sphere( ray const & r, sphere const & s, float & t, bool & hit_inside ){
    //sphere surface constraint: dot(s.offset,s.offset) - s.radius^2 = 0
    //ray: p(t) = r.offset + t*r.dir
    //normalize r
    //substitute ray into sphere to solve the simplified quadratic equation:
    // -b +/- sqrt( dot(b,b) - c )

    ray r_normalized = r;
    r_normalized._dir.NormalizeCurrent();

    Vec relative_offset = r_normalized._offset - s._offset;
    
    float b = relative_offset.Dot( r_normalized._dir );
    float c = relative_offset.Dot( relative_offset ) - s._radius * s._radius;
    
    if( ( b > 0.0 ) && ( c > 0 ) ){
	//ray points away from sphere and is outside of the sphere
	return false;
    }
    
    float d = b*b - c; //discriminant
    if( d < 0.0 ){
	//ray misses sphere
	return false;
    }

    if( c <= 0 ){
	hit_inside = true;
    }else{
	hit_inside = false;
    }
    
    float t1 = -b - sqrt(d);
    float t2 = -b + sqrt(d);

    if( t1 < 0 ){
	t = t2;
	return true;
    }
    else if( t2 < 0 ){
	t = t1;
	return true;
    }
    
    t = t1 < t2 ? t1 : t2;

    return true;
}

bool intersection::ray_plane( ray const & r, plane const & p, float & t ){
    //ray equation: r(t) = r.offset + r.dir*t
    //plane equation: p(x) = dot(p.normal,x-p.offset) = 0
    //                p(x) = -dot(p.offset,p.normal) + dot(p.normal,x) = 0
    //substitute ray into plane quation:
    // p(t) = -dot(p.offset,p.normal) + dot(p.normal, r.offset + r.dir*t) = 0
    //      = -dot(p.offset,p.normal) + dot(p.normal, r.offset) + t*dot(p.normal, r.dir) = 0
    // t = ( dot(p.offset,p.normal) - dot(p.normal, r.offset) )/( dot(p.normal, r.dir) )
    float numerator = p._offset.Dot(p._normal) - p._normal.Dot( r._offset );
    float denominator = p._normal.Dot( r._dir );
    if( denominator == 0 ){
	//ray direction is coplannar to the plane
	return false;
    }
    if( denominator > 0 ){
	//ray direction is not facing plane normal
	return false;
    }
    
    t = numerator / denominator;
    if( t < 0 ){
	return false;
    }
    return true;
}
