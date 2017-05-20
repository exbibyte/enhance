#ifndef E2_I_RENDER_CMD_HPP
#define E2_I_RENDER_CMD_HPP

#include <cstdint>

namespace e2 { namespace interface {
	
enum e_rendercmd_type : uint64_t {
    query = 1,
    compute,
    store,
    load,
    init,
    deinit,
    point,
    exec,
};


} }

#endif
