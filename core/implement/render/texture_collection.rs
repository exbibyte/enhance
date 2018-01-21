use std::collections::HashMap;
use std::string::String;


use implement::render::router;

pub struct TextureCollection {
    _textures: HashMap< u64, ( router::ShaderType, i64 ) >,
    _id_to_descrip: HashMap< u64, String >,
    _descrip_to_id: HashMap< String, u64 >,
    _id_generator: u64,
}


impl Default for TextureCollection {
    fn default() -> TextureCollection {
        TextureCollection {
            _textures: HashMap::new(),
            _id_to_descrip: HashMap::new(),
            _descrip_to_id: HashMap::new(),
            _id_generator: 0u64,
        }
    }
}

impl Drop for TextureCollection {
    fn drop( & mut self ) {
        self._textures.clear();
    }
}

impl TextureCollection {
    pub fn add( & mut self, shader_type: router::ShaderType, internal_handle: i64, descrip: String ) -> Result< (u64), & 'static str > {
        let mut handle = self._id_generator;
        while self._textures.contains_key( &handle ) {
            handle = handle + 1;
        }
        match self._textures.insert( handle, ( shader_type, internal_handle ) ) {
            None => (),
            Some( ( _shader_type, _old_handle ) ) => {
                return Err( &"adding texture handle failed")
            }
        }
        self._id_generator = handle;
        self._id_to_descrip.insert( handle, descrip.clone() );
        self._descrip_to_id.insert( descrip, handle );
        Ok( handle )
    }
    pub fn put( & mut self, id: u64, shader_type: router::ShaderType, internal_handle: i64, descrip: String ) -> Result< (), & 'static str > {
        match self._textures.insert( id, ( shader_type, internal_handle ) ) {
            None => (),
            Some( ( shader_type, old_handle ) ) => {
                router::delete_texture( old_handle, shader_type )?;
                trace!( "removed old texture( {} ).", old_handle );
            }
        }
        self._id_to_descrip.insert( id, descrip.clone() );
        self._descrip_to_id.insert( descrip, id );
        Ok( () )
    }
    pub fn clear( & mut self ) -> Result< (), & 'static str > {
        for ( &_k, &( ref shader_type, ref handle ) ) in self._textures.iter() {
            router::delete_texture( *handle, (*shader_type).clone() )?
        }
        self._id_to_descrip.clear();
        self._descrip_to_id.clear();
        Ok( () )
    }
    pub fn remove( & mut self, id: u64 ) -> Result< (), & 'static str > {
        match self._textures.remove( &id ) {
            Some( ( shader_type, handle ) ) => {
                router::delete_texture( handle, shader_type )?;
                if let Some( descrip ) = self._id_to_descrip.remove( &id ) {
                    self._descrip_to_id.remove( &descrip );
                }
            },
            None => (),
        }
        Ok( () )
    }
    pub fn get( & mut self, id: u64 ) -> Option< i64 > {
        match self._textures.get( &id ) {
            Some( &( ref _shader_type, ref handle ) ) => {
                return Some( *handle )
            },
            None => return None
        }
    }
}
