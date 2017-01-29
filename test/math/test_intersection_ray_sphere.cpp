#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include "catch.hpp"
#include "intersection.hpp"
#include "Vec.hpp"
#include "ray.hpp"
#include "sphere.hpp"

#include <iostream>
using namespace std;

TEST_CASE( "ray_sphere hit: outside of sphere, opposite direction", "[ray_sphere]" ) {
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
    bool hit_inside;
    bool ret = intersection::ray_sphere( r, s, t, hit_inside );
    CHECK( ret );
    CHECK( false == hit_inside );
    CHECK(t >= 15-error);
    CHECK(t <= 15+error);
}

TEST_CASE( "ray_sphere no hit: outside of sphere, oblique direction", "[ray_sphere]" ) {
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
    bool hit_inside;
    bool ret = intersection::ray_sphere( r, s, t, hit_inside );
    CHECK( false == ret );
}

TEST_CASE( "ray_sphere no hit: outside of sphere, same direction", "[ray_sphere]" ) {
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
    bool hit_inside;
    bool ret = intersection::ray_sphere( r, s, t, hit_inside );
    CHECK( false == ret );
}

TEST_CASE( "ray_sphere hits: inside of sphere", "[ray_sphere]" ) {
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
    bool hit_inside;
    bool ret = intersection::ray_sphere( r, s, t, hit_inside );
    CHECK( ret );
    CHECK( hit_inside );
    CHECK(t <= 2+error);
    CHECK(t >= 2-error);
}
