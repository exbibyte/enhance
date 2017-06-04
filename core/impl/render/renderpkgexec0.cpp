#include <cstdint>
#include <cstring>
#include <list>
#include <utility>

#include "i_rendernode.hpp"
#include "renderpkgexec0.hpp"
#include "renderdevice_gl.hpp"

namespace e2 { namespace render {

bool renderpkgexec0::process( ::e2::interface::i_renderdevice * rd, std::list< ::e2::interface::i_renderpackage * > * packages ){
    assert( rd );
    assert( packages );
    for( auto it = packages->begin(), it_e = packages->end(); it != it_e; ++it ){
	assert( *it );
	rd->renderdevice_process( **it );
    }
    return true;
}

} }
