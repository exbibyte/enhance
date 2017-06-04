#ifndef E2_RENDERPKGEXEC0_HPP
#define E2_RENDERPKGEXEC0_HPP

#include <cstdint>
#include <cstring>
#include <list>
#include <utility>

#include "i_rendernode.hpp"
#include "i_renderpkgexec.hpp"
#include "renderdevice_gl.hpp"
#include "i_renderpackage.hpp"

namespace e2 { namespace render {

class renderpkgexec0 : public ::e2::interface::i_renderpkgexec {
public:
    bool process( ::e2::interface::i_renderdevice * rd, std::list< ::e2::interface::i_renderpackage * > * packages ) override;
};

} }
	
#endif
