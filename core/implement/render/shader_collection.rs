use std::collections::HashMap;
use std::vec::Vec;
use std::string::String;

extern crate gl;

use implement::render::router;

pub struct ShaderCollection {
    _programs: HashMap< u64, ( router::ShaderType, i64 ) >,
    _id_to_descrip: HashMap< u64, String >,
    _descrip_to_id: HashMap< String, u64 >,
}


impl Default for ShaderCollection {
    fn default() -> ShaderCollection {
        ShaderCollection {
            _programs: HashMap::new(),
            _id_to_descrip: HashMap::new(),
            _descrip_to_id: HashMap::new(),
        }
    }
}
    
impl ShaderCollection {
    pub fn put( & mut self, id: u64, shader_type: router::ShaderType, internal_handle: i64, descrip: String ) -> Result< (), & 'static str > {
        match self._programs.insert( id, ( shader_type, internal_handle ) ) {
            None => (),
            Some( ( shader_type, old_handle ) ) => {
                router::delete_shader_program( old_handle, shader_type );
                println!( "removed old shader program( {} ).", old_handle );
            }
        }
        self._id_to_descrip.insert( id, descrip.clone() );
        self._descrip_to_id.insert( descrip, id );
        Ok( () )
    }
    pub fn clear( & mut self ) -> Result< (), & 'static str > {
        for ( &k, &( ref shader_type, ref handle ) ) in self._programs.iter() {
            router::delete_shader_program( *handle, (*shader_type).clone() );
        }
        self._id_to_descrip.clear();
        self._descrip_to_id.clear();
        Ok( () )
    }
    pub fn remove( & mut self, id: u64 ) -> Result< (), & 'static str > {
        match self._programs.remove( &id ) {
            Some( ( shader_type, handle ) ) => {
                router::delete_shader_program( handle, shader_type );
                if let Some( descrip ) = self._id_to_descrip.remove( &id ) {
                    self._descrip_to_id.remove( &descrip );
                }
            },
            None => (),
        }
        Ok( () )
    }
    pub fn get( & mut self, id: u64 ) -> Option< i64 > {
        match self._programs.get( &id ) {
            Some( &( ref shader_type, ref handle ) ) => {
                return Some( *handle )
            },
            None => return None
        }
    }
}
