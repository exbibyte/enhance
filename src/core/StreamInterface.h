#ifndef STREAM_INTERFACE_H
#define STREAM_INTERFACE_H

#include "StreamChannel.h"
#include "StreamManager.h"

#include <string>
#include <queue>
#include <list>

class StreamInterface {
public:
    StreamInterface( StreamManager * );
    ~StreamInterface(){}

    bool SelectStreamOut( StreamType type );
    bool SelectStreamIn( StreamType type );
    bool SetStreamChannelIn( StreamChannel * );
    bool SetStreamChannelOut( StreamChannel * );
    
    bool GetStreamIn( int & stream_timestamp, std::string &, void * & );
    bool SetStreamOut( std::string, void * );

    bool FlushStreamOut(); // sends all streams in _StreamOut to _StreamChannelOut
    bool FlushStreamIn( unsigned int timestamp ); // gets all relevant streams from _StreamChannelIn to _StreamIn
private:
    std::list< StreamCapsule > _StreamOut;
    std::list< StreamCapsule > _StreamIn;
    StreamManager * _StreamManager;
    StreamChannel * _StreamChannelIn;
    StreamChannel * _StreamChannelOut;
};

#endif
