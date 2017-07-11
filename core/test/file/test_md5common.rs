use std::collections::HashMap;

use implement::file::md5common;

#[cfg(test)]
#[derive(Debug)]
#[derive(PartialEq)]
pub enum TestToken {
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

    let mut hm_keywords = HashMap::new();
    hm_keywords.insert( "MD5Version", TestToken::Version );
    hm_keywords.insert( "commandline", TestToken::Commandline );
    hm_keywords.insert( "numJoints", TestToken::Numjoints );
    hm_keywords.insert( "numMeshes", TestToken::Nummeshes );
    hm_keywords.insert( "joints", TestToken::Joints );
    hm_keywords.insert( "mesh", TestToken::Mesh );
    hm_keywords.insert( "shader", TestToken::Shader );
    hm_keywords.insert( "numverts", TestToken::Numverts );
    hm_keywords.insert( "verts", TestToken::Verts );
    hm_keywords.insert( "numtris", TestToken::Numtris );
    hm_keywords.insert( "tri", TestToken::Tri );
    hm_keywords.insert( "numweights", TestToken::Numweights );
    hm_keywords.insert( "weight", TestToken::Weight );
    
    let mut count = 0;
    let mut idx = 0usize;
    loop {
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
