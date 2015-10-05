#include "StreamInterface.h"
#include "StreamManager.h"
#include "StreamChannel.h"

#include <string>
#include <list>
using namespace std;

StreamInterface::StreamInterface( StreamManager * stream_manager ){
    _StreamChannelOut = nullptr;
    _StreamChannelIn = nullptr;
    _StreamManager = stream_manager;
}
bool StreamInterface::SetStreamChannelIn( StreamChannel * stream_channel ){
    _StreamChannelIn = stream_channel;
    return true;
}
bool StreamInterface::SetStreamChannelOut( StreamChannel * stream_channel ){
    _StreamChannelOut = stream_channel;
    return true;
}
bool StreamInterface::SelectStreamOut( StreamType type ){
    bool bRet = _StreamManager->SelectStream( type, _StreamChannelOut );
    return bRet;
}
bool StreamInterface::SelectStreamIn( StreamType type ){
    bool bRet = _StreamManager->SelectStream( type, _StreamChannelIn );
    return bRet;
}
bool StreamInterface::GetStreamIn( int & stream_timestamp, string & stream_string, void * & stream_data ){
    if( !_StreamIn.empty() ){
	StreamCapsule current_capsule = _StreamIn.front();
	_StreamIn.pop_front();
	GetStreamTimestamp( current_capsule, stream_timestamp );
	GetStreamString( current_capsule, stream_string );
	GetStreamData( current_capsule, stream_data );
	return true;
    }
    return false;
}
bool StreamInterface::SetStreamOut( string stream_string, void * stream_data ){
    int iTimeStamp;
    _StreamChannelOut->GetTimeStamp( iTimeStamp );
    StreamCapsule new_capsule = make_tuple( iTimeStamp, stream_string, stream_data );
    _StreamOut.push_back( new_capsule );
    return true;
}
bool StreamInterface::FlushStreamOut(){
    _StreamChannelOut->AddStream( _StreamOut );
    _StreamOut.clear();
    return true;
}
bool StreamInterface::FlushStreamIn( unsigned int timestamp_duration ){
    int timestamp_current;
    bool bRet = _StreamChannelIn->RequestStreamsRelative( timestamp_duration, timestamp_current, _StreamIn );
    return bRet;
}
