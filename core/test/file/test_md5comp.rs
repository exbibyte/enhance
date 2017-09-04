use implement::file::md5common;
use implement::file::md5mesh;
use implement::file::md5anim;
use implement::file::md5rig;
use implement::file::md5comp;

use std::str;

#[test]
fn test_parse_md5comp(){
    let file_mesh = md5common::file_open( "core/asset/md5/qshambler.md5mesh" ).expect("md5mesh file open invalid");
    let file_anim = md5common::file_open( "core/asset/md5/qshamblerattack01.md5anim" ).expect("md5anim file open invalid");
    let mesh = match md5mesh::parse( &file_mesh ) {
        Ok( o ) => o,
        Err( e ) => panic!( e ),
    };
    let anim = match md5anim::parse( &file_anim ) {
        Ok( o ) => o,
        Err( e ) => panic!( e ),
    };
    let posecollection = match md5rig::process( & anim ) {
        Ok( o ) => o,
        Err( e ) => panic!( e ),
    };
    assert!( 5 < posecollection._frames.len() );
    let _comp = match md5comp::process( & posecollection, & mesh, 0, 5, 0.5f32 ){
        Ok( o ) => o,
        Err( e ) => panic!( e ),
    };
    // println!( "{:?}", comp );
}
