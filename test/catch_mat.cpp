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

  SECTION( "GetVec" ) {
    int sizea[2] = { 4, 4 };
    a.ResizeInt(2, sizea);
    a(1,1) = 5;
    a(3,2) = -4;
    
    Vec v;
    bool isCol = false;
    a.GetVec( v, 3, isCol );
    REQUIRE( v[0] == 0);
    REQUIRE( v[1] == 0);
    REQUIRE( v[2] == -4);
    REQUIRE( v[3] == 0);
  }

  SECTION( "SetFromVec" ) {
    Vec v;
    v.SetDim(2);
    v._vec[0] = 4;
    v._vec[1] = 5;

    bool isCol = false;
    a.SetFromVec( v, isCol );
    REQUIRE( a._dim[0] == 1);
    REQUIRE( a._dim[1] == 2);
    REQUIRE( a(0,0) == 4);
    REQUIRE( a(0,1) == 5);
  }
  
  SECTION( "GetSumMat" ) {
    int sizea[2] = { 4, 4 };
    a.ResizeInt(2, sizea);
    a(2,2) = 5;
    a(3,3) = -4;
    
    Mat temp;

    a.GetSubMat( temp, 2, 1, 2, 3 );
    
    REQUIRE( temp._dim[0] == 2);
    REQUIRE( temp._dim[1] == 3);
    REQUIRE( temp(0,0) == 0);
    REQUIRE( temp(0,2) == 0);
    REQUIRE( temp(1,1) == 0);
    REQUIRE( temp(1,0) == 0);
    REQUIRE( temp(0,1) == 5);
    REQUIRE( temp(1,2) == -4);
  }

  SECTION( "addition" ) {
    int sizea[2] = { 3, 3 };
    a.ResizeInt(2, sizea);
    a(1,1) = 5;
    a(0,2) = -4;
    
    Mat b; 
    int size[2] = { 3, 3 };
    b.ResizeInt(2, size);
    b(1,1) = 3;
    b(1,2) = -10;

    Mat c = b + a;

    REQUIRE( c._size == 9 );
    REQUIRE( c._dim[0] == 3 );
    REQUIRE( c._dim[1] == 3 );
    REQUIRE( c(0,0) == 0 );
    REQUIRE( c(1,1) == 8 );
    REQUIRE( c(0,2) == -4 );
    REQUIRE( c(1,2) == -10 );
  }

  SECTION( "subtraction" ) {
    int sizea[2] = { 3, 3 };
    a.ResizeInt(2, sizea);
    a(1,1) = 5;
    a(0,2) = -4;
    
    Mat b; 
    int size[2] = { 3, 3 };
    b.ResizeInt(2, size);
    b(1,1) = 3;
    b(1,2) = -10;

    Mat c = b - a;

    REQUIRE( c._size == 9 );
    REQUIRE( c._dim[0] == 3 );
    REQUIRE( c._dim[1] == 3 );
    REQUIRE( c(0,0) == 0 );
    REQUIRE( c(1,1) == -2 );
    REQUIRE( c(0,2) == 4 );
    REQUIRE( c(1,2) == -10 );
    REQUIRE( c(2,2) == 0 );
  }

}
