#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include "catch.hpp"
#include "CircularBufferThreadSafe.h"
#include <string>
#include <iostream>
using namespace std;

TEST_CASE( "CircularBufferThreadSafe", "[CircularBufferThreadSafe]" ) {
  
  CircularBufferThreadSafe< string > cb;

  SECTION( "Add item" ) {
    REQUIRE( cb.GetSize() == 0 );
    string str = "i was here";
    cb.Add( str );
    REQUIRE( cb.GetSize() == 1 );
  }
  
  SECTION( "Set Max Size" ) {
    
    REQUIRE( cb.GetMaxSize() == -1 );
    cb.SetMaxSize(2);
    REQUIRE( cb.GetMaxSize() == 2 );
    string str1 = "i was here";
    string str2 = "Bryan is here";
    string str3 = "Richard is here";
    cb.Add( str1 );
    bool ret = cb.Add( str2 );
    REQUIRE( ret == true );
    ret = cb.Add( str3 );
    REQUIRE( ret == false );
    REQUIRE( cb.GetSize() == 2 );
  }

  SECTION( "Consume" ) {

    string str1 = "i was here";
    string str2 = "Bryan is here";
    string str3 = "Richard is here";
    cb.Add( str1 );
    cb.Add( str2 );
    cb.Add( str3 );

    REQUIRE( cb.GetSize() == 3 );
    string getStr;
    bool ret = cb.Consume( getStr );
    REQUIRE( ret == true );
    REQUIRE( cb.GetSize() == 2 );
    REQUIRE( getStr == "i was here" );

    ret = cb.Consume( getStr );
    REQUIRE( ret == true );
    REQUIRE( cb.GetSize() == 1 );
    REQUIRE( getStr == "Bryan is here" );

    ret = cb.Consume( getStr );
    REQUIRE( ret == true );
    REQUIRE( cb.GetSize() == 0 );
    REQUIRE( getStr == "Richard is here" );

    ret = cb.Consume( getStr );
    REQUIRE( ret == false );
    REQUIRE( cb.GetSize() == 0 );
  }


  SECTION( "Clear" ) {

    string str1 = "i was here";
    string str2 = "Bryan is here";
    string str3 = "Richard is here";
    cb.Add( str1 );
    cb.Add( str2 );
    cb.Add( str3 );
    REQUIRE( cb.GetSize() == 3 );
    cb.Clear();
    REQUIRE( cb.GetSize() == 0 );
  }


  SECTION( "Get Status" ) {
    cb.SetMaxSize( 4 );
    REQUIRE( cb.GetStatus() == CIRBUFFER_EMPTY );
    string str1 = "i was here";
    string str2 = "Bryan is here";
    string str3 = "Richard is here";
    cb.Add( str1 );
    cb.Add( str2 );
    cb.Add( str3 );
    REQUIRE( cb.GetStatus() == CIRBUFFER_PARTIAL );
    cb.Add( str3 );
    REQUIRE( cb.GetStatus() == CIRBUFFER_FULL );
  }
}
