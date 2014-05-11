#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include "catch.hpp"
#include "Vec.h"

TEST_CASE( "vector", "[vec]" ) {
  
  Vec a = Vec();
  Vec b = Vec(4);
  a.SetDim(4);

  for( int i = 0; i < 4; i++ ){ 
    a._vec[i] = i;
    b._vec[i] = i + 10;
  }

  Vec c = a+b;

  REQUIRE( c._vec[0] == 10 );
  REQUIRE( c._vec[1] == 12 );
  REQUIRE( c._vec[2] == 14 );
  REQUIRE( c._vec[3] == 16 );
}
