#ifndef E2_I_RENDERGRAPHCONVERTER_HPP
#define E2_I_RENDERGRAPHCONVERTER_HPP

#include <cstdint>
#include <cstring>
#include <list>
#include <utility>

#include "i_rendernode.hpp"
#include "i_renderpackage.hpp"

namespace e2 { namespace interface {

class i_rendergraphconverter {
public:
    virtual bool process( ::e2::memory::buffer * buf, std::list< ::e2::interface::i_rendernode * > * input, std::list< ::e2::interface::i_renderpackage * > * output ) = 0;
};

} }
	
#endif
