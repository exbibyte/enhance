#ifndef EN_COMPONENT_META_H
#define EN_COMPONENT_META_H

#include <functional>
#include <deque>
#include <cstring>

#include "enPacketTransmit.hpp"

class enEngineKernelAbstract;

class enComponentMeta {
public:
    virtual ~enComponentMeta(){}
    enComponentMeta( enComponentType type, char const * strid ) : _component_type(type) {
	strncpy( _strid, strid, 64 );
	_strid[63] = '\0';
    }
    void SaveKernelInfo( enEngineKernelAbstract * kernel, uint_fast32_t cid );
    enComponentType get_component_type();
    int get_cid();
protected:
    void send( enComponentType dest, std::vector<int_fast32_t> msg );
    void send( enComponentType dest, uint_fast32_t dest_cid, std::vector<int_fast32_t> msg );
    void receive( enPacketTransmit packet );
    void process_received_messages( std::function<void(enPacketTransmit &)> func );
private:
    enComponentType _component_type;
    uint_fast32_t _cid;
    enEngineKernelAbstract * _kernel;
    std::deque< enPacketTransmit > _queue_receive;
    char _strid[64] = "\0";
};

#endif
