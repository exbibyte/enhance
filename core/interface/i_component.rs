use std::ops::{ Deref, DerefMut };
use std::any::Any;
use std::ops::FnMut;
use std::collections::HashMap;

use interface::i_renderobj;

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
    fn flush_into_render_device( components: & mut Vec< Box< IComponent > >, rd: & mut i_renderobj::RenderDevice ) -> Result< (), & 'static str > {
        for i in components.iter() {
            //downcasting: https://stackoverflow.com/questions/33687447/how-to-get-a-struct-reference-from-a-boxed-trait
            let current_component : &ComponentRenderBuffer = match i.as_any().downcast_ref::< ComponentRenderBuffer >() {
                Some( o ) => o,
                None => { continue; }
            };
            rd.store_buff_data( i_renderobj::RenderObjType::TRI, &current_component._data_dict );            
        }
        Ok( () )
    }
}




