#ifndef E2_RENDERBACKEND0_HPP
#define E2_RENDERBACKEND0_HPP

#include <cstdint>
#include <cstring>
#include <utility>
#include <list>
#include <cassert>
#include <iostream>

#include "i_rendernode.hpp"
#include "i_trait_general.hpp"
#include "i_renderbackend.hpp"

#include "renderdevice_gl.hpp"
#include "rendergraphscheduler0.hpp"
#include "memory_manager_p1t_g1_ff.hpp"

namespace e2 { namespace render {

template< class mem_manager >
class renderbackend0_impl {
public:
    renderbackend0_impl();
    bool renderbackend_process_rendernodes( std::list< ::e2::interface::i_rendernode * > * nodes  );
    bool renderbackend_process_commit();
    bool renderbackend_process_batches( int count );
    bool renderbackend_get_window( void ** win );
    ::e2::render::renderdevice_gl _rd;
    ::e2::render::rendergraphscheduler0 _sch;
    std::list< ::e2::interface::i_rendernode * > _scheduled;
    std::list< std::list< ::e2::interface::i_rendernode * > > _committed;
};

#include "renderbackend0.tpp"

template< class mem_manager >
class renderbackend0 : public ::e2::interface::i_renderbackend< renderbackend0_impl< mem_manager > > {};

} }

//trait information

namespace e2 { namespace trait {

// template<>
// struct trait_is_renderbackend< ::e2::render::renderbackend0 > {
//     constexpr static bool _val = true;
// };

template<>
struct trait_is_renderbackend< ::e2::render::renderbackend0< ::e2::memory::memory_manager_p1t_g1_ff > > {
    constexpr static bool _val = true;
};

} }
	
#endif

