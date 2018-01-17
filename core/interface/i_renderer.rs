extern crate gl;

use interface::i_ele;
use interface::i_window::IWindow;
use interface::i_renderobj::RenderDevice;
use interface::i_renderobj;
use interface::i_renderpass;
use interface::i_component;
use interface::i_renderer;

use implement::capability::capability_gl;
use implement::render::util_gl;
use implement::render::shader_collection;
use implement::render::texture_collection;
use implement::render::router;
use implement::render::renderdevice_gl;

pub trait IRenderer {

    type EventRender;

    fn init() -> Result< Self, & 'static str > where Self: Sized;

    fn process_render_events( & mut self, & [ Self::EventRender ] ) -> Result< (), & 'static str >;

    // fn load_shader( & mut self, sources: &[ ( &str, util_gl::ShaderType ) ] ) -> Result< ( u64 ), & 'static str >;

    // fn load_texture( & mut self, description: String, image: &[u8], w: usize, h: usize ) -> Result< ( u64 ), & 'static str >;

    // fn create_draw_group( & mut self, prim_type: i_renderobj::RenderObjType ) -> Result< ( gl::types::GLuint, gl::types::GLuint, usize ), & 'static str >;

    // fn add_obj( & mut self, name: &str, e: i_ele::Ele ) -> Result< ( usize ), & 'static str >;
    
    // fn get_shader_program( & mut self, id: u64 ) -> Option< i64 >;
}

