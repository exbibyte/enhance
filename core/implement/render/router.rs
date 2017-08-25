extern crate gl;

use implement::render::util_gl;

#[derive(Copy)]
#[derive(Clone)]
pub enum ShaderType {
    GLSL,
    HLSL,
}

pub fn delete_shader_program( internal_handle: i64, shader_type: ShaderType ) -> Result< (), & 'static str > {
    match shader_type {
        ShaderType::GLSL => util_gl::delete_shader_program( internal_handle ),
        _ => ()
    }
    Ok( () )
}

pub fn delete_texture( internal_handle: i64, shader_type: ShaderType ) -> Result< (), & 'static str > {
    match shader_type {
        ShaderType::GLSL => { util_gl::delete_texture( internal_handle as _ ); },
        _ => (),
    }
    Ok( () )
}
