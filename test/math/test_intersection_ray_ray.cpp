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

TEST_CASE( "ray ray no intersection: parallel offset", "[ray_ray]" ) {
    ray a;
    a._offset[0] = 20.0;
    a._offset[1] = 0;
    a._offset[2] = 0;
    a._offset[3] = 0;
    a._dir[0] = 1;
    a._dir[1] = 1;
    a._dir[2] = 1;
    a._dir[3] = 0;
    ray b;
    b._offset[0] = 25.0;
    b._offset[1] = 0;
    b._offset[2] = 0;
    b._offset[3] = 0;
    b._dir[0] = 1;
    b._dir[1] = 1;
    b._dir[2] = 1;
    b._dir[3] = 0;
    float t;

    bool ret = intersection::intersect_rays( a, b, t );
    CHECK( false == ret );
}

TEST_CASE( "ray ray intersection", "[ray_ray]" ) {
    ray a;
    a._offset[0] = 5;
    a._offset[1] = 5;
    a._offset[2] = 0;
    a._offset[3] = 0;
    a._dir[0] = -1;
    a._dir[1] = 0;
    a._dir[2] = 0;
    a._dir[3] = 0;
    ray b;
    b._offset[0] = 0;
    b._offset[1] = 0;
    b._offset[2] = 0;
    b._offset[3] = 0;
    b._dir[0] = 0;
    b._dir[1] = 1;
    b._dir[2] = 0;
    b._dir[3] = 0;
    float t;

    bool ret = intersection::intersect_rays( a, b, t );
    CHECK( ret );
    float error = 0.0001;
    CHECK( t >= 5 - error );
    CHECK( t <= 5 + error );
}

TEST_CASE( "ray ray intersection colinear at b", "[ray_ray]" ) {
    ray a;
    a._offset[0] = 5;
    a._offset[1] = 5;
    a._offset[2] = 0;
    a._offset[3] = 0;
    a._dir[0] = 1;
    a._dir[1] = 1;
    a._dir[2] = 0;
    a._dir[3] = 0;
    ray b;
    b._offset[0] = 15;
    b._offset[1] = 15;
    b._offset[2] = 0;
    b._offset[3] = 0;
    b._dir[0] = 1;
    b._dir[1] = 1;
    b._dir[2] = 0;
    b._dir[3] = 0;
    float t;

    bool ret = intersection::intersect_rays( a, b, t );
    CHECK( ret );
    float error = 0.0001;
    CHECK( t >= 10 - error );
    CHECK( t <= 10 + error );
}

TEST_CASE( "ray ray intersection colinear at a", "[ray_ray]" ) {
    ray a;
    a._offset[0] = 5;
    a._offset[1] = 5;
    a._offset[2] = 0;
    a._offset[3] = 0;
    a._dir[0] = 1;
    a._dir[1] = 1;
    a._dir[2] = 0;
    a._dir[3] = 0;
    ray b;
    b._offset[0] = 15;
    b._offset[1] = 15;
    b._offset[2] = 0;
    b._offset[3] = 0;
    b._dir[0] = 1;
    b._dir[1] = 1;
    b._dir[2] = 0;
    b._dir[3] = 0;
    float t;

    bool ret = intersection::intersect_rays( b, a, t );
    CHECK( ret );
    float error = 0.0001;
    CHECK( t >= 0 - error );
    CHECK( t <= 0 + error );
}

TEST_CASE( "ray ray no intersection: non-coplanar", "[ray_ray]" ) {
    ray a;
    a._offset[0] = 5;
    a._offset[1] = 5;
    a._offset[2] = 2;
    a._offset[3] = 0;
    a._dir[0] = -1;
    a._dir[1] = -1;
    a._dir[2] = 0;
    a._dir[3] = 0;
    ray b;
    b._offset[0] = 5;
    b._offset[1] = 5;
    b._offset[2] = 0;
    b._offset[3] = 0;
    b._dir[0] = 1;
    b._dir[1] = 1;
    b._dir[2] = 0;
    b._dir[3] = 0;
    float t;

    bool ret = intersection::intersect_rays( a, b, t );
    CHECK( false == ret );
}
