#ifndef E2_RENDERBACKEND0_HPP
#define E2_RENDERBACKEND0_HPP

#include <cstdint>
#include <cstring>
#include <utility>

#include "i_trait_general.hpp"
#include "i_renderbackend.hpp"

namespace e2 { namespace render {

class renderbackend0_impl {
public:
    renderbackend0_impl(){}
    bool renderbackend_process( ::e2::interface::e_renderbackend_action a, void * v ){ return false; }
};

class renderbackend0 : public ::e2::interface::i_renderbackend< renderbackend0_impl > {};

} }

//trait information

namespace e2 { namespace trait {

template<>
struct trait_is_renderbackend< ::e2::render::renderbackend0 > {
    constexpr static bool _val = true;
};

} }
	
#endif

