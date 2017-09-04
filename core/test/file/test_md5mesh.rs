use implement::file::md5common;
use implement::file::md5mesh;

use std::str;

#[test]
fn test_parse_md5mesh(){
    let file_content = md5common::file_open( "core/asset/md5/qshambler.md5mesh" ).expect("file open invalid");
    println!("file content length: {}", file_content.len() );
    md5mesh::parse( &file_content ).is_ok();
}
