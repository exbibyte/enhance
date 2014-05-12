#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include "catch.hpp"
#include "Quat.h"
#include <math.h>
#include <iostream>

using namespace std;

TEST_CASE( "quaternion", "[quat]" ) {

  Quat a,b,c;
  
  for(int i = 0; i < 4; i++){
    a._quat[i] = i;
  }

  b = a;
  
  SECTION( "copy quaternion" ) {
    for( int i = 0; i < 4; i++ ){
      REQUIRE( b._quat[i] == i );
    }
  }

  SECTION( "addition" ) {
    c = a + b;
    for( int i = 0; i < 4; i++ ){
      REQUIRE( c._quat[i] == 2*i );
    }
  }    

  SECTION( "subtraction" ) {
    c = a - b;
    for( int i = 0; i < 4; i++ ){
      REQUIRE( c._quat[i] == 0 );
    }
  }    

  a._quat[0] = 2;
  a._quat[1] = 3;
  a._quat[2] = 4;
  a._quat[3] = 1;

  b._quat[0] = 6;
  b._quat[1] = 7;
  b._quat[2] = 8;
  b._quat[3] = 5;

  SECTION( "multiplication" ) {
    c = a * b; //0 to 3 times 3 to 6
    
    REQUIRE( c._quat[0] == 12 );
    REQUIRE( c._quat[1] == 30 );
    REQUIRE( c._quat[2] == 24 );
    REQUIRE( c._quat[3] == -60 );
  }    

  SECTION( "multiplication *=" ) {
    b *= a;

    REQUIRE( b._quat[0] == 20 );
    REQUIRE( b._quat[1] == 14 );
    REQUIRE( b._quat[2] == 32 );
    REQUIRE( b._quat[3] == -60 );
  }

  SECTION( "normalize and length" ) {
    a.Normalize();
    double eval = 0;
    for( int i = 0; i < 4; i++){
      eval += a._quat[i] * a._quat[i];
    }
    
    eval = sqrt(eval);
    REQUIRE( eval >= 1 - 0.0001);
    REQUIRE( eval <= 1 + 0.0001);
  }

  SECTION( "axis angle" ) {
    Vec v;
    v._vec[0] = 1;
    v._vec[1] = 2;
    v._vec[2] = -3;
    c.AxisAngleDegreeVector( v, 45 );  

    float error = 0.0001;

    REQUIRE( c._quat[0] >= 0.10228 - error);
    REQUIRE( c._quat[0] <= 0.10228 + error);
    REQUIRE( c._quat[1] >= 0.20455 - error);
    REQUIRE( c._quat[1] <= 0.20455 + error);
    REQUIRE( c._quat[2] >= -0.30683 - error);
    REQUIRE( c._quat[2] <= -0.30683 + error);
    REQUIRE( c._quat[3] >= 0.92388 - error);
    REQUIRE( c._quat[3] <= 0.92388 + error);
  }

  SECTION( "power" ) {
     a._quat[0] = 2;
     a._quat[1] = 3;
     a._quat[2] = 4;
     a._quat[3] = -3;
     // a.Normalize();
     c = a.Pow(3);

    float error = 0.0001;

     REQUIRE( c._quat[0] >= -4 - error );
     REQUIRE( c._quat[0] <= -4 + error );
     REQUIRE( c._quat[1] >= -6 - error );
     REQUIRE( c._quat[1] <= -6 + error );
     REQUIRE( c._quat[2] >= -8 - error );
     REQUIRE( c._quat[2] <= -8 + error );
     REQUIRE( c._quat[3] >= 234 - error );     
     REQUIRE( c._quat[3] <= 234 + error );
     
  }

  SECTION( "accessor []" ) {
    a._quat[0] = 2;
    a._quat[1] = 3;
    a._quat[2] = 4;
    a._quat[3] = -3;

    a[3] = 5;
    
    REQUIRE( a._quat[3] == 5 );
     
  }

  SECTION( "To Matrix" ) {
    a._quat[0] = 1;
    a._quat[1] = 2;
    a._quat[2] = 3;
    a._quat[3] = 2;
    
    float mat[16];
    a.ToMatrix(mat);

    REQUIRE( mat[0] == -25 );
    REQUIRE( mat[1] == 16 );
    REQUIRE( mat[2] == -2 );
    REQUIRE( mat[3] == 0 );

    REQUIRE( mat[4] == -8 );
    REQUIRE( mat[5] == -19 );
    REQUIRE( mat[6] == 16 );
    REQUIRE( mat[7] == 0 );

    REQUIRE( mat[8] == 14 );
    REQUIRE( mat[9] == 8 );
    REQUIRE( mat[10] == -9 );
    REQUIRE( mat[11] == 0 );

    REQUIRE( mat[12] == 0 );
    REQUIRE( mat[13] == 0 );
    REQUIRE( mat[14] == 0 );
    REQUIRE( mat[15] == 1 );    
  }
}
