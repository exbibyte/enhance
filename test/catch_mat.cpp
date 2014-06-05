#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include "catch.hpp"
#include "Mat.h"
#include <math.h>

#include <iostream>
using namespace std;

TEST_CASE( "mat", "[mat]" ) {

  Mat a;

  SECTION( "resizing dimension" ) {
    int size[2] = { 4, 4 };
    a.ResizeInt(2, size);
    REQUIRE( a._size == 16 );
    REQUIRE( a._dim[0] == 4 );
    REQUIRE( a._dim[1] == 4 );
  }

  SECTION( "accessor" ) {
    a(1,1) = 5;
    a(3,3) = -4;
    REQUIRE( a._mat[5] == 5 );
    REQUIRE( a._mat[15] == -4 );
  }

  SECTION( "copy" ) {
    a(1,1) = 5;
    a(3,3) = -4;
    
    Mat b = a;
    
    REQUIRE( b._mat[5] == 5 );
    REQUIRE( b._mat[15] == -4 );
  }

  SECTION( "multiplication" ) {
    int sizea[2] = { 4, 4 };
    a.ResizeInt(2, sizea);
    a(1,1) = 5;
    a(3,3) = -4;
    
    Mat b; 
    int size[2] = { 2, 4 };
    b.ResizeInt(2, size);
    b(0,3) = 2;
    b(1,1) = -4;

    Mat c = b * a;

    REQUIRE( c._size == 8 );
    REQUIRE( c._dim[0] == 2 );
    REQUIRE( c._dim[1] == 4 );
    REQUIRE( c(0,3) == -8 );
    REQUIRE( c(1,1) == -20 );
  }
}
