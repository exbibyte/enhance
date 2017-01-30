#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include "catch.hpp"
#include "interp_poly_div_diff.hpp"

#include <cmath>
#include <iostream>
#include <vector>
using namespace std;

TEST_CASE( "ray coord offset", "[ray_coord]" ) {

    vector<pair<float,float> > control_points{ {-3,-5}, {-2,-1.1}, {2,1.9}, {3,4.8} };
    vector<float> x_inputs{ -2.5, 0, 2.5 };
    vector<float> y_outputs{};
    interp_poly_div_diff div_diff;
    float error = 0.001;
    bool ret = div_diff.calc_coefficients( control_points );
    CHECK( ret );

    CHECK( div_diff._coefficients[0] >= -5      - error );
    CHECK( div_diff._coefficients[0] <= -5      + error );
    CHECK( div_diff._coefficients[1] >=  3.9    - error );
    CHECK( div_diff._coefficients[1] <=  3.9    + error );
    CHECK( div_diff._coefficients[2] >= -0.63   - error );
    CHECK( div_diff._coefficients[2] <= -0.63   + error );
    CHECK( div_diff._coefficients[3] >=  0.1767 - error );
    CHECK( div_diff._coefficients[3] <=  0.1767 + error );

    ret = div_diff.calc_interpolated( x_inputs, y_outputs );
    CHECK( ret );
    REQUIRE( y_outputs.size() == 3 );
    CHECK( y_outputs[0] >= -2.694 - error );
    CHECK( y_outputs[0] <= -2.694 + error );
    CHECK( y_outputs[1] >=    0.8 - error );
    CHECK( y_outputs[1] <=    0.8 + error );
    CHECK( y_outputs[2] >=  3.044 - error );
    CHECK( y_outputs[2] <=  3.044 + error );
}
