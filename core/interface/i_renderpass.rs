// use implement::render::rendercommon::PipelineArgRequire;
// use implement::render::rendercommon::Scene;

use interface::i_renderobj;
use interface::i_ele;

use implement::render::renderdevice_gl;

pub enum eRenderPassType {
    DEFAULT,
}

pub trait IRenderPass {
    fn process( & self, pass_type: eRenderPassType, rd: & mut i_renderobj::RenderDevice, objs: & mut Vec< i_ele::Ele >, uniform: & mut renderdevice_gl::RenderUniformCollection, uniform_group: u64 ) -> Result< (), & 'static str >;
}
