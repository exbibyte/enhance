#ifndef EN_PACKET_TRANSMIT_H
#define EN_PACKET_TRANSMIT_H

#include <vector>
#include <cstdint>

#include "enComponentType.hpp"

//structure of a transmitted message between components
struct enPacketTransmit {
    enComponentType _type_src;
    enComponentType _type_dest;
    uint32_t _cid_src;
    uint32_t _cid_dest;
    std::vector<uint32_t> _data;
};

#endif
