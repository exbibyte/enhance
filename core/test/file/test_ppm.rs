use implement::file::ppm;

use std::str;

#[test]
fn test_parse_ppm(){
    let ( w, h, img_bytes ) = ppm::read( "core/test_asset/sample_ppm/boxes_1.ppm" ).expect("ppm file read invalid");
    println!("w: {}, h: {}, image bytes length: {}", w, h, img_bytes.len() );
    assert!( w == 63 );
    assert!( h == 63 );
    assert!( img_bytes.len() == 63 * 63 * 3 );
}
