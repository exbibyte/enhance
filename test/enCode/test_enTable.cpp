#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include "catch.hpp"
#include "enTable.h"
#include <math.h>
#include <string>
#include <iostream>
using namespace std;

TEST_CASE( "enTable", "[enTable]" ) {

  enTable< string, string > a;
  enTable< string, int > b;
  enTable< int, int > c;

  SECTION( "insert accessor and size" ) {
    bool exists;
    a("iuo", exists) = "item1";
    REQUIRE( exists == false );
    a("2nd", exists) = "item2";    
    REQUIRE( exists == false );
    a("2nd", exists) = "item3";
    REQUIRE( exists == true );
    a("another", exists) = "item3";
    REQUIRE( exists == false );

    REQUIRE( a.Size() == 3 );
  }    

  SECTION( "modify accessor and exist" ) {
    bool exists;
    a("iuo", exists) = "item1";
    a("2nd", exists) = "item2";    
    a("another", exists) = "item3";
    
    a("2nd", exists) = "item_changed";
    REQUIRE( exists == true );
    REQUIRE( a("2nd", exists) == "item_changed" );
    REQUIRE( a.Exists("2nd") == true );
    REQUIRE( a.Exists("notexisting") == false );
  }   

 
 
}
