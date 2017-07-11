use std::str;
use std::os;
use std::collections::HashMap;
use std::str::Chars;
use std::iter::Peekable;

use implement::math::quat::Quat;
use implement::file::md5common;

#[derive(Debug)]
#[derive(PartialEq)]
pub enum MD5MeshToken {
    Version,
    Commandline,
    Numjoints,
    Nummeshes,
    Joints,
    Mesh,
    Shader,
    Numverts,
    Verts,
    Numtris,
    Tri,
    Numweights,
    Weight,
}

pub struct Md5MultiMesh {
    pub _md5ver: u64,
    pub _cmdline: String,
    pub _numjoints: u64,
    pub _nummeshes: u64,
    pub _joints: Vec< Md5Joint >,
    pub _meshes: Vec< Md5Mesh >,
}

pub struct Md5Mesh {
    pub _shader: String,
    pub _numverts: u64,
    pub _numtris: u64,
    pub _numweights: u64,
    pub _verts: Vec< Md5Vert >,
    pub _tris: Vec< Md5Tri >,
    pub _weights: Vec< Md5Weight >,
}

pub struct Md5Joint {
    pub _name: String,
    pub _parent_index: u64,
    pub _pos: [f64;3],
    pub _orient: [f64;3],
    pub _rot: Quat,
}

pub struct Md5Vert {
    pub _index: u64,
    pub _tex_coords: [f64;2],
    pub _weight_start: u64,
    pub _weight_count: u64,
    pub _normal: [f64;3],
    pub _pos: [f64;3],
}

pub struct Md5Tri {
    pub _index: u64,
    pub _vert_indices: [f64;3],
}

pub struct Md5Weight {
    pub _index: u64,
    pub _joint_index: u64,
    pub _weight_bias: f64,
    pub _pos: [f64;3],
}

impl Md5MultiMesh {
    //entry point to user
    pub fn init( path: &str ) -> Md5MultiMesh {
        unimplemented!();
    }
}

pub fn parse( file_content: &str ){

    let mut hm_keywords = HashMap::new();
    hm_keywords.insert( "MD5Version", MD5MeshToken::Version );
    hm_keywords.insert( "commandline", MD5MeshToken::Commandline );
    hm_keywords.insert( "numJoints", MD5MeshToken::Numjoints );
    hm_keywords.insert( "numMeshes", MD5MeshToken::Nummeshes );
    hm_keywords.insert( "joints", MD5MeshToken::Joints );
    hm_keywords.insert( "mesh", MD5MeshToken::Mesh );
    hm_keywords.insert( "shader", MD5MeshToken::Shader );
    hm_keywords.insert( "numverts", MD5MeshToken::Numverts );
    hm_keywords.insert( "verts", MD5MeshToken::Verts );
    hm_keywords.insert( "numtris", MD5MeshToken::Numtris );
    hm_keywords.insert( "tri", MD5MeshToken::Tri );
    hm_keywords.insert( "numweights", MD5MeshToken::Numweights );
    hm_keywords.insert( "weight", MD5MeshToken::Weight );
    
    let mut count = 0;
    let mut idx = 0usize;
    loop {
        let ( tok, idx_s, idx_e, idx_next ) = md5common::tokenize( &file_content[0..], idx, & mut hm_keywords );
        match tok {
            md5common::Token::End => {
                println!("token: {:?}, index: {:?}, content: {:?}.", tok, idx_next, &file_content[idx_s..idx_e] );
                break;
            },
            md5common::Token::Invalid =>
                println!("token: {:?}, index: {:?}, content: {:?}.", tok, idx_next, &file_content[idx_s..idx_e] ),
            _ => ()
        }
        idx = idx_next;
        count += 1;
    }
}
