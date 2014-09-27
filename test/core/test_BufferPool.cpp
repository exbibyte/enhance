#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include "catch.hpp"
#include "BufferPool.h"
#include "CircularBufferThreadSafe.h"
#include <string>
#include <iostream>
#include <sstream>
using namespace std;

TEST_CASE( "ThreadPool", "[ThreadPool]" ) {
  
    BufferPool< CircularBufferThreadSafe<string>, string > tpool;

    SECTION( "Set number of buffers" ) {
        CHECK( tpool.GetNumBuffers() == 1 ); // default is 1
        tpool.SetNumBuffers(3);
        CHECK( tpool.GetNumBuffers() == 3 );
        tpool.SetNumBuffers(2);
        CHECK( tpool.GetNumBuffers() == 2 );
    }
    SECTION( "Get buffer at index" ) {
        CircularBufferThreadSafe< string > * buf = NULL;
        tpool.SetNumBuffers(3);
        bool ret;
        for( int i = 0; i < 3; i++ ){
            ret = tpool.GetBufferAtIndex( i, buf );
            CHECK( ret == true );
            CHECK( buf != NULL );
        }
        ret = tpool.GetBufferAtIndex( -1, buf );
        CHECK( ret == false );
        CHECK( buf == NULL );
        ret = tpool.GetBufferAtIndex( 3, buf );
        CHECK( ret == false );
        CHECK( buf == NULL );
    }
    SECTION( "Add and Consume Target Buffers" ) {
        CircularBufferThreadSafe< string > * buf = NULL;
        tpool.SetNumBuffers(3);
        string str = "quartet";
        string temp = string(str);
        tpool.AddToBuffers( temp );
        string str2 = "voyageur";
        temp = string(str2);
        tpool.AddToBuffers( temp );

        int getcount = 0;
        for( int i = 0; i < 3; i++ ){
            bool ret = tpool.GetBufferAtIndex( i, buf );
            REQUIRE( true == ret );
            string getstr;
            ret = buf->Consume(getstr);
            if( i == 2 ){
                CHECK( false == ret );
            }else{
                CHECK( true == ret );
            }
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
            bool ret = tpool.AddToBuffers( str_temp );
            if( i == 3 ){
                CHECK( false == ret );
            }
            else{
                CHECK( true == ret );
            }
        }    
        for( int i = 0; i < 3; i++ ){ //check to make sure 4th item is not added
            bool ret = tpool.GetBufferAtIndex( i, buf );
            CHECK( true == ret );      
            string getstr;
            ret = buf->Consume(getstr);
            CHECK( true == ret );
            stringstream ss;
            ss << str << i;
            string str_temp;
            ss >> str_temp;
            CHECK( getstr == str_temp );
            CHECK( buf->GetSize() == 0 );
        }
    }

    SECTION( "Consume Random Buffers" ) {
        CircularBufferThreadSafe< string > * buf = NULL;
        tpool.SetNumBuffers(3);
        string str = "quartet";
        //reset internal buffer index counter for tests below
        tpool.Clear();
        tpool.SetNumBuffers(3);

        //try add beyond some set limit
        for( int i = 0; i < 3; i++ ){
            bool ret = tpool.GetBufferAtIndex( i, buf );
            CHECK( true == ret );
            buf->SetMaxSize(2); 
        }
        tpool.ResetIndex();
        for( int i = 0; i < 4; i++ ){ // add 4 items to 3 buffers
            stringstream ss;
            ss << str << i;
            string str_temp;
            ss >> str_temp;
            tpool.AddToBuffers( str_temp );
        }  
        tpool.ResetIndex();
        for( int i = 0; i < 3; i++ ){ // consume random buffers 3 times and check results
            string getstr;
            bool ret = tpool.ConsumeBuffersRandom(getstr);
            CHECK( true == ret );
            stringstream ss;
            ss << str << i;
            string str_temp;
            ss >> str_temp;
            CHECK( getstr == str_temp );
        }
        tpool.ResetIndex();
        for( int i = 0; i < 3; i++ ){ //check to make sure there is 1 remaining item in 1st buffer and other buffers are empty
            bool ret = tpool.GetBufferAtIndex( i, buf );
            REQUIRE( true == ret );
            if( i == 0){
                CHECK( buf->GetSize() == 1 );
            }else{
                CHECK( buf->GetSize() == 0 );
            }
        }
    }
}
