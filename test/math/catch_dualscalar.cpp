#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include "catch.hpp"
#include "DualScalar.h"
#include <math.h>
#include <iostream>

using namespace std;

TEST_CASE( "dualscalar", "[dualscalar]" ) {

  DualScalar a, b, c;
  
  float v [2] = { 6, 7 };

  DualScalar d( 3, 5 );
  a = d;
  b.SetArray( v );
  c.SetDualNumber( -3, -2 );

  float error_delta = 1e-5;

  SECTION( "initialization" ) {
    REQUIRE( a.GetReal() == 3 );
    REQUIRE( a.GetDual() == 5 );
    REQUIRE( b.GetReal() == 6 );
    REQUIRE( b.GetDual() == 7 );

    float g [2];
    c.GetArray( g );
    REQUIRE( g[0] == -3 );
    REQUIRE( g[1] == -2 );
  }

  SECTION( "arithmetic" ) {
    DualScalar e;
    e = a+b;
    REQUIRE( e.GetReal() == 9 );
    REQUIRE( e.GetDual() == 12 );    

    e = a - b;
    REQUIRE( e.GetReal() == -3 );
    REQUIRE( e.GetDual() == -2 );    

    e = a;
    e += b;
    REQUIRE( e.GetReal() == 9 );
    REQUIRE( e.GetDual() == 12 );   

    e = a;
    e -= b;
    REQUIRE( e.GetReal() == -3 );
    REQUIRE( e.GetDual() == -2 );   

    e = a * b;
    REQUIRE( e.GetReal() == 18 );
    REQUIRE( e.GetDual() == 51 );   
    
    e = a;
    e *= b;
    REQUIRE( e.GetReal() == 18 );
    REQUIRE( e.GetDual() == 51 );   

    e = a/b;
    REQUIRE( e.GetReal() == 0.5 );
    REQUIRE( e.GetDual() == 0.5 );   
    
    e = a;
    e /= b;
    REQUIRE( e.GetReal() == 0.5 );
    REQUIRE( e.GetDual() == 0.5 );
  }

  SECTION( "Other functions" ) {
    
    DualScalar e;

    e = c;
    e = e.Conjugate();
    REQUIRE( e.GetReal() == -3 );
    REQUIRE( e.GetDual() == 2 );

    float norm = c.Norm();
    REQUIRE( norm == 3 );

    e = a.Sqrt();
    REQUIRE( e.GetReal() <= sqrt(3) + error_delta );
    REQUIRE( e.GetReal() >= sqrt(3) - error_delta );
    REQUIRE( e.GetDual() <= a.GetDual() / (2*sqrt(3)) + error_delta );    
    REQUIRE( e.GetDual() >= a.GetDual() / (2*sqrt(3)) - error_delta );    

    DualScalar f;
    f.SetDualNumber(2, -6);
    e = a.Pow(f);
    REQUIRE( e.GetReal() == 9 );
    REQUIRE( e.GetDual() <= -29.32506 + error_delta );    
    REQUIRE( e.GetDual() >= -29.32506 - error_delta );    

    e = c.Invert();
    REQUIRE( e.GetReal() <= 1.0/-3 + error_delta );
    REQUIRE( e.GetReal() >= 1.0/-3 - error_delta );
    REQUIRE( e.GetDual() <= 2 * -1.0/3 * -1.0/3 + error_delta);    
    REQUIRE( e.GetDual() >= 2 * -1.0/3 * -1.0/3 - error_delta);    
  }
}
