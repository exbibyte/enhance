use implement::file::md5common;
use implement::file::md5anim;

use std::str;

#[test]
fn test_parse_md5anim(){
    let file_content = md5common::file_open( "core/asset/md5/qshamblerattack01.md5anim" ).expect("file open invalid");
    println!("file content length: {}", file_content.len() );
    md5anim::parse( &file_content ).is_ok();
}
