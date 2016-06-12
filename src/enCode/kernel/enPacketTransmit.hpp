#ifndef EN_PACKET_TRANSMIT_H
#define EN_PACKET_TRANSMIT_H

#include <vector>
#include <cstdint>

#include "enComponentType.hpp"

//structure of a transmitted message between components
struct enPacketTransmit {
    enComponentType _src;
    enComponentType _dest;
    uint_fast32_t _src_cid;
    uint_fast32_t _dest_cid;
    std::vector<int_fast32_t> _data;
};

#endif
