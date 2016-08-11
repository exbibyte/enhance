#include <functional>
#include <deque>

#include "enComponentMeta.hpp"
#include "enEngineKernelAbstract.hpp"

using namespace std;

void enComponentMeta::SaveKernelInfo( enEngineKernelAbstract * kernel, uint32_t cid ){
    _kernel = kernel;
    _cid = cid;
}
enComponentType enComponentMeta::get_component_type() const {
    return _component_type;
}
int enComponentMeta::get_cid() const {
    return _cid;
}
char const * enComponentMeta::get_strid() const {
    return _strid;
}
void enComponentMeta::add_to_sent( std::vector<uint32_t> msg, enComponentType dest_type, uint32_t dest_cid ){
    enPacketTransmit packet;
    packet._type_src = _component_type;
    packet._type_dest = dest_type;
    packet._cid_src = _cid;
    packet._cid_dest = dest_cid;
    packet._data = std::move( msg );
    _queue_send.push_back( std::move( packet ) );
}
void enComponentMeta::add_to_received( enPacketTransmit packet ){
    _queue_receive.push_back( std::move(packet) );
}
void enComponentMeta::flush_sent(){
    while( !_queue_send.empty() ){
	enPacketTransmit & packet = _queue_send.front();
	_kernel->transmit( std::move( packet ) );
	_queue_send.pop_front();
    }
}
void enComponentMeta::process_received( std::function<bool(enPacketTransmit &)> func ){
    std::deque< enPacketTransmit > obtained {};
    obtained.swap( _queue_receive );
    while( !obtained.empty() ){
	bool continue_process = func( obtained.front() );
	obtained.pop_front();
	if( !continue_process )
	    break;
    }
}
