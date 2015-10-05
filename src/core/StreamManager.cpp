#include "StreamManager.h"
#include "StreamChannel.h"

#include <map>
#include <utility>

using namespace std;

StreamManager::StreamManager(){
    _MapStreamChannel.insert( std::pair< StreamType, StreamChannel * >( StreamType::GRAPHICS, new StreamChannel ) );
    _MapStreamChannel.insert( std::pair< StreamType, StreamChannel * >( StreamType::LOGIC, new StreamChannel ) );
    _MapStreamChannel.insert( std::pair< StreamType, StreamChannel * >( StreamType::UI, new StreamChannel ) );
}
StreamManager::~StreamManager(){
    for( auto & i : _MapStreamChannel ){
	if( i.second ){
	    delete i.second;
	    i.second = nullptr;
	}
    }
    _MapStreamChannel.clear();
}
bool StreamManager::SelectStream( StreamType type, StreamChannel * & stream ){
    auto it = _MapStreamChannel.find( type );
    if( it == _MapStreamChannel.end() ){
	return false;
    }
    stream = it->second;
    return true;
}
