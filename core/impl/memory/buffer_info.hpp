#ifndef E2_BUFFERINFO_HPP
#define E2_BUFFERINFO_HPP

#include <string>
#include <cstring>

namespace e2 { namespace memory {

class buffer_info {
public:
    size_t _id;
    size_t _offset;
    size_t _length;
};

} }

#endif
