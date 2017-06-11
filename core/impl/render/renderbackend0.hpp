#ifndef E2_RENDERBACKEND0_HPP
#define E2_RENDERBACKEND0_HPP

#include <cstdint>
#include <cstring>
#include <utility>

#include "i_trait_general.hpp"
#include "i_renderbackend.hpp"
#include "i_renderpackage.hpp"

#include "rendertaskpackager_gl.hpp"
#include "renderdevice_gl.hpp"
#include "rendergraphconverter_gl.hpp"
#include "renderpkgexec0.hpp"
#include "rendergraphscheduler0.hpp"
#include "buffer.hpp"

namespace e2 { namespace render {

class renderbackend0_impl {
public:
    renderbackend0_impl();
    bool renderbackend_process_rendernodes( std::list< ::e2::interface::i_rendernode * > * nodes  );
    bool renderbackend_process_commit();
    bool renderbackend_process_renderpackages( int count );
    bool renderbackend_get_window( void ** win );
    ::e2::render::renderpkgexec0 _executer;
    ::e2::render::renderdevice_gl _rd;
    ::e2::render::rendergraphconverter_gl _converter;
    ::e2::render::rendertaskpackager_gl _packager;
    ::e2::render::rendergraphscheduler0 _sch;
    ::e2::memory::buffer _buf;
    std::list< ::e2::interface::i_renderpackage * > _renderpackages;
    std::list< std::list< ::e2::interface::i_renderpackage * > > _renderpackages_committed;
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

