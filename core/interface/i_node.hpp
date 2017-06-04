#ifndef E2_I_NODE_INOUT_HPP
#define E2_I_NODE_INOUT_HPP

#include <thread>
#include <functional>
#include <type_traits>
#include <list>

namespace e2 { namespace interface {

template< class Impl >
class i_node_inout : public Impl {
public:
    std::list< i_node_inout * > _in;
    std::list< i_node_inout * > _out;
};

} }

#endif
