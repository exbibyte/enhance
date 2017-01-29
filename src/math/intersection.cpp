#include "intersection.hpp"
#include "ray.hpp"
#include "sphere.hpp"
#include "Vec.hpp"

#include <cmath>
#include <iostream>
using namespace std;

bool intersection::ray_sphere( ray const & r, sphere const & s, float & t ){
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
