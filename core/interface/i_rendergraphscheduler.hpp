#ifndef E2_I_RENDERGRAPHSCHEDULER_HPP
#define E2_I_RENDERGRAPHSCHEDULER_HPP

#include <cstdint>
#include <cstring>
#include <list>
#include <utility>

#include "i_rendernode.hpp"

namespace e2 { namespace interface {

template< class Impl >
class i_rendergraphscheduler : public Impl {
public:
    bool schedule_graph( std::list< ::e2::interface::i_rendernode * > * input, std::list< ::e2::interface::i_rendernode * > * output ){ return Impl::schedule_graph( input, output ); }
};

} }
	
#endif
