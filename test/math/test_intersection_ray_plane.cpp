#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include "catch.hpp"
#include "intersection.hpp"
#include "Vec.hpp"
#include "ray.hpp"
#include "plane.hpp"

#include <iostream>
using namespace std;

TEST_CASE( "ray_plane hit: in front of plane, opposite direction", "[ray_plane]" ) {
    ray r;
    r._offset[0] = 0;
    r._offset[1] = 0;
    r._offset[2] = 10;
    r._offset[3] = 0;
    r._dir[0] = 0;
    r._dir[1] = 0;
    r._dir[2] = -1;
    r._dir[3] = 0;
    plane p;
    p._offset[0] = 0;
    p._offset[1] = 0;
    p._offset[2] = 1;
    p._offset[3] = 0;
    p._normal[0] = 0;
    p._normal[1] = 0;
    p._normal[2] = 1;
    p._normal[3] = 0;

    float error = 0.0001;
    float t;
    bool ret = intersection::ray_plane( r, p, t );
    CHECK( ret );
    CHECK(t <= 9+error);
    CHECK(t >= 9-error);
}

TEST_CASE( "ray_plane no hit: in front of plane, same direction", "[ray_plane]" ) {
    ray r;
    r._offset[0] = 0;
    r._offset[1] = 0;
    r._offset[2] = 10;
    r._offset[3] = 0;
    r._dir[0] = 0;
    r._dir[1] = 0;
    r._dir[2] = 1;
    r._dir[3] = 0;
    plane p;
    p._offset[0] = 0;
    p._offset[1] = 0;
    p._offset[2] = 1;
    p._offset[3] = 0;
    p._normal[0] = 0;
    p._normal[1] = 0;
    p._normal[2] = 1;
    p._normal[3] = 0;

    float error = 0.0001;
    float t;
    bool ret = intersection::ray_plane( r, p, t );
    CHECK( false == ret );
}

TEST_CASE( "ray_plane no hit: behind plane, opposite direction", "[ray_plane]" ) {
    ray r;
    r._offset[0] = 0;
    r._offset[1] = 0;
    r._offset[2] = 0;
    r._offset[3] = 0;
    r._dir[0] = 0;
    r._dir[1] = 0;
    r._dir[2] = -1;
    r._dir[3] = 0;
    plane p;
    p._offset[0] = 0;
    p._offset[1] = 0;
    p._offset[2] = 1;
    p._offset[3] = 0;
    p._normal[0] = 0;
    p._normal[1] = 0;
    p._normal[2] = 1;
    p._normal[3] = 0;

    float error = 0.0001;
    float t;
    bool ret = intersection::ray_plane( r, p, t );
    CHECK( false == ret );
}

TEST_CASE( "ray_plane no hit: behind plane, same direction", "[ray_plane]" ) {
    ray r;
    r._offset[0] = 0;
    r._offset[1] = 0;
    r._offset[2] = 0;
    r._offset[3] = 0;
    r._dir[0] = 0;
    r._dir[1] = 0;
    r._dir[2] = 1;
    r._dir[3] = 0;
    plane p;
    p._offset[0] = 0;
    p._offset[1] = 0;
    p._offset[2] = 1;
    p._offset[3] = 0;
    p._normal[0] = 0;
    p._normal[1] = 0;
    p._normal[2] = 1;
    p._normal[3] = 0;

    float error = 0.0001;
    float t;
    bool ret = intersection::ray_plane( r, p, t );
    CHECK( false == ret );
}

TEST_CASE( "ray_plane no hit: in front of plane, tangent direction", "[ray_plane]" ) {
    ray r;
    r._offset[0] = 0;
    r._offset[1] = 0;
    r._offset[2] = 10;
    r._offset[3] = 0;
    r._dir[0] = 1;
    r._dir[1] = 0;
    r._dir[2] = 0;
    r._dir[3] = 0;
    plane p;
    p._offset[0] = 0;
    p._offset[1] = 0;
    p._offset[2] = 1;
    p._offset[3] = 0;
    p._normal[0] = 0;
    p._normal[1] = 0;
    p._normal[2] = 1;
    p._normal[3] = 0;

    float error = 0.0001;
    float t;
    bool ret = intersection::ray_plane( r, p, t );
    CHECK( false == ret );
}

TEST_CASE( "ray_plane no hit: coplanar, tangent direction", "[ray_plane]" ) {
    ray r;
    r._offset[0] = 0;
    r._offset[1] = 0;
    r._offset[2] = 1;
    r._offset[3] = 0;
    r._dir[0] = 1;
    r._dir[1] = 0;
    r._dir[2] = 0;
    r._dir[3] = 0;
    plane p;
    p._offset[0] = 0;
    p._offset[1] = 0;
    p._offset[2] = 1;
    p._offset[3] = 0;
    p._normal[0] = 0;
    p._normal[1] = 0;
    p._normal[2] = 1;
    p._normal[3] = 0;

    float error = 0.0001;
    float t;
    bool ret = intersection::ray_plane( r, p, t );
    CHECK( false == ret );
}

TEST_CASE( "ray_plane hit: coplanar, oblique opposite direction", "[ray_plane]" ) {
    ray r;
    r._offset[0] = 0;
    r._offset[1] = 0;
    r._offset[2] = 1;
    r._offset[3] = 0;
    r._dir[0] = 0;
    r._dir[1] = -1;
    r._dir[2] = -1;
    r._dir[3] = 0;
    plane p;
    p._offset[0] = 0;
    p._offset[1] = 0;
    p._offset[2] = 1;
    p._offset[3] = 0;
    p._normal[0] = 0;
    p._normal[1] = 0;
    p._normal[2] = 1;
    p._normal[3] = 0;

    float error = 0.0001;
    float t;
    bool ret = intersection::ray_plane( r, p, t );
    CHECK( true == ret );
    CHECK( t >= 0-error );
    CHECK( t <= 0+error );
}
