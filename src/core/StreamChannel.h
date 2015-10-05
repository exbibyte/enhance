#ifndef STREAM_CHANNEL
#define STREAM_CHANNEL

#include <deque>
#include <tuple>
#include <string>
#include <list>
#include <utility>

using StreamCapsule = std::tuple< int, std::string, void * > ;

enum class StreamCapsuleIndex { TIMESTAMP, STREAMSTRING, STREAMDATA };

bool GetStreamTimestamp( StreamCapsule & stream_capsule, int & timestamp );
bool GetStreamString( StreamCapsule & stream_capsule, std::string & stream_string );
bool GetStreamData( StreamCapsule & stream_capsule, void * & stream_data );

class StreamChannel {
public:
    StreamChannel() : _timestamp(0) {}
    bool RequestStreams( int timestamp_min, int & timestamp_current, std::list< StreamCapsule > & );
    bool RequestStreamsRelative( unsigned int timestamp_window, int & timestamp_current, std::list< StreamCapsule > & );
    bool AddStream( std::list< std::pair< std::string, void * > > & );
    bool AddStream( std::list< StreamCapsule > & );
    bool CleanExpiredStreams( unsigned int older_than );
    bool IncrementTimeStamp( unsigned int );
    bool SetTimeStamp( int );
    bool GetTimeStamp( int & );
private:
    std::deque< StreamCapsule > _Stream;
    int _timestamp;
};

#endif
