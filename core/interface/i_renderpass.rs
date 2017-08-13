use interface::i_renderobj;
use interface::i_ele;

use implement::render::renderdevice_gl;
use implement::kernel::kernel_render::Renderer;

#[derive(PartialEq)]
#[derive(Eq)]
#[derive(Hash)]
pub enum eRenderPassType {
    DEFAULT,
}

pub trait IRenderPass {
    fn process( & self, renderer: & mut Renderer ) -> Result< (), & 'static str >;
}
