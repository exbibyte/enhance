#include "intersection.hpp"
#include "ray.hpp"
#include "sphere.hpp"
#include "Vec.hpp"

#include <cmath>
#include <iostream>
#include <cassert>

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
    plane p2 = p;
    p2._normal.NormalizeCurrent();
    float numerator = p2._offset.Dot(p2._normal) - p2._normal.Dot( r._offset );
    float denominator = p2._normal.Dot( r._dir.Normalize() );
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

Vec intersection::coord_from_ray( ray const & r, float & t ){
    ray r2 = r;
    r2._dir.NormalizeCurrent();
    Vec v = Vec::ScaleVecAdd( t, r2._dir, r2._offset );
    return v;
}

bool intersection::intersect_info_ray_sphere( ray const & r, sphere const & s, Vec & intersect_pos, Vec & sphere_normal ){
    float t;
    bool hit_inside;
    bool ret = ray_sphere( r, s, t, hit_inside );
    if( !ret ){
	return false;
    }
    intersect_pos = coord_from_ray( r, t );
    sphere_normal = intersect_pos - s._offset;
    sphere_normal.NormalizeCurrent();
    return true;
}

bool intersection::intersect_info_ray_plane( ray const & r, plane const & p, Vec & intersect_pos, Vec & plane_normal ){
    float t;
    bool ret = ray_plane( r, p, t );
    if( !ret ){
	return false;
    }
    intersect_pos = coord_from_ray( r, t );
    plane_normal = p._normal;
    return true;
}

bool intersection::intersect_rays( ray const & a, ray const & b, float & t ){
    if( a._dir.GetDim() != b._dir.GetDim() ){
	return false;
    }
    if( a._offset.GetDim() != a._dir.GetDim() ){
	return false;
    }
    if( b._offset.GetDim() != b._dir.GetDim() ){
	return false;
    }
    if( a._dir.GetDim() == 0 ){
	return false;
    }
    Vec a1, b1, a1_offset, b1_offset;
    if( a._dir.GetDim() > 3 ){
	a1 = a._dir.GetSubVector( 0, 3 );
	b1 = b._dir.GetSubVector( 0, 3 );
	a1_offset = a._offset.GetSubVector( 0, 3 );
	b1_offset = b._offset.GetSubVector( 0, 3 );
    }else{
	a1 = a._dir;
	b1 = b._dir;
	a1_offset = a._offset;
	b1_offset = b._offset;
    }
    Vec c = b1 - a1;
    Vec v = a1.Cross( b1 );
    float error = 0.0001;
    float dot_v_c = v.Dot( c );
    if( ( dot_v_c < 0 - error ) && ( dot_v_c > 0 + error ) ){
	//no intersection because lines are not in a same plane
	return false;
    }
    //test for colinearity
    Vec zero( c.GetDim() );
    Vec d = b1_offset - a1_offset;
    if( v.IsEqual( zero, 0.0001 ) ){
	//check triangle area forms by points on ray a and b
	Vec point1 = a1_offset + a1;
	Vec triangle = point1.Cross( b1_offset );
	float triangle_area = triangle.Magnitude();
	if( triangle_area < 0 - error || triangle_area > 0 + error ){
	    //lines are parallel but no overlap
	    return false;
	}
	//lines are colinear
	float direction = (b1_offset-a1_offset).Dot( a1 ) < 0 ? -1 : 1;
	float distance = direction * (b1_offset-a1_offset).Magnitude()/a1.Magnitude();
	if( distance < 0 ){
	    // intersection at offset of ray a, so clamp paramter t to 0
	    t = 0;
	}else{
	    //intersection at offset of ray b
	    t = distance;
	}
	return true;
    }
    //there is a solvable intersection
    Vec numerator = d.Cross( b1 );
    float t2 = numerator.Magnitude()/v.Magnitude();
    if( t2 < 0 ){
	//not possible with ray translation
	return false;
    }

    t = t2;
    return true;
}
