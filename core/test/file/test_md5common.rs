use implement::file::md5common;

use std::io;
use std::io::prelude::*;
use std::fs::File;
use std::io::BufReader;
use std::str;
use std::os;
use std::collections::HashMap;
use std::str::Chars;
use std::iter::Peekable;

#[derive(Debug)]
#[derive(PartialEq)]
enum MD5MeshToken {
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

#[test]
fn test_parse_common(){

    let file_content = md5common::file_open( "core/asset/md5/qshambler.md5mesh" ).expect("file open invalid");
    println!("file content length: {}", file_content.len() );
    // skip_white_space( &file_content[0..], &keywords );

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
    // while count < 1000 {
    loop {
        let idx_prev = idx;
        let ( tok, idx_s, idx_e, idx_next ) = md5common::tokenize( &file_content[0..], idx, & mut hm_keywords );
        match tok {
            md5common::Token::End => {
                println!("token: {:?}, index: {:?}, content: {:?}.", tok, idx_next, &file_content[idx_s..idx_e] );
                break;
            },
            md5common::Token::Keyword =>
                println!("token: {:?}, index: {:?}, content: {:?}.", tok, idx_next, &file_content[idx_s..idx_e] ),
            // _ => println!("token: {:?}, index: {:?}, content: {:?}.", tok, idx_next, &file_content[idx_s..idx_e] )
            _ => ()
        }
        idx = idx_next;
        count += 1;
    }
    
}
