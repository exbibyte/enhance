#ifndef E2_RENDERFRONTEND0_HPP
#define E2_RENDERFRONTEND0_HPP

#include <cstdint>
#include <cstring>
#include <utility>

#include "i_trait_general.hpp"
#include "i_renderfrontend.hpp"

namespace e2 { namespace render {

class renderfrontend0_impl {
public:
    renderfrontend0_impl(){}
    bool renderfrontend_process( ::e2::interface::e_renderfrontend_action a, void * v ){ return false; }
};

class renderfrontend0 : public ::e2::interface::i_renderfrontend< renderfrontend0_impl > {};

} }

//trait information

namespace e2 { namespace trait {

template<>
struct trait_is_renderfrontend< ::e2::render::renderfrontend0 > {
    constexpr static bool _val = true;
};

} }
	
#endif
