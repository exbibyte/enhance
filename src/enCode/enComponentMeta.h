#ifndef EN_COMPONENT_META
#define EN_COMPONENT_META

#include <functional>
#include <deque>

#include "enEngineKernelAbstract.h"

class enComponentMeta {
public:
    enComponentMeta( enComponentType type ) : _component_type(type) {}
    void SaveKernelInfo( enEngineKernelAbstract * kernel, uint_fast32_t cid ){
        _kernel = kernel;
        _cid = cid;
    }
    enComponentType get_component_type(){
      return _component_type;
    }
    int get_cid(){
      return _cid;
    }
protected:
    void send( enComponentType dest, std::vector<int_fast32_t> msg ){
        send( dest, 0, msg );
    }
    void send( enComponentType dest, uint_fast32_t dest_cid, std::vector<int_fast32_t> msg ){
	enPacketTransmit packet;
	packet._src = _component_type;
	packet._dest = dest;
	packet._src_cid = _cid;
	packet._dest_cid = dest_cid;
	packet._data = std::move( msg );
        _kernel->transmit( packet ); 
    }
    void receive( enPacketTransmit packet ){
	_queue_receive.push_back( packet );
    }
    void process_received_messages( std::function<void(enPacketTransmit &)> func ){
	std::deque< enPacketTransmit > obtained;
	obtained.swap( _queue_receive );
	while( !obtained.empty() ){
	    func( obtained.front() );
	    obtained.pop_front();
	}
    }
private:
    uint_fast32_t _cid;
    enEngineKernelAbstract * _kernel;
    enComponentType _component_type;
    std::deque< enPacketTransmit > _queue_receive;
};

#endif
