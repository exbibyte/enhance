use std::ops::{ Deref, DerefMut };
use std::any::Any;
use std::ops::FnMut;
use std::collections::HashMap;

use interface::i_renderobj;

use implement::render::renderdevice_gl;
use implement::math::mat::{Mat3,Mat4};

pub trait IComponent: 'static {
    fn as_any( & self ) -> & Any;
}

pub struct ComponentRenderBuffer {
    pub _data_dict: HashMap< i_renderobj::BuffDataType, Vec<f32> >,
}

impl IComponent for ComponentRenderBuffer {
    fn as_any( & self ) -> & Any {
        self
    }
}

impl ComponentRenderBuffer {
    /// # this dumps the data to render device
    pub fn flush_into_render_device( & self, rd: & mut i_renderobj::RenderDevice ) -> Result< (), & 'static str > {
        println!("flushing into render device" );
        rd.store_buff_data( i_renderobj::RenderObjType::TRI, & self._data_dict );
        Ok( () )
    }
}

pub struct ComponentRenderUniform {
    /// # stores the uniforms values
    pub _data_dict_vf: HashMap< String, Vec<f32> >,
    pub _data_dict_mat4f: HashMap< String, Vec<f32> >,
    pub _data_dict_mat3f: HashMap< String, Vec<f32> >,
    /// # this maps an id to multiple uniforms
    pub _data_uniform_group: HashMap< u64, Vec< String > >,
}

impl Default for ComponentRenderUniform {
    fn default() -> ComponentRenderUniform {
        ComponentRenderUniform {
            _data_dict_vf: HashMap::new(),
            _data_dict_mat4f: HashMap::new(),
            _data_dict_mat3f: HashMap::new(),
            _data_uniform_group: HashMap::new(),
        }
    }
}
impl IComponent for ComponentRenderUniform {
    fn as_any( & self ) -> & Any {
        self
    }
}

impl ComponentRenderUniform {
    /// # this dumps the data to uniform manager
    pub fn flush_into_uniform_collection( & self, shader_program: i64, uc: & mut renderdevice_gl::RenderUniformCollection ) -> Result< (), & 'static str > {
        println!("flushing into uniform collection" );
        for ( ref k, ref v ) in self._data_dict_vf.iter() {
            uc.set_uniform_f( shader_program as _, (*k).as_str(), renderdevice_gl::UniformType::VEC, &v[..] );
        }
        for ( ref k, ref v ) in self._data_dict_mat4f.iter() {
            uc.set_uniform_f( shader_program as _, (*k).as_str(), renderdevice_gl::UniformType::MAT4, &v[..] );
        }
        for ( ref k, ref v ) in self._data_dict_mat3f.iter() {
            uc.set_uniform_f( shader_program as _, (*k).as_str(), renderdevice_gl::UniformType::MAT3, &v[..] );
        }
        
        for ( ref k, ref v ) in self._data_uniform_group.iter() {
            println!("uniform group: {}, length: {}.", **k, (**v).len() );
            uc.set_group( shader_program as _, **k, (**v).clone() ).is_ok();            
        }

        Ok( () )
    }
}
