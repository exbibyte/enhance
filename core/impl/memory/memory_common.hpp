#ifndef E2_MEMORY_COMMON_HPP
#define E2_MEMORY_COMMON_HPP

namespace e2 { namespace memory {
	
struct memory_block_info {
    size_t _offset;
    size_t _len;
    void ** _lookup_ptr;
};

} } 

#endif
