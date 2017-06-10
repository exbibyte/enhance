#include <chrono>
#include <thread>
#include <iostream>
#include <cassert>
#include <cstdint>

#include "i_trait_general.hpp"
#include "renderfrontend0.hpp"
#include "renderbackend0.hpp"

struct something {};

int main(){

    static_assert( ::e2::trait::trait_is_renderfrontend < ::e2::render::renderfrontend0 >::_val, "renderfrontend0 is not a renderfrontend" );
    static_assert( false == ::e2::trait::trait_is_renderfrontend < something >::_val, "something is not a renderfrontend" );
    static_assert( ::e2::trait::trait_is_renderbackend < ::e2::render::renderbackend0 >::_val, "renderbackend0 is not a renderbackend" );

    ::e2::render::renderfrontend0 rf;
    assert( ::e2::trait::instance_has_trait< ::e2::trait::trait_is_renderfrontend > ( &rf ) );

    ::e2::render::renderbackend0 rb;
    assert( false == ::e2::trait::instance_has_trait< ::e2::trait::trait_is_renderfrontend > ( &rb ) );
    
    return 0;
}
