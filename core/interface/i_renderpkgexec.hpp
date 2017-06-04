#ifndef E2_I_RENDERPKGEXEC_HPP
#define E2_I_RENDERPKGEXEC_HPP

#include <cstdint>
#include <cstring>
#include <list>
#include <utility>

#include "i_rendernode.hpp"
#include "i_renderpackage.hpp"
#include "i_renderdevice.hpp"

namespace e2 { namespace interface {

class i_renderpkgexec {
public:
    virtual bool process( ::e2::interface::i_renderdevice * rd, std::list< ::e2::interface::i_renderpackage * > * packages ) = 0;
};

} }
	
#endif
