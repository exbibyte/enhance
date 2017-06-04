#ifndef E2_RENDERGRAPHBUILDER0_HPP
#define E2_RENDERGRAPHBUILDER0_HPP

#include <cstdint>
#include <cstring>
#include <list>
#include <utility>

#include "i_rendergraphbuilder.hpp"
#include "i_rendernode.hpp"

namespace e2 { namespace render {

class rendergraphbuilder0 : public ::e2::interface::i_rendergraphbuilder {
public:
    rendergraphbuilder0() : _gen_handle(0) {}
    bool set_handle( ::e2::interface::i_rendernode * node ) override;
    bool link_prerequisite( ::e2::interface::i_rendernode * n, ::e2::interface::i_rendernode * n_prereq ) override;
    uint64_t _gen_handle;
};

} }
	
#endif
