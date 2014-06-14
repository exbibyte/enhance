#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include "catch.hpp"
#include "BufferPool.h"
#include "CircularBuffer.h"
#include <string>
#include <iostream>
#include <sstream>
using namespace std;

TEST_CASE( "ThreadPool", "[ThreadPool]" ) {
  
  BufferPool< string > tpool;

  SECTION( "Set number of buffers" ) {
    REQUIRE( tpool.GetNumBuffers() == 1 ); // default is 1
    tpool.SetNumBuffers(3);
    REQUIRE( tpool.GetNumBuffers() == 3 );
    tpool.SetNumBuffers(2);
    REQUIRE( tpool.GetNumBuffers() == 2 );
  }
  SECTION( "Get buffer at index" ) {
    CircularBuffer< string > * buf = NULL;
    tpool.SetNumBuffers(3);
    bool ret;
    for( int i = 0; i < 3; i++ ){
      ret = tpool.GetBufferAtIndex( i, buf );
      REQUIRE( ret == true );
      REQUIRE( buf != NULL );
    }
    ret = tpool.GetBufferAtIndex( -1, buf );
    REQUIRE( ret == false );
    REQUIRE( buf == NULL );
    ret = tpool.GetBufferAtIndex( 3, buf );
    REQUIRE( ret == false );
    REQUIRE( buf == NULL );
  }
  SECTION( "Add To Buffers, Clear" ) {
    CircularBuffer< string > * buf = NULL;
    tpool.SetNumBuffers(3);
    string str = "quartet";
    tpool.AddToBuffers( str );
    string str2 = "voyageur";
    tpool.AddToBuffers( str2 );

    int getcount = 0;
    for( int i = 0; i < 3; i++ ){
      tpool.GetBufferAtIndex( i, buf );
      string getstr;
      bool ret = buf->Consume(getstr);
      if(ret == true){
	REQUIRE( i <= 1 ); // test to make sure only first two buffer got the data
	if( i == 0 ){
	  REQUIRE( getstr == str );
	}else if( i == 1 ){
	  REQUIRE( getstr == str2 );
	}
	++getcount;
      }
    }
    REQUIRE( getcount == 2 );	

    //reset internal buffer index counter for tests below
    tpool.Clear();
    tpool.SetNumBuffers(3);

    //try add beyond some set limit
    for( int i = 0; i < 3; i++ ){
      tpool.GetBufferAtIndex( i, buf );
      buf->SetMaxSize(1); // make size of 1 for each buffer
    }
    for( int i = 0; i < 4; i++ ){ // add 4 items to 3 buffers
      stringstream ss;
      ss << str << i;
      string str_temp;
      ss >> str_temp;
      tpool.AddToBuffers( str_temp );
    }    
    for( int i = 0; i < 3; i++ ){ //check too make sure 4th item is not added
      tpool.GetBufferAtIndex( i, buf );
      string getstr;
      bool ret = buf->Consume(getstr);
      stringstream ss;
      ss << str << i;
      string str_temp;
      ss >> str_temp;
      REQUIRE( getstr == str_temp );
      REQUIRE( buf->GetSize() == 0 );
    }
  }
}
