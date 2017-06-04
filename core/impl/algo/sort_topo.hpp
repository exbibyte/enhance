#ifndef E2_SORT_TOPO_HPP
#define E2_SORT_TOPO_HPP

#include <cstring>
#include <cstdint>
#include <set>
#include <list>
#include <utility>

#include "i_sort.hpp"

namespace e2 { namespace algo {

class sort_topo_impl {
public:
    class node_topo {
    public:
	uint64_t _handle;
	std::set< uint64_t > _in;
	std::set< uint64_t > _out;
    };
    bool sort( std::list< std::pair< uint64_t, uint64_t > > * input, std::list< uint64_t > * output );
};

class sort_topo : public ::e2::interface::i_sort< sort_topo_impl > {};

} }

#endif
