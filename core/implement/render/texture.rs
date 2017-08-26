use std::collections::HashMap;
use std::cmp::Eq;
use std::cmp::PartialEq;

extern crate image;
extern crate num;

use self::image::Pixel;

#[derive(Clone)]
pub struct TextureNormalized {
    _data: Vec<(Channel,f32)>,
    _dim: Vec<usize>,
    _channels: HashMap<Channel,usize>,
    
}

#[derive(Clone)]
#[derive(Eq)]
#[derive(PartialEq)]
#[derive(Hash)]
pub enum Channel {
    R,
    G,
    B,
    A,
}

#[derive(Clone)]
pub enum TextureBuiltin {
    SOLID,
    CHECKER,
}

use std::ops::Index;
use std::ops::IndexMut;

#[derive(Clone)]
pub struct Texture {
    pub _data: Vec<(Channel, u8)>,
    pub _dim: Vec<usize>,
    pub _channels: HashMap<Channel,usize>,
}

impl Index<(usize, Channel)> for Texture {
    type Output = u8;
    fn index( &self, i: (usize, Channel) ) -> &u8 {
        let tuple_index = match self._channels.get( &i.1 ) {
            Some( &v ) => v,
            _ => { panic!("unmatching channel detected"); 0usize },
        };
        &self._data[ i.0 * self._channels.len() + tuple_index ].1
    }
}

impl Texture {
    pub fn from< T: image::GenericImage >( img_buffer: &T ) -> Texture
    {
        let mut buf = vec![];
        for (x,y,p) in img_buffer.pixels() {
            let rgb = p.to_rgb();
            let r: u8 = num::cast(rgb.data[0]).unwrap();
            let g: u8 = num::cast(rgb.data[1]).unwrap();
            let b: u8 = num::cast(rgb.data[2]).unwrap();
            buf.push( ( Channel::R, r ) );
            buf.push( ( Channel::G, g ) );
            buf.push( ( Channel::B, b ) );
        }
        Texture {
            _data: buf,
            _dim: vec![ img_buffer.dimensions().0 as _, img_buffer.dimensions().1 as _ ],
            _channels: [ (Channel::R, 0usize), (Channel::G, 1usize), (Channel::B, 2usize) ].iter().cloned().collect(),
        }
    }
}

impl From< Texture > for Vec< u8 > {
    fn from( t: Texture ) -> Vec< u8 > {
        let mut v = vec![];
        for i in t._data {
            v.push( i.1 );
        }
        v
    }
}

impl TextureNormalized {
    pub fn init_builtin( builtin: TextureBuiltin ) -> TextureNormalized {
        match builtin {
            TextureBuiltin::SOLID => {
                return TextureNormalized {
                    _data: vec![ (Channel::R, 1.0), (Channel::G, 1.0), (Channel::B, 0.0) ],
                    _dim: vec![ 1 ],
                    _channels: [ (Channel::R, 0usize), (Channel::G, 1usize), (Channel::B, 2usize) ].iter().cloned().collect(),
                }
            },
            TextureBuiltin::CHECKER => {
                return TextureNormalized {
                    _data: vec![ (Channel::R, 0.0), (Channel::G, 0.0), (Channel::B, 0.0),
                                 (Channel::R, 1.0), (Channel::G, 1.0), (Channel::B, 1.0) ],
                    _dim: vec![ 2 ],
                    _channels: [ (Channel::R, 0usize), (Channel::G, 1usize), (Channel::B, 2usize) ].iter().cloned().collect()
                }
            },
            _ => panic!(),
        }
    }
}

pub fn modulate( normalized: & TextureNormalized, hm: & HashMap<Channel,u8> ) -> Texture {
    let mut t = Texture {
        _data: vec![],
        _dim: normalized._dim.clone(),
        _channels: normalized._channels.clone(),
    };
    for i in normalized._data.iter() {
        match i {
            &( ref c, ref v) => {
                let val = match hm.get( c ){
                    Some(&a) => a,
                    _ => { println!("unmatched channel, setting to 0 instead"); 0u8 },
                };
                t._data.push( (Channel::R, (v * val as f32) as u8 ) );
            },
        }
    }
    t
}

