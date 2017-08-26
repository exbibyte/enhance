#[allow(unused_imports)]
use std::collections::HashMap;
#[allow(unused_imports)]
use implement::render::texture::{ TextureNormalized, TextureBuiltin, Texture, Channel };
#[allow(unused_imports)]
use implement::render::texture;

#[test]
pub fn test_texture() {
    let t_solid : TextureNormalized = TextureNormalized::init_builtin( TextureBuiltin::SOLID );
    let t_checker : TextureNormalized = TextureNormalized::init_builtin( TextureBuiltin::CHECKER );

    let channel_val : HashMap<Channel,u32> = [ (Channel::R, 255u32),
                                               (Channel::G, 120u32),
                                               (Channel::B, 75u32),
    ].iter().cloned().collect();

    let t_modulated_solid : Texture = texture::modulate( &t_solid, &channel_val );
    let t_modulated_checker : Texture = texture::modulate( &t_checker, &channel_val );

    assert!( 255u32 == t_modulated_solid[(0usize,Channel::R)] );
    assert!( 120u32 == t_modulated_solid[(0usize,Channel::G)] );
    assert!( 0u32 == t_modulated_solid[(0usize,Channel::B)] );

    assert!( 0u32 == t_modulated_checker[(0usize,Channel::R)] );
    assert!( 0u32 == t_modulated_checker[(0usize,Channel::G)] );
    assert!( 0u32 == t_modulated_checker[(0usize,Channel::B)] );

    assert!( 255u32 == t_modulated_checker[(1usize,Channel::R)] );
    assert!( 120u32 == t_modulated_checker[(1usize,Channel::G)] );
    assert!( 75u32 == t_modulated_checker[(1usize,Channel::B)] );
   
}
