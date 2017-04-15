#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include "catch.hpp"

#include <cmath>
#include <iostream>
#include <limits>

#include "vectorspace.hpp"
#include "Vec.hpp"

using namespace std;

TEST_CASE( "standard p-norm", "[vector space]" ) {
    Vec v;
    double arr[] = { -2.0, 4, 5, -6.0 };
    v.SetFromArray( 4, arr );
    double p = 2;
    double ret = vectorspace::p_norm_standard( v, p );
    CHECK( 9 == ret );

    p = 1;
    ret = vectorspace::p_norm_standard( v, p );
    CHECK( 17 == ret );

    p = std::numeric_limits<double>::infinity();
    ret = vectorspace::p_norm_standard( v, p );
    CHECK( 6 == ret );
}
