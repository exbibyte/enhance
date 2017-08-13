use std::collections::HashMap;

use interface::i_renderpass;
use implement::kernel::kernel_render;

pub struct RenderPassCollection {
    pub _rp: HashMap< i_renderpass::eRenderPassType, Box< i_renderpass::IRenderPass > >,
}

pub fn process_render_pass( pass_type: i_renderpass::eRenderPassType, passes: & mut RenderPassCollection, renderer: & mut kernel_render::Renderer ) -> Result< (), & 'static str > {
    match passes._rp.get( & pass_type ) {
        Some( pass ) => { pass.process( renderer ) },
        None => Err( "pass type not found" )
    }
}
