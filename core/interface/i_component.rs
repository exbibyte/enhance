use std::ops::{ Deref, DerefMut };
use std::any::Any;
use std::ops::FnMut;
use std::collections::HashMap;

use interface::i_renderobj;
use implement::render::renderdevice_gl;

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

pub struct ComponentRenderUniform {}

impl IComponent for ComponentRenderUniform {
    fn as_any( & self ) -> & Any {
        self
    }
}

impl ComponentRenderUniform {
    /// # this dumps the data to render device
    pub fn flush_into_uniform_collection( & self, rd: & mut renderdevice_gl::RenderUniformCollection ) -> Result< (), & 'static str > {
        println!("flushing into uniform collection" );
        //todo
        unimplemented!();
    }
}
