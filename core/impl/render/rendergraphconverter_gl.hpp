#ifndef E2_RENDERGRAPHCONVERTER_GL_HPP
#define E2_RENDERGRAPHCONVERTER_GL_HPP

#include <cstdint>
#include <cstring>
#include <list>
#include <utility>

#include "i_rendernode.hpp"
#include "i_rendergraphconverter.hpp"
#include "buffer.hpp"

namespace e2 { namespace render {

class rendergraphconverter_gl : public ::e2::interface::i_rendergraphconverter {
public:
    bool process( ::e2::memory::buffer * buf, std::list< ::e2::interface::i_rendernode * > * input, std::list< ::e2::interface::i_renderpackage * > * output ) override;
};

} }
	
#endif
