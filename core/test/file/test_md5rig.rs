use implement::file::md5common;
use implement::file::md5anim;
use implement::file::md5rig;

use std::io;
use std::io::prelude::*;
use std::fs::File;
use std::io::BufReader;
use std::str;
use std::os;
use std::collections::HashMap;
use std::str::Chars;
use std::iter::Peekable;

#[test]
fn test_parse_md5rig(){
    let file_content = md5common::file_open( "core/asset/md5/qshamblerattack01.md5anim" ).expect("file open invalid");
    println!("file content length: {}", file_content.len() );
    let anim = match md5anim::parse( &file_content ) {
        Ok( o ) => o,
        Err( e ) => panic!( e ),
    };
    let rig = match md5rig::process( & anim ) {
        Ok( o ) => o,
        Err( e ) => panic!( e ),
    };
    assert!( rig._frames.len() as u64 == anim._numframes );
    assert!( rig._framerate as u64 == anim._framerate );
    println!( "rig frames: {:?}", rig._frames );
}
