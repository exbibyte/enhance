#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include "catch.hpp"
#include "Vec.h"

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

}
