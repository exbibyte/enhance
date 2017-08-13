use std::borrow::BorrowMut;
use std::ops::{ Deref, DerefMut };

extern crate gl;
extern crate glutin;
extern crate libc;

use interface::i_renderpass;
use interface::i_ele;
use interface::i_renderobj;

use implement::kernel::kernel_render::Renderer;
use implement::render::renderdevice_gl;

pub struct RenderPassDefault {}

impl RenderPassDefault {
    pub fn process_default( & self, rd: & mut i_renderobj::RenderDevice, objs: & mut Vec< i_ele::Ele >, uniform: & mut renderdevice_gl::RenderUniformCollection, uniform_group: u64 ) -> Result< (), & 'static str > {
        for i in objs.iter_mut() {
            ( (**i).borrow_mut() as & mut i_ele::Facility ).load_into_buffer( rd ).is_ok();
        }
        rd.bind_buffer().is_ok();
        match uniform.send_uniform_group( uniform_group ){
            Err(e) => return Err(e),
            _ => ()
        }
        rd.draw_buffer_all();
        Ok( () )
    }
}

impl i_renderpass::IRenderPass for RenderPassDefault {
    fn process( & self, renderer: & mut Renderer ) -> Result< (), & 'static str > {
        unimplemented!();
    }    
}
