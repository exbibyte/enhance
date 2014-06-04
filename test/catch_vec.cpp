#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include "catch.hpp"
#include "Vec.h"
#include <math.h>

TEST_CASE( "vector", "[vec]" ) {

  Vec a = Vec(5);
  Vec b = Vec(4);
  
  for( int i = 0; i < 4; i++ ){ 
    a._vec[i] = i; //0 to 3
    b._vec[i] = i + 10; //10 to 13
  }
  a._vec[4] = 4;

  SECTION( "resizing dimension" ) {
    a.SetDim(4);
    REQUIRE( a._dim == 4 );
    for( int i = 0; i < 4; i++ ){ 
      REQUIRE( a._vec[i] == i );
    }
  }

  a.SetDim(4);
  Vec copied = a;
  SECTION( "copy"){
    for( int i = 0; i < 4; i++ ){ 
      REQUIRE( copied._vec[i] == a._vec[i] );
    }    
  }

  SECTION( "adddition" ) {
    Vec c = copied+b;

    REQUIRE( c._vec[0] == 10 );
    REQUIRE( c._vec[1] == 12 );
    REQUIRE( c._vec[2] == 14 );
    REQUIRE( c._vec[3] == 16 );
  }

  SECTION( "subtraction" ) {
    Vec c = b-copied;

    REQUIRE( c._vec[0] == 10 );
    REQUIRE( c._vec[1] == 10 );
    REQUIRE( c._vec[2] == 10 );
    REQUIRE( c._vec[3] == 10 );
  }

  SECTION( "dot product" ) {
    float c = b.Dot(copied);

    REQUIRE( c == 74 );
  }

  
  SECTION( "cross product" ) {
    copied.SetDim(3);
    b.SetDim(3);
    Vec c = b.Cross(copied);

    REQUIRE( c._vec[0] == 10 );
    REQUIRE( c._vec[1] == -20 );
    REQUIRE( c._vec[2] == 10 );
  }  

  SECTION( "magnitude" ) {
    float expected = sqrt(1+4+9);
    REQUIRE( a.Magnitude() >= expected - expected * 0.001 );
    REQUIRE( a.Magnitude() <= expected + expected * 0.001 );
  }  

  SECTION( "normalize" ) {
    a.NormalizeThis();
    Vec c = a;

    float val = 0;
    
    for(int i = 0; i < 4; i++){
      val += c._vec[i]*c._vec[i];
    }
    
    val = sqrt(val);

    REQUIRE( val >= 0.9999 );
    REQUIRE( val <= 1.0001 );
  }  

  SECTION( "copy from array" ) {
    float test_array[5] = {4,3,2,1,0};
    
    Vec copiedArray;
    copiedArray.SetFromArray( 5, test_array );

    for( int i = 0; i < 5; i++ ){
      REQUIRE( copiedArray._vec[i] == 4 - i );
    }  
  }

  SECTION( "copy to array" ) {
    float * test_array;
    
    Vec copiedArray;
    copiedArray.SetDim(7);

    for( int i = 0; i < 7; i++ ){
      copiedArray._vec[i] = i;
    }

    int dim;
    copiedArray.GetArray(dim, test_array );

    REQUIRE( dim == 7 );

    for( int i = 0; i < 7; i++ ){
      REQUIRE( test_array[i] == i );
    }
  }
}
