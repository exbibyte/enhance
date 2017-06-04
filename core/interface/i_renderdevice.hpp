#ifndef E2_I_RENDERDEVICE_HPP
#define E2_I_RENDERDEVICE_HPP

#include <utility>
#include <functional>
#include <cstdint>

namespace e2 { namespace interface {

class i_renderdevice {
public:
    virtual bool renderdevice_process( ::e2::interface::i_renderpackage p ) = 0;
};

} }

#endif
