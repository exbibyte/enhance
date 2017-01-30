#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include "catch.hpp"
#include "least_squares.hpp"

#include <cmath>
#include <iostream>
#include <vector>

using namespace std;

TEST_CASE( "least squares bad input", "[least_squares]" ) {
    vector<pair<float,float> > inputs {};
    float offset, slope;
    bool ret = least_squares::calc_coefficients( inputs, offset, slope );
    CHECK( false == ret );
}

TEST_CASE( "least squares good input", "[least_squares]" ) {
    vector<pair<float,float> > inputs { {-4,-3}, {-3,-1}, {-2,-2}, {-1.5,-0.5}, {-0.5,1}, {1,0}, {2,1.5}, {3.5,1}, {4,2.5} };
    
    float offset, slope;
    bool ret = least_squares::calc_coefficients( inputs, offset, slope );
    CHECK( ret );
    float error = 0.0001;
    CHECK( offset >= -0.0249 - error );
    CHECK( offset <= -0.0249 + error );
    CHECK( slope >= 0.5519 - error );
    CHECK( slope <= 0.5519 + error );
}
