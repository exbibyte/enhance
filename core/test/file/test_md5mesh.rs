use implement::file::md5common;
use implement::file::md5mesh;

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
fn test_parse_md5mesh(){
    let file_content = md5common::file_open( "core/asset/ppm/test.ppm" ).expect("file open invalid");
    println!("file content length: {}", file_content.len() );
    md5mesh::parse( &file_content );
}
