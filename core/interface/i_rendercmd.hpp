#ifndef E2_I_RENDER_CMD_HPP
#define E2_I_RENDER_CMD_HPP

namespace e2 { namespace interface {
	
enum e_rendercmd_type {
    query = 1,
    compute,
    store,
    load,
    init,
    deinit,
    point,
};

} }

#endif
