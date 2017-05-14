#include <iostream>

#include "i_renderpackage.hpp"
#include "i_rendercmd.hpp"
#include "renderdevice_gl.hpp"

namespace e2 { namespace render {

renderdevice_gl_impl::renderdevice_gl_impl(){
    //todo
    // _dispatch[ renderdispatch_key( e_render_cmd_type::init_resource,
    // 				   ::e2::interface::e_renderdest_type::window_size,
    // 				   ::e2::interface::e_renderresource_type::float_2 ) ] = &process_init_window_size;
}

bool renderdevice_gl_impl::renderdevice_process( ::e2::interface::i_renderpackage package ){
    // return _dispatch.dispatch_process( package );
    return true;
}

bool renderdevice_gl_impl::process_init_window_size( ::e2::interface::i_renderpackage p ){
    std::cout << "process init window size" << std::endl;
    return true;
}
	
} }
