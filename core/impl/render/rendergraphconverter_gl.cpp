#include <cstdint>
#include <cstring>
#include <list>
#include <utility>

#include "i_renderpackage.hpp"
#include "i_rendernode.hpp"
#include "rendergraphconverter_gl.hpp"
#include "rendertaskpackager_gl.hpp"
#include "buffer.hpp"

namespace e2 { namespace render {

bool rendergraphconverter_gl::process( ::e2::memory::buffer * buf, std::list< ::e2::interface::i_rendernode * > * input, std::list< ::e2::interface::i_renderpackage * > * output ){
    ::e2::render::rendertaskpackager_gl packager;
    for( auto it = input->begin(), it_e = input->end(); it != it_e; ++it ){
	assert( *it );
	::e2::interface::i_renderpackage * pkg;
	assert( packager.process( buf, &pkg, *it ) );
	output->push_back( pkg );
    }
    return true;
}

} }
