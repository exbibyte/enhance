#include <functional>
#include <deque>

#include "enComponentMeta.hpp"
#include "enEngineKernelAbstract.hpp"

using namespace std;

void enComponentMeta::SaveKernelInfo( enEngineKernelAbstract * kernel, uint_fast32_t cid ){
    _kernel = kernel;
    _cid = cid;
}
enComponentType enComponentMeta::get_component_type(){
    return _component_type;
}
int enComponentMeta::get_cid(){
    return _cid;
}
void enComponentMeta::send( enComponentType dest, std::vector<int_fast32_t> msg ){
    send( dest, 0, msg );
}
void enComponentMeta::send( enComponentType dest, uint_fast32_t dest_cid, std::vector<int_fast32_t> msg ){
    enPacketTransmit packet;
    packet._src = _component_type;
    packet._dest = dest;
    packet._src_cid = _cid;
    packet._dest_cid = dest_cid;
    packet._data = std::move( msg );
    _kernel->transmit( packet ); 
}
void enComponentMeta::receive( enPacketTransmit packet ){
    _queue_receive.push_back( packet );
}
void enComponentMeta::process_received_messages( std::function<void(enPacketTransmit &)> func ){
    std::deque< enPacketTransmit > obtained;
    obtained.swap( _queue_receive );
    while( !obtained.empty() ){
	func( obtained.front() );
	obtained.pop_front();
    }
}
