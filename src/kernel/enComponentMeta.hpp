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
    void SaveKernelInfo( enEngineKernelAbstract * kernel, uint32_t cid );
    enComponentType get_component_type() const;
    int get_cid() const;
    char const * get_strid() const;
protected:
    void add_to_sent( std::vector<uint32_t> msg, enComponentType dest, uint32_t dest_cid = 0 );
    void add_to_received( enPacketTransmit packet );
    void flush_sent();
    void process_received( std::function<bool(enPacketTransmit &)> func );
private:
    enComponentType _component_type;
    uint32_t _cid;
    enEngineKernelAbstract * _kernel;
    std::deque< enPacketTransmit > _queue_receive;
    std::deque< enPacketTransmit > _queue_send;
    char _strid[64] = "\0";
};

#endif
