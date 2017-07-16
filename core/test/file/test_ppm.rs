use implement::file::ppm;

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
    let img_bytes = ppm::read( "core/asset/md5/qshambler.md5mesh" ).expect("ppm file read invalid");
    println!("image bytes length: {}", img_bytes.len() );
}
