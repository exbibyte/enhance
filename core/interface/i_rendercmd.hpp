#ifndef E2_I_RENDER_CMD_HPP
#define E2_I_RENDER_CMD_HPP

#include <cstdint>

namespace e2 { namespace interface {
	
enum e_rendercmd_type : uint64_t {
    e_rendercmd_type_query = 1,
    e_rendercmd_type_compute,
    e_rendercmd_type_store,
    e_rendercmd_type_load,
    e_rendercmd_type_init,
    e_rendercmd_type_deinit,
    e_rendercmd_type_exec,
    e_rendercmd_type_enable,
    e_rendercmd_type_disable,
    e_rendercmd_type_bind,
    e_rendercmd_type_unbind,
};


} }

#endif
