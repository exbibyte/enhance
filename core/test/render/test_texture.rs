extern crate image;

use std::path::Path;

use self::image::GenericImage;

#[allow(unused_imports)]
use std::collections::HashMap;
#[allow(unused_imports)]
use implement::render::texture::{ TextureNormalized, TextureBuiltin, Texture, Channel };
#[allow(unused_imports)]
use implement::render::texture;

#[test]
pub fn test_texture() {

    //test for loading image into texture
    let img = image::open( &Path::new( "core/asset/images/texture0.jpg" ) ).unwrap();
    println!( "image dimension: {:?}", img.dimensions() );
    println!( "image type: {:?}", img.color() );
    // let texture0 = texture::Texture::from( &img );
    //let texture_data = Vec::from( texture0 ); //todo: verify content


    //test for loading builtin textures
    let t_solid : TextureNormalized = TextureNormalized::init_builtin( TextureBuiltin::SOLID );
    let t_checker : TextureNormalized = TextureNormalized::init_builtin( TextureBuiltin::CHECKER );

    let channel_val : HashMap<Channel,u8> = [ (Channel::R, 255u8),
                                               (Channel::G, 120u8),
                                               (Channel::B, 75u8),
    ].iter().cloned().collect();

    let t_modulated_solid : Texture = texture::modulate( &t_solid, &channel_val );
    let t_modulated_checker : Texture = texture::modulate( &t_checker, &channel_val );

    assert!( 255u8 == t_modulated_solid[(0usize,Channel::R)] );
    assert!( 120u8 == t_modulated_solid[(0usize,Channel::G)] );
    assert!( 0u8 == t_modulated_solid[(0usize,Channel::B)] );

    assert!( 0u8 == t_modulated_checker[(0usize,Channel::R)] );
    assert!( 0u8 == t_modulated_checker[(0usize,Channel::G)] );
    assert!( 0u8 == t_modulated_checker[(0usize,Channel::B)] );

    assert!( 255u8 == t_modulated_checker[(1usize,Channel::R)] );
    assert!( 120u8 == t_modulated_checker[(1usize,Channel::G)] );
    assert!( 75u8 == t_modulated_checker[(1usize,Channel::B)] );
   
}
