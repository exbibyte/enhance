extern crate gl;
extern crate glutin;
extern crate libc;
extern crate rand;

use ::std::mem;
use ::std::fs::File;
use ::std::io::BufReader;
use ::std::str::FromStr;
use ::std::io::Read;
use ::std::ffi::CStr;
use ::std::os::raw::c_char;
use ::std::fmt;
use ::std::str;
use self::rand::Rng;
use ::std::any::Any;
use ::std::borrow::BorrowMut;
use ::std::ops::{ Deref, DerefMut };
use std::collections::HashMap;
use std::cell::RefCell;
    
use self::glutin::GlContext;

use interface::i_ele;
use interface::i_window::IWindow;
use interface::i_renderobj::IRenderBuffer;
use interface::i_renderobj::RenderDevice;
use interface::i_renderobj;
use interface::i_renderpass::IRenderPass;
use interface::i_renderpass;
use interface::i_component;

use implement::window::winglutin::WinGlutin;
use implement::capability::capability_gl;
use implement::render::util_gl;
use implement::math;
use implement::render::camera;
use implement::render::light;
use implement::render::shader_collection;
use implement::render::texture_collection;
use implement::render::router;
use implement::render::mesh;
use implement::render::renderdevice_gl;
use implement::render::primitive;

pub struct Engine {

}

impl Engine {
    pub fn init() -> Result< Engine, & 'static str > {
        
    }
    pub fn add_obj( & mut self, name: &str, e: i_ele::Ele ) -> Result< ( usize ), & 'static str > {

        let index = renderer._objs.borrow_mut().len();
        renderer._objs.borrow_mut().push( Box::new( e ) );

        //load component data
        match renderer._objs.borrow_mut()[index].update_components_from_impl() {
            Err( e ) => { return Err( e ) },
            _ => (),
        }

        //detect command to flush and process all data in buffer
        let mut trigger_to_process_objs = false;
        let mut group_id = 0;
        for j in renderer._objs.borrow_mut()[index]._components.iter() {
            match j.as_any().downcast_ref::< i_component::ComponentDrawGroupDispatch >() {
                Some( o ) => {
                    println!("detected trigger for draw group dispatch");
                    trigger_to_process_objs = true;
                    group_id = o._group_id;
                },
                None => {},
            }
        }

        if trigger_to_process_objs {
            Renderer::process_objs( renderer, group_id );
        }

        Ok( renderer._objs.borrow_mut().len() )  
    }
    pub fn process_objs( renderer: & mut Renderer, group_index: usize ) -> Result< (), & 'static str > {
        let mut index = 0;
        
        println!("objects size: {}", renderer._objs.borrow_mut().len() );
        
        for i in renderer._objs.borrow_mut().iter() {

            for j in i._components.iter() {

                //downcasting: https://stackoverflow.com/questions/33687447/how-to-get-a-struct-reference-from-a-boxed-trait
                match j.as_any().downcast_ref::< i_component::ComponentRenderBuffer >() {
                    Some( o ) => {
                        println!("buffer flushed");
                        match o.flush_into_render_device( & mut renderer._draw_groups.get_mut()[ group_index ] ) {
                            Err( e ) => return Err( e ),
                            _ => { continue; },
                        }
                        ()
                    },
                    None => (),
                }
                match j.as_any().downcast_ref::< i_component::ComponentRenderUniform >() {
                    Some( o ) => {
                        println!("uniform flushed");
                        let shader_program = renderer._shader_collection.borrow_mut().get( renderer._current_shader_program ).unwrap();
                        match o.flush_into_uniform_collection( shader_program, & mut renderer._uniforms.borrow_mut() ) {
                            Err( e ) => return Err( e ),
                            _ => { continue; },
                        }
                        ()
                    },
                    None => (),
                }
                match j.as_any().downcast_ref::< i_component::ComponentDrawGroupClear >() {
                    Some( o ) => {
                        println!("draw group clear");
                        renderer.reset_draw_group_data( &[ o._group_id ] ).is_ok();
                        continue;
                    },
                    None => (),
                }
                match j.as_any().downcast_ref::< i_component::ComponentDrawGroupBind >() {
                    Some( o ) => {
                        println!("draw group bind");
                        renderer.bind_draw_group_data( &[ o._group_id ] ).is_ok();
                        continue;
                    },
                    None => (),
                }
                match j.as_any().downcast_ref::< i_component::ComponentDrawGroupDependentUniforms >() {
                    Some( o ) => {
                        println!("draw group dependent uniforms");
                        renderer.set_draw_group_uniforms( o._group_id, &o._uniform_ids[..] ).is_ok();
                        continue;
                    },
                    None => (),
                }
                match j.as_any().downcast_ref::< i_component::ComponentDrawGroupDispatch >() {
                    Some( o ) => {
                        println!("draw group dispatch");
                        let renderer_immut : & Renderer = & * renderer;
                        Renderer::drawcall_draw_group( renderer_immut, &[ o._group_id ] ).is_ok();
                        continue;
                    },
                    None => (),
                }
                return Err( &"unmatching render command" )
            }
        }
        renderer._objs.borrow_mut().clear();
        Ok( () )
    }
}

