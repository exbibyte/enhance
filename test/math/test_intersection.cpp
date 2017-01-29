#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include "catch.hpp"
#include "intersection.hpp"
#include "Vec.hpp"
#include "ray.hpp"
#include "sphere.hpp"

#include <iostream>
using namespace std;

TEST_CASE( "ray_sphere hit", "[ray_sphere]" ) {
    ray r;
    r._offset[0] = 20.0;
    r._offset[1] = 0;
    r._offset[2] = 0;
    r._offset[3] = 0;
    r._dir[0] = -1;
    r._dir[1] = 0;
    r._dir[2] = 0;
    r._dir[3] = 0;
    sphere s;
    s._radius = 5.0;
    s._offset[0] = 0;
    s._offset[1] = 0;
    s._offset[2] = 0;
    s._offset[3] = 0;

    float error = 0.0001;
    float t;
    bool ret = intersection::ray_sphere( r, s, t );
    CHECK( ret );
    CHECK(t >= 15-error);
    CHECK(t <= 15+error);
}

TEST_CASE( "ray_sphere no hit", "[ray_sphere]" ) {
    ray r;
    r._offset[0] = 20.0;
    r._offset[1] = 0;
    r._offset[2] = 0;
    r._offset[3] = 0;
    r._dir[0] = 0;
    r._dir[1] = 1;
    r._dir[2] = 0;
    r._dir[3] = 0;
    sphere s;
    s._radius = 5.0;
    s._offset[0] = 0;
    s._offset[1] = 0;
    s._offset[2] = 0;
    s._offset[3] = 0;

    float error = 0.0001;
    float t;
    bool ret = intersection::ray_sphere( r, s, t );
    CHECK( false == ret );
}

TEST_CASE( "ray_sphere no hit opposite direction", "[ray_sphere]" ) {
    ray r;
    r._offset[0] = 5.5;
    r._offset[1] = 0;
    r._offset[2] = 0;
    r._offset[3] = 0;
    r._dir[0] = 1;
    r._dir[1] = 0;
    r._dir[2] = 0;
    r._dir[3] = 0;
    sphere s;
    s._radius = 5.0;
    s._offset[0] = 0;
    s._offset[1] = 0;
    s._offset[2] = 0;
    s._offset[3] = 0;

    float error = 0.0001;
    float t;
    bool ret = intersection::ray_sphere( r, s, t );
    CHECK( false == ret );
}

TEST_CASE( "ray_sphere hits from inside", "[ray_sphere]" ) {
    ray r;
    r._offset[0] = 0;
    r._offset[1] = 0;
    r._offset[2] = 3;
    r._offset[3] = 0;
    r._dir[0] = 0;
    r._dir[1] = 0;
    r._dir[2] = 1;
    r._dir[3] = 0;
    sphere s;
    s._radius = 5.0;
    s._offset[0] = 0;
    s._offset[1] = 0;
    s._offset[2] = 0;
    s._offset[3] = 0;

    float error = 0.0001;
    float t;
    bool ret = intersection::ray_sphere( r, s, t );
    CHECK( ret );
    CHECK(t <= 2+error);
    CHECK(t >= 2-error);
}
