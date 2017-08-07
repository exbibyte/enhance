use implement::math::mat::Mat4;
use implement::math::mat::Mat3x1;
use implement::math::mat::Mat2x1;
use implement::render::camera;
use std::collections::HashMap;
use std::vec::Vec;

extern crate gl;

use interface::i_renderobj;
use implement::render::renderdevice_gl;
use implement::render::util_gl;

pub struct Mesh {
    pub _id: u64,
    pub _pos: Vec< Mat3x1< f32 > >,
    pub _normal: Vec< Mat3x1< f32 > >,
    pub _tc: Vec< Mat2x1< f32 > >,
    // _xform: Mat4< f32 >,
}

impl Mesh {
    pub fn init( id: u64 ) -> Mesh {
        Mesh {
            _id: id,
            _pos: vec![],
            _normal: vec![],
            _tc: vec![],
        }
    }
}

impl i_renderobj::IRenderBuffer< renderdevice_gl::RenderDrawGroup > for Mesh {
    fn load_into_buffer( & self, rd: & mut renderdevice_gl::RenderDrawGroup ) -> Result< (), & 'static str > {
        if self._pos.len() != self._normal.len() ||
           self._pos.len() != self._tc.len()
        {
            return Err( &"unequal length for position, normal, tc data" )
        }
        let stride = ( 3 + 3 + 2 ) * ::std::mem::size_of::<f32>();
        if stride != rd._stride as _ {
            return Err( &"unequal length stride bytes" )
        }
        let data_len = self._pos.len() * stride;

        for i in 0..self._pos.len() {
            rd._buffer_draw.push( self._pos[i][0] );
            rd._buffer_draw.push( self._pos[i][1] );
            rd._buffer_draw.push( self._pos[i][2] );
            
            rd._buffer_draw.push( self._normal[i][0] );
            rd._buffer_draw.push( self._normal[i][1] );
            rd._buffer_draw.push( self._normal[i][2] );

            rd._buffer_draw.push( self._tc[i][0] );
            rd._buffer_draw.push( self._tc[i][1] );
        }

        println!( "rd._buffer_draw.len(): {}", rd._buffer_draw.len() );
        Ok( () )
    }
}


