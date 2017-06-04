#include <cstdint>
#include <cstring>
#include <list>
#include <utility>
#include <set>
#include <cassert>

#include "rendergraphbuilder0.hpp"
#include "i_rendernode.hpp"

namespace e2 { namespace render {

bool rendergraphbuilder0::link_prerequisite( ::e2::interface::i_rendernode * n, ::e2::interface::i_rendernode * n_prereq ){
    assert( n );
    assert( n_prereq );
    n->_inputs.push_back( n_prereq );
    n_prereq->_outputs.push_back( n );
    return true;
}
bool rendergraphbuilder0::set_handle( ::e2::interface::i_rendernode * node ){
    node->_handle = _gen_handle++;
    return true;
}

} }
