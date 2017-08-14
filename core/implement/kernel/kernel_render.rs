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

use self::glutin::GlContext;

use interface::i_ele;
use interface::i_window::IWindow;
use interface::i_renderobj::IRenderBuffer;
use interface::i_renderobj::RenderDevice;
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
use implement::render::router;
use implement::render::mesh;
use implement::render::renderdevice_gl;
use implement::render::primitive;
// use implement::render::renderpass_default;

pub struct Renderer {
    _win: WinGlutin,
    _rp: Vec< Box< i_renderpass::IRenderPass > >,
    _map_string_to_rp: HashMap< String, usize >,
    _shader_collection: shader_collection::ShaderCollection,
    _shader_programs: Vec< u64 >,
    _draw_groups: Vec< renderdevice_gl::RenderDrawGroup >,
    _vaos: Vec< gl::types::GLuint >,
    _vbos: Vec< gl::types::GLuint >,
    _objs: Vec< Box< i_ele::Ele > >,
    _map_string_to_objs: HashMap< String, usize >,
    _uniforms: renderdevice_gl::RenderUniformCollection,
    _draw_group_uniforms: Vec< Vec< u64 > >,
    _shaders_compiled: Vec< gl::types::GLuint >,
}

impl Drop for Renderer {
    fn drop( & mut self ) {
        unsafe {
            gl::DeleteBuffers( self._vaos.len() as _, self._vaos.as_ptr(), );

            gl::DeleteBuffers( self._vbos.len() as _, self._vbos.as_ptr(), );
        }
        self._shader_collection.clear(); //does DeleteProgram
        unsafe {
            for i in self._shaders_compiled.iter() {
                gl::DeleteShader( *i );
            }
        }
    }
}

impl Renderer {
    pub fn init() -> Result< Renderer, & 'static str > {
        let mut rk = Renderer {
            _win: IWindow::init( 500, 500 ),
            _rp: vec![],
            _map_string_to_rp: HashMap::new(),
            _shader_collection: Default::default(),
            _shader_programs: vec![],
            _draw_groups: vec![],
            _vaos: vec![],
            _vbos: vec![],
            _objs: vec![],
            _map_string_to_objs: HashMap::new(),
            _uniforms: Default::default(),
            _draw_group_uniforms: vec![],
            _shaders_compiled: vec![],
        };
        match rk._win.make_current() {
            Err( e ) => return Err( e ),
            _ => (),
        }
        let cap = capability_gl::query_gl();
        println!( "{}", cap );
        Ok( rk )
    }
    pub fn load_shader( & mut self, sources: &[ ( &str, util_gl::ShaderType ) ] ) -> Result< ( u64 ), & 'static str > {
        let mut compiled_shaders = vec![];
        for &(ref src, ref src_type ) in sources.into_iter() {
            let s = match util_gl::load_and_compile_shader( src, *src_type ) {
                Ok( o ) => o,
                Err( o ) => {
                    println!( "{}", o );
                    return Err( "error loading shader" )
                }
            };
            unsafe {
                util_gl::check_last_op();
            }
            compiled_shaders.push( s );
        }
        {
            let i = self._shader_programs.len();
            {
                self._shader_collection.put( i as u64, router::ShaderType::GLSL, util_gl::create_program_from_shaders( compiled_shaders.as_slice() ) as _, String::from("ads_program") ).is_ok();
            }
            let shader_program = self._shader_collection.get( i as u64 ).unwrap();
            unsafe {
                gl::UseProgram( shader_program as _ );
                gl::BindFramebuffer( gl::FRAMEBUFFER, 0 );
                gl::Enable( gl::DEPTH_TEST );
            }
            self._shader_programs.push( shader_program as u64 );
            {
                self._shaders_compiled.append( & mut compiled_shaders );
            }
            Ok( ( shader_program as u64 ) )
        }
    }
    pub fn create_draw_group( & mut self ) -> Result< ( gl::types::GLuint, gl::types::GLuint, usize ), & 'static str > {
        let mut obj_vao = 0;
        let mut obj_vbo = 0;
        unsafe {
            gl::GenVertexArrays( 1, & mut obj_vao );
            util_gl::check_last_op();
            gl::GenBuffers( 1, & mut obj_vbo );
            util_gl::check_last_op();
        }
        let mut draw_group = renderdevice_gl::RenderDrawGroup::init_with_default_format( obj_vao as _, obj_vbo as _ );
        self._draw_groups.push( draw_group );
        Ok( ( obj_vao, obj_vbo, self._draw_groups.len() - 1) )
    }
    pub fn add_obj( & mut self, name: &str, e: i_ele::Ele ) -> usize {
        let index = self._objs.len();
        self._objs.push( Box::new( e ) );
        self._map_string_to_objs.insert( String::from(name), index );
        index
    }
    pub fn get_obj( & mut self, name: &str ) -> Option< ( usize, &i_ele::Ele ) > {
        match self._map_string_to_objs.get( & String::from( name ) ){
            None => return None,
            Some( index ) => {
                let n = &self._objs[ *index ];
                return Some( ( *index, n ) )
            }
        }
    }
    pub fn set_obj( & mut self, name: &str, e: i_ele::Ele ) -> Option< usize > {
        match self._map_string_to_objs.get( & String::from( name ) ){
            None => return None,
            Some( index ) => {
                self._objs[ *index ] = Box::new( e );
                Some( *index )
            }
        }
    }
    pub fn load_objs_to_draw_group( & mut self, obj_indices: &[ usize], target_draw_group: usize ) -> Result< (), & 'static str > {
        let group_index = target_draw_group;
        if group_index >= self._draw_groups.len() {
            return Err( "target draw group out of range" )
        }
        for &i in obj_indices {
            if i >= self._objs.len() {
                return Err( "object index out o
f range" )
            }
            //compute component data
            match self._objs[i].update_components_from_impl() {
                Err( e ) => return Err( e ),
                _ => (),
            }
            //dispatch component specialization
            match i_component::ComponentRenderBuffer::flush_into_render_device( & mut self._objs[i]._components, & mut self._draw_groups[ group_index ] ) {
                Err( e ) => return Err( e ),
                _ => (),
            }
        }
        Ok( () )
    }
    pub fn reset_draw_group_data( & mut self, group_indices: &[ usize ] ) -> Result< (), & 'static str > {
        for &i in group_indices {
            if i >= self._draw_groups.len() {
                return Err( "object index out of range" )
            }
            self._draw_groups[ i ].clear_buff_data();
        }
        Ok( () )
    }
    pub fn bind_draw_group_data( & mut self, group_indices: &[ usize ] ) -> Result< (), & 'static str > {
        for &i in group_indices {
            if i >= self._draw_groups.len() {
                return Err( "object index out of range" )
            }
            match self._draw_groups[ i ].bind_buffer() {
                Err( e ) => return Err( e ),
                _ => (),
            }
        }
        Ok( () )
    }
    pub fn drawcall_draw_group( & mut self, group_indices: &[ usize ] ) -> Result< (), & 'static str > {
        for &i in group_indices {
            for uniform_group in & self._draw_group_uniforms[i] {
                if i >= self._draw_groups.len() {
                    return Err( "object index out of range" )
                }
                match self._uniforms.send_uniform_group( *uniform_group ){
                    Err(e) => return Err(e),
                    _ => ()
                }
                match self._draw_groups[ i ].draw_buffer_all() {
                    Err( e ) => return Err( e ),
                    _ => (),
                }
            }
        }
        Ok( () )
    }
    pub fn add_renderpass< T > ( & mut self, name: String, rp: T ) -> usize
    where T: i_renderpass::IRenderPass + 'static {
        let i = self._rp.len();
        self._map_string_to_rp.insert( name, i );
        self._rp.push( Box::new( rp ) );
        i
    }
    pub fn get_renderpass( & mut self, name: String ) -> Option< & mut i_renderpass::IRenderPass > {
        match self._map_string_to_rp.get( &name ){
            None => None,
            Some( o ) => {
                Some( self._rp[*o].deref_mut() )
            }
        }
    }
    pub fn uniforms_ref( & mut self ) -> & mut renderdevice_gl::RenderUniformCollection {
        & mut self._uniforms
    }
    pub fn add_draw_group_uniforms( & mut self, draw_group: usize, uniform_group: &[u64] ) -> Result< (), & 'static str > {
        if self._draw_group_uniforms.len() <= draw_group {
            self._draw_group_uniforms.resize( draw_group + 1, vec![] );
        }
        self._draw_group_uniforms[ draw_group ].extend_from_slice( uniform_group );
        Ok( () )
    }
    pub fn clear_draw_group_uniforms( & mut self, draw_group: usize ) -> Result< (), & 'static str > {
        if self._draw_group_uniforms.len() <= draw_group {
            return Err( "draw group out of range" )
        }
        self._draw_group_uniforms[ draw_group ].clear();
        Ok( () )
    }
    pub fn win_ref( & mut self ) -> & mut WinGlutin {
        & mut self._win
    }
}

