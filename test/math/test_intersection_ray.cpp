#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include "catch.hpp"
#include "intersection.hpp"
#include "Vec.hpp"
#include "ray.hpp"
#include "sphere.hpp"
#include "plane.hpp"

#include <cmath>
#include <iostream>
using namespace std;

TEST_CASE( "ray coord offset", "[ray_coord]" ) {
    ray r;
    r._offset[0] = 20.0;
    r._offset[1] = 0;
    r._offset[2] = 0;
    r._offset[3] = 0;
    r._dir[0] = 1;
    r._dir[1] = 1;
    r._dir[2] = 1;
    r._dir[3] = 0;
    float t = 10;

    float error = 0.0001;
    bool hit_inside;
    Vec coord = intersection::coord_from_ray( r, t);
    float axis_delta_per_unit_vector = 1.0/sqrt(3.0);
    float axis_delta = axis_delta_per_unit_vector * t;
    CHECK( coord._dim == 4 );
    CHECK( coord[0] >= axis_delta + 20 - error );
    CHECK( coord[0] <= axis_delta + 20 + error );
    CHECK( coord[1] >= axis_delta + 0 - error );
    CHECK( coord[1] <= axis_delta + 0 + error );
    CHECK( coord[2] >= axis_delta + 0 - error );
    CHECK( coord[2] <= axis_delta + 0 + error );
}

TEST_CASE( "intersect info ray sphere", "[intersec_info]" ) {
    ray r;
    r._offset[0] = 7.0;
    r._offset[1] = 7.0;
    r._offset[2] = 5.0;
    r._offset[3] = 0;
    r._dir[0] = -1;
    r._dir[1] = -1;
    r._dir[2] = 0;
    r._dir[3] = 0;
    sphere s;
    s._offset[0] = 5.0;
    s._offset[1] = 5.0;
    s._offset[2] = 5.0;
    s._offset[3] = 0;
    s._radius = 1;

    float error = 0.0001;
    Vec intersect_pos;
    Vec sphere_normal;
    bool ret = intersection::intersect_info_ray_sphere( r, s, intersect_pos, sphere_normal );
    float axis_delta = 1/sqrt(2);
    CHECK( ret );
    CHECK( intersect_pos[0] >= 5 + axis_delta - error );
    CHECK( intersect_pos[0] <= 5 + axis_delta + error );
    CHECK( intersect_pos[1] >= 5 + axis_delta - error );
    CHECK( intersect_pos[1] <= 5 + axis_delta + error );
    CHECK( intersect_pos[2] >= 5 - error );
    CHECK( intersect_pos[2] <= 5 + error );

    CHECK( sphere_normal[0] >= axis_delta - error );
    CHECK( sphere_normal[0] <= axis_delta + error );
    CHECK( sphere_normal[1] >= axis_delta - error );
    CHECK( sphere_normal[1] <= axis_delta + error );
    CHECK( sphere_normal[2] >= 0 - error );
    CHECK( sphere_normal[2] <= 0 + error );
}

TEST_CASE( "intersect info ray plane", "[intersec_info]" ) {
    ray r;
    r._offset[0] = 5.0;
    r._offset[1] = 6.0;
    r._offset[2] = 0.0;
    r._offset[3] = 0;
    r._dir[0] = -1;
    r._dir[1] = -1;
    r._dir[2] = 0;
    r._dir[3] = 0;
    plane p;
    p._offset[0] = 0.0;
    p._offset[1] = 1.0;
    p._offset[2] = 0.0;
    p._offset[3] = 0;
    p._normal[0] = 1.0;
    p._normal[1] = 0;
    p._normal[2] = 0;
    p._normal[3] = 0;
    
    float error = 0.0001;
    Vec intersect_pos;
    Vec plane_normal;
    bool ret = intersection::intersect_info_ray_plane( r, p, intersect_pos, plane_normal );
    CHECK( ret );
    CHECK( intersect_pos[0] >= 0 - error );
    CHECK( intersect_pos[0] <= 0 + error );
    CHECK( intersect_pos[1] >= 1 - error );
    CHECK( intersect_pos[1] <= 1 + error );
    CHECK( intersect_pos[2] >= 0 - error );
    CHECK( intersect_pos[2] <= 0 + error );

    CHECK( plane_normal[0] >= 1 - error );
    CHECK( plane_normal[0] <= 1 + error );
    CHECK( plane_normal[1] >= 0 - error );
    CHECK( plane_normal[1] <= 0 + error );
    CHECK( plane_normal[2] >= 0 - error );
    CHECK( plane_normal[2] <= 0 + error );
}
