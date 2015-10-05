#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include "catch.hpp"

#include "StreamInterface.h"
#include "StreamManager.h"
#include "StreamChannel.h"

#include <list>
#include <utility>
#include <string>
#include <iostream>
#include <sstream>
using namespace std;

TEST_CASE( "StreamInterface", "[StreamInterface]" ) {

    StreamManager stream_manager;
    StreamChannel * stream_ui;
    StreamChannel * stream_logic;
    stream_manager.SelectStream( StreamType::UI, stream_ui );
    stream_manager.SelectStream( StreamType::LOGIC, stream_logic );
    StreamInterface stream_interface( &stream_manager );
    stream_interface.SelectStreamOut( StreamType::LOGIC );
    stream_interface.SelectStreamIn( StreamType::UI );
    SECTION( "retrieve from stream in" ) {
	list< pair< string, void * > > list_stream { {"id_0", nullptr}, {"id_1", nullptr} };
	stream_ui->AddStream( list_stream );
	stream_interface.FlushStreamIn( 10 ); // gets all relevant streams that is 10 timestamp units old
	string stream_string;
	void * stream_data;
	int stream_timestamp;
	bool bRet;
	for( int i = 0; i < 2; i++ ){
	    bRet = stream_interface.GetStreamIn( stream_timestamp, stream_string, stream_data );
	    CHECK( bRet );
	    CHECK( ( stream_string == "id_0" || stream_string == "id_1" ) );
	}
	bRet = stream_interface.GetStreamIn( stream_timestamp, stream_string, stream_data );
	CHECK( bRet == false );
    }
    SECTION( "sending to stream out" ) {
	int i = 9;
	int timestamp_current;
	list< StreamCapsule > list_stream;
	stream_logic->RequestStreamsRelative( 10, timestamp_current, list_stream );
	CHECK( list_stream.size() == 0 );
	
	stream_interface.SetStreamOut( "out_0", (void*)&i );
	stream_interface.FlushStreamOut();
	stream_logic->RequestStreamsRelative( 10, timestamp_current, list_stream );
        REQUIRE( list_stream.size() == 1 );
	void * stream_data;
	GetStreamData( list_stream.front(), stream_data );
	int data = *(int*)stream_data;
	CHECK( data == 9 );
    }
}
