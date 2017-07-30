extern crate gl;
extern crate glutin;
extern crate libc;

extern crate e2rcore;

use std::mem;
use std::fs::File;
use std::io::BufReader;
use std::str::FromStr;
use std::io::Read;
use std::ffi::CStr;
use std::os::raw::c_char;
use std::fmt;
use std::str;

use self::glutin::GlContext;

use self::e2rcore::interface::i_window::IWindow;
use self::e2rcore::implement::window::winglutin::WinGlutin;

use self::e2rcore::implement::capability::capability_gl;
use self::e2rcore::implement::render::util_gl;

pub fn file_open( file_path: & str ) -> Option<String> {
    let path = File::open( file_path ).expect("file path open invalid");
    let mut buf_reader = BufReader::new(path);
    let mut contents = String::new();
    buf_reader.read_to_string( & mut contents );
    Some(contents)
}

fn main() {
    
    let vs_src = file_open( "core/example/shading/ads.vs" ).expect("vertex shader not retrieved");
    let fs_src = file_open( "core/example/shading/ads.fs" ).expect("fragment shader not retrieved");
    
    let mut window : WinGlutin = IWindow::init( 500, 500 );
    window.make_current().expect("window make_current failed");

    let cap = capability_gl::query_gl();
    println!( "{}", cap );

    let vs = match util_gl::load_and_compile_shader( vs_src.as_str(), util_gl::ShaderType::VERTEX ) { Ok( o ) => o, Err( o ) => { panic!( "{}", o ); } };
    let fs = match util_gl::load_and_compile_shader( fs_src.as_str(), util_gl::ShaderType::FRAGMENT ) { Ok( o ) => o, Err( o ) => { panic!( "{}", o ); } };

    let mut gl_program = 0;
    unsafe {
        gl_program = gl::CreateProgram();
        if gl_program == 0 {
            panic!("gl program creation failed");
        }
        gl::AttachShader( gl_program, vs );
        gl::AttachShader( gl_program, fs );
        gl::LinkProgram( gl_program );
        match util_gl::check_program_link( gl_program ) {
            Err( o ) => panic!( "{}", o ),
            _ => ()
        }
        gl::UseProgram( gl_program );
        util_gl::check_last_op();
        gl::BindFramebuffer( gl::FRAMEBUFFER, 0 );
    }
    
    let mut running = true;
    while running {
        let mut new_win_dim = None;
        window.handle_events( |event| {
            match event {
                glutin::Event::WindowEvent{ event, .. } => match event {
                    glutin::WindowEvent::Closed => running = false,
                    glutin::WindowEvent::Resized(w, h) => new_win_dim = Some( (w,h) ),
                    glutin::WindowEvent::KeyboardInput {
                        input: glutin::KeyboardInput {
                            state: glutin::ElementState::Pressed,
                            virtual_keycode: Some( glutin::VirtualKeyCode::Q ),
                            ..
                        }, ..
                    } => running = false,
                    _ => (),
                },
                _ => ()
            }
        } );
        if let Some( ( w, h ) ) = new_win_dim {
            window._win._wingl.resize(w, h);
        }
        unsafe {
            gl::ClearColor( 0.3, 0.3, 0.3, 1.0 );
            gl::Clear(gl::COLOR_BUFFER_BIT);
        }
        window.swap_buf();
    }
}
