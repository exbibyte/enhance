#include "StreamChannel.h"

#include <deque>
#include <tuple>
#include <string>
#include <list>
#include <utility>
using namespace std;

bool GetStreamTimestamp( StreamCapsule & stream_capsule, int & timestamp ){
    timestamp = std::get< static_cast<int>( StreamCapsuleIndex::TIMESTAMP ) >( stream_capsule );
    return true;
}
bool GetStreamString( StreamCapsule & stream_capsule, std::string & stream_string ){
    stream_string = std::get< static_cast<int>( StreamCapsuleIndex::STREAMSTRING ) >( stream_capsule );
    return true;
}
bool GetStreamData( StreamCapsule & stream_capsule, void * & stream_data ){
    stream_data = std::get< static_cast<int>( StreamCapsuleIndex::STREAMDATA ) >( stream_capsule );
    return true;
}

bool StreamChannel::RequestStreams( int timestamp_min, int & timestamp_current, list< StreamCapsule > & stream_data ){
    stream_data.clear();
    auto it_begin = _Stream.begin();
    auto it_end = _Stream.end();
    while( it_begin != it_end ){
        int stream_timestamp = std::get< static_cast<int>( StreamCapsuleIndex::TIMESTAMP ) >(*it_begin);
	if( stream_timestamp >= timestamp_min ){ //search for items with appropriate timestamp requirement
	    break;
	}
	++it_begin;
    }
    stream_data.assign( it_begin, it_end ); //copy stream items
    timestamp_current = _timestamp;
    return true;
}
bool StreamChannel::RequestStreamsRelative( unsigned int timestamp_window, int & timestamp_current, list< StreamCapsule > & stream_data ){
    int timestamp_absolute = _timestamp - timestamp_window;
    bool bRet = RequestStreams( timestamp_absolute, timestamp_current, stream_data );
    return bRet;
}
bool StreamChannel::AddStream( std::list< std::pair< std::string, void * > > & stream ){
    for( auto & i : stream ){
	_Stream.push_back( make_tuple( _timestamp, i.first, i.second ) );
    }
    return true;
}
bool StreamChannel::AddStream( std::list< StreamCapsule > & stream ){
    _Stream.insert( _Stream.end(), stream.begin(), stream.end() );
    return true;
}
bool StreamChannel::CleanExpiredStreams( unsigned int older_than ){
    if( older_than > _timestamp ){
	older_than = _timestamp;
    }
    _timestamp = _timestamp - older_than;
    auto it_begin = _Stream.begin();
    auto it = it_begin;
    auto it_end = _Stream.end();
    while( it != it_end ){
	int stream_timestamp = std::get< static_cast<int>( StreamCapsuleIndex::TIMESTAMP ) >(*it);
	if( stream_timestamp > _timestamp ){ //search for items with appropriate timestamp requirement
	    break;
	}
	++it;
    }
    _Stream.erase( it_begin, it );
    while( it != it_end ){
	std::get< static_cast<int>( StreamCapsuleIndex::TIMESTAMP ) >(*it) -= older_than;
	++it;
    }
    return true;
}
bool StreamChannel::IncrementTimeStamp( unsigned int increment){
    _timestamp += increment;
    return true;
}
bool StreamChannel::SetTimeStamp( int val ){
    _timestamp = val;
    return true;
}
bool StreamChannel::GetTimeStamp( int & val ){
    val = _timestamp;
    return true;
}
