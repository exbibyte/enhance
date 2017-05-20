#ifndef E2_I_RENDER_PAYLOAD_HPP
#define E2_I_RENDER_PAYLOAD_HPP

#include <cstdint>

#include "buffer.hpp"

namespace e2 { namespace interface {

enum e_renderpayload_type : uint64_t { //list of primitive resources that can be manipulated
    na = 1,
    float_n,
    int_n,
    uint_n,
    mat_3,
    mat_4,
    quat,
    text,
};
	
class i_renderpayload {
public:
        e_renderpayload_type _payload_type;
                      size_t _id;
      ::e2::memory::buffer * _buf;
                      size_t _offset;
};
	
} }

#endif
