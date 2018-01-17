#![feature(use_extern_macros)]

/// example using ads rendering path to render primitives

extern crate gl;
extern crate glutin;
extern crate rand;
extern crate image;
extern crate e2rcore;

use std::fs::File;
use std::path::Path;
use std::io::BufReader;
use std::io::Read;
use std::str;
use rand::Rng;

use self::image::GenericImage;

use self::glutin::GlContext;

use self::e2rcore::implement::kernel::kernel_impl_000::KernelImpl000;

use self::e2rcore::interface::i_ele;
use self::e2rcore::interface::i_window::IWindow;
use self::e2rcore::interface::i_renderobj;
use self::e2rcore::interface::i_renderer::IRenderer;

use self::e2rcore::implement::render::util_gl;
use self::e2rcore::implement::math;
use self::e2rcore::implement::render::camera;
use self::e2rcore::implement::render::light;
use self::e2rcore::implement::render::mesh;
use self::e2rcore::implement::render::primitive;
use self::e2rcore::implement::render::texture;

use self::e2rcore::implement::window::winglutin::WinGlutin;

use self::e2rcore::implement::kernel::kernel_render::Renderer;
use self::e2rcore::implement::kernel::kernel_render::DummyRenderEventType;
use self::e2rcore::implement::render::render_commands;

pub fn file_open( file_path: & str ) -> Option<String> {
    let path = File::open( file_path ).expect("file path open invalid");
    let mut buf_reader = BufReader::new(path);
    let mut contents = String::new();
    match buf_reader.read_to_string( & mut contents ){
        Err( e ) => { println!("{}", e ); return None },
        _ => (),
    }
    Some(contents)
}

#[allow(unused)]
fn main() {

    //load assets
    let img = image::open( &Path::new( "core/asset/images/texture0.jpg" ) ).unwrap();
    println!( "image dimension: {:?}", img.dimensions() );
    println!( "image type: {:?}", img.color() );
    
    let vs_src = file_open( "core/example/shading/ads.vs" ).expect("vertex shader not retrieved");
    let fs_src = file_open( "core/example/shading/ads.fs" ).expect("fragment shader not retrieved");

    let texture0 = texture::Texture::from( &img );

    //setup kernel
    
    let mut k = KernelImpl000::init().unwrap();
    k._windowing.make_current();

    k._renderer.push( Renderer::init().unwrap() );

    k.run();

}
