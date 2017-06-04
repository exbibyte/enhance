#ifndef E2_I_RENDERGRAPHBUILDER_HPP
#define E2_I_RENDERGRAPHBUILDER_HPP

#include <cstdint>
#include <cstring>
#include <list>
#include <utility>

#include "i_rendernode.hpp"

namespace e2 { namespace interface {

class i_rendergraphbuilder {
public:
    // virtual bool process( std::list< ::e2::interface::i_rendernode * > * nodes ) = 0;
    virtual bool link_prerequisite( ::e2::interface::i_rendernode * n, ::e2::interface::i_rendernode * n_prereq ) = 0;
    virtual bool set_handle( ::e2::interface::i_rendernode * node ) = 0;
};

} }
	
#endif
