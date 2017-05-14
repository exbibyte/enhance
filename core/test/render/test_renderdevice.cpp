#include "renderdevice_gl.hpp"

int main(){

    ::e2::render::renderdevice_gl rd;

    ::e2::render::renderpackage_0 pkg;
    pkg._render_cmd.rendercmd_set( ::e2::interface::e_rendercmd_extract::cmd_type, ::e2::render::e_render_cmd_type::init_resource );
    rd.renderdevice_process( pkg );

    return 0;
}
