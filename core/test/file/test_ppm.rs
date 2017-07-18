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
fn test_parse_ppm(){
    let ( w, h, img_bytes ) = ppm::read( "core/test_asset/sample_ppm/boxes_1.ppm" ).expect("ppm file read invalid");
    println!("w: {}, h: {}, image bytes length: {}", w, h, img_bytes.len() );
    assert!( w == 63 );
    assert!( h == 63 );
    assert!( img_bytes.len() == 63 * 63 * 3 );
}
