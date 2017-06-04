#ifndef E2_RENDERGRAPHSCHEDULER0_HPP
#define E2_RENDERGRAPHSCHEDULER0_HPP

#include <cstdint>
#include <cstring>
#include <list>
#include <utility>

#include "i_rendergraphscheduler.hpp"
#include "i_rendernode.hpp"

namespace e2 { namespace render {

class rendergraphscheduler0_impl {
public:
    bool schedule_graph( std::list< ::e2::interface::i_rendernode * > * input, std::list< ::e2::interface::i_rendernode * > * output );
};

class rendergraphscheduler0 final : public ::e2::interface::i_rendergraphscheduler< rendergraphscheduler0_impl > {};

} }
	
#endif
