#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include "catch.hpp"
#include "DualQuat.h"
#include "Quat.h"
#include <math.h>
#include <iostream>

using namespace std;

TEST_CASE( "dualquaternion", "[dualquat]" ) {

  DualQuat a,b,c;
  
  for(int i = 0; i < 4; i++){
    a._A._quat[i] = i;
    a._B._quat[i] = i;
    b._A._quat[i] = 3 - i;
    b._B._quat[i] = 5 + 3 - i;
  }

  SECTION( "addition" ) {
    c = a + b;
    for( int i = 0; i < 4; i++ ){
      REQUIRE( c._A._quat[i] == 3 );
      REQUIRE( c._B._quat[i] == 3 + 5 );
    }
  }    

  SECTION( "subtraction" ) {
    a._A = Quat(5,5,5,5);
    a._B = Quat(6,6,6,6);

    b._A = Quat(1,2,3,4);
    b._B = Quat(4,3,2,1);

    c = a - b;

    for( int i = 0; i < 4; i++ ){
      REQUIRE( c._A._quat[i] == 4 - i );
      REQUIRE( c._B._quat[i] == 2 + i );
    }
  }   

  SECTION( "mutiplication" ) {
    a._A = Quat(1,2,3,4);
    a._B = Quat(5,6,7,8);

    b._A = Quat(-1,-2,-3,4);
    b._B = Quat(9,10,11,12);

    c = a * b;

    //c._A = a._A * b._A
    REQUIRE( c._A._quat[0] == 0 );
    REQUIRE( c._A._quat[1] == 0 );
    REQUIRE( c._A._quat[2] == 0 );
    REQUIRE( c._A._quat[3] == 30 );

    //c._B = a._A * b._B + a._B * b._A:
    //-14 + 40i + 80j + 72k +
    // 70 + 8i + 16j + 0k

    REQUIRE( c._B._quat[0] == 48 );
    REQUIRE( c._B._quat[1] == 96 );
    REQUIRE( c._B._quat[2] == 72 );
    REQUIRE( c._B._quat[3] == 56 );

  }

  SECTION( "conjugate" ) {
    a._A = Quat(1,2,3,4);
    a._B = Quat(-1,-2,-3,-4);

    c = a.Conjugate();

    for( int i = 0; i < 3; i++ ){
      REQUIRE( c._A._quat[i] == -1 - i );
      REQUIRE( c._B._quat[i] == 1 + i );
    }
    REQUIRE( c._A._quat[3] == 4 );
    REQUIRE( c._B._quat[3] == -4 );
  } 
 
}
