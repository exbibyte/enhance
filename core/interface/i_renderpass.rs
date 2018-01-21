use implement::render::renderer_gl::Renderer;

#[derive(PartialEq)]
#[derive(Eq)]
#[derive(Hash)]
pub enum RenderPassType {
    DEFAULT,
}

pub trait IRenderPass {
    fn process( & self, renderer: & mut Renderer ) -> Result< (), & 'static str >;
}
