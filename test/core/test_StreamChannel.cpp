#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include "catch.hpp"

#include "StreamChannel.h"

#include <list>
#include <utility>
#include <string>
#include <iostream>
#include <sstream>
using namespace std;

TEST_CASE( "StreamChannel", "[StreamChannel]" ) {

    StreamChannel stream_channel;
    
    SECTION( "Add and get streams" ) {
	std::list< std::pair< std::string, void * > > list_streams { {"id_0", nullptr }, {"id_1", nullptr }, {"id_2", nullptr } };
	CHECK( stream_channel.AddStream( list_streams ) == true );
	CHECK( stream_channel.IncrementTimeStamp( 10 ) == true );
	int timestamp_min = 0;
	int timestamp_current;
	std::list< StreamCapsule > list_streams_check;
	CHECK( stream_channel.RequestStreams( timestamp_min, timestamp_current, list_streams_check ) == true );
	CHECK( 10 == timestamp_current );
	CHECK( list_streams_check.size() == 3 );
	for( auto & i : list_streams_check ){
	    int timestamp;
	    GetStreamTimestamp( i, timestamp );
	    CHECK( 0 == timestamp );
	    string stream_string;
	    GetStreamString( i, stream_string );
	    CHECK( ("id_0" == stream_string || "id_1" == stream_string || "id_2" == stream_string) );
	}
    }
    SECTION( "Set timestamp and clean expired timestamp" ) {
	CHECK( stream_channel.SetTimeStamp( 10 ) == true );
	std::list< std::pair< std::string, void * > > list_streams { {"id_0", nullptr }, {"id_1", nullptr }, {"id_2", nullptr } };
	CHECK( stream_channel.AddStream( list_streams ) == true );

	//set time to 20 and retrieve all streams from 15 onward
	CHECK( stream_channel.SetTimeStamp( 20 ) == true );
	int timestamp_min = 15;
	int timestamp_current;
	std::list< StreamCapsule > list_streams_check;
	CHECK( stream_channel.RequestStreams( timestamp_min, timestamp_current, list_streams_check ) == true );
	CHECK( 20 == timestamp_current );
	CHECK( list_streams_check.size() == 0 );

	//clean streams with timestamp less than 11 from current timestamp, set current timestamp to 20 - 11 = 9
	CHECK( stream_channel.CleanExpiredStreams( 11 ) == true );
	CHECK( stream_channel.GetTimeStamp( timestamp_current ) == true );
	CHECK( 9 == timestamp_current );
	//retrieve all streams >= timestamp of 7
	timestamp_min = 7; 
	CHECK( stream_channel.RequestStreams( timestamp_min, timestamp_current, list_streams_check ) == true );
	CHECK( list_streams_check.size() == 0 );
	//retrieve all streams >= timestamp of -1
	timestamp_min = -1;
	CHECK( stream_channel.RequestStreams( timestamp_min, timestamp_current, list_streams_check ) == true );
	CHECK( list_streams_check.size() == 3 );
	//clean streams with timestamp less than or equal to current timestamp, set current timestamp to 9 - 0 = 9
	CHECK( stream_channel.CleanExpiredStreams( 0 ) == true );
	CHECK( stream_channel.GetTimeStamp( timestamp_current ) == true );
	CHECK( 9 == timestamp_current );
	//retrieve all streams >= timestamp of -1
	timestamp_min = -1;
	CHECK( stream_channel.RequestStreams( timestamp_min, timestamp_current, list_streams_check ) == true );
	CHECK( list_streams_check.size() == 0 );
    }
}
