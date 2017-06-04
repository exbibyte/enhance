#ifndef E2_I_RENDERPKGEXEC_HPP
#define E2_I_RENDERPKGEXEC_HPP

#include <utility>
#include <functional>
#include <cstdint>
#include <list>

#include "renderdevice_gl.hpp"
#include "i_renderpackage.hpp"

namespace e2 { namespace interface {

class i_renderpkgexec {
public:
    bool process( ::e2::render::renderdevice_gl * rd, std::list< ::e2::interface::i_renderpackage * > * );
};

} }

#endif
