extern crate gl;
extern crate glutin;
extern crate libc;

extern crate e2rcore;

use std::mem;

use self::glutin::GlContext;

use self::e2rcore::interface::i_window::IWindow;
use self::e2rcore::implement::window::winglutin::WinGlutin;

fn main() {
    
    let mut window : WinGlutin = IWindow::init( 500, 500 );
    window.make_current().expect("window make_current failed");

    //create g buffers for deferred shading
    let mut defer_fbo : gl::types::GLuint = 0;
    let mut defer_depth: gl::types::GLuint = 0;
    let mut defer_pos: gl::types::GLuint = 0;
    let mut defer_normal: gl::types::GLuint = 0;
    let mut defer_color: gl::types::GLuint = 0;

    let width = 500;
    let height = 500;
    
    unsafe {
        //create and bind fbo
        gl::GenFramebuffers( 1, & mut defer_fbo );
        gl::BindFramebuffer( gl::FRAMEBUFFER, defer_fbo );

        //bind depth buffer
        gl::GenFramebuffers( 1, & mut defer_depth );
        gl::BindRenderbuffer( gl::RENDERBUFFER, defer_depth );
        gl::RenderbufferStorage( gl::RENDERBUFFER, gl::DEPTH_COMPONENT, width, height );

        //create position buffer, assign index 0 as texture unit
        gl::ActiveTexture( gl::TEXTURE0 );
        gl::GenTextures( 1, & mut defer_pos );
        gl::BindTexture( gl::TEXTURE_2D, defer_pos );
        let pixels_ptr : * const std::os::raw::c_void = std::ptr::null();
        gl::TexImage2D( gl::TEXTURE_2D, 0, gl::RGB32F as gl::types::GLint, width, height, 0, gl::RGB, gl::UNSIGNED_BYTE, pixels_ptr );
        gl::TexParameteri( gl::TEXTURE_2D, gl::TEXTURE_MIN_FILTER, gl::NEAREST as gl::types::GLint );
        gl::TexParameteri( gl::TEXTURE_2D, gl::TEXTURE_MAG_FILTER, gl::NEAREST as gl::types::GLint );

        //create normal buffer, assign index 1 as texture unit
        gl::ActiveTexture( gl::TEXTURE1 );
        gl::GenTextures( 1, & mut defer_normal );
        gl::BindTexture( gl::TEXTURE_2D, defer_normal );
        gl::TexImage2D( gl::TEXTURE_2D, 0, gl::RGB32F as gl::types::GLint, width, height, 0, gl::RGB, gl::UNSIGNED_BYTE, pixels_ptr );
        gl::TexParameteri( gl::TEXTURE_2D, gl::TEXTURE_MIN_FILTER, gl::NEAREST as gl::types::GLint );
        gl::TexParameteri( gl::TEXTURE_2D, gl::TEXTURE_MAG_FILTER, gl::NEAREST as gl::types::GLint );

        //create diffuse buffer, assign index 2 for texture unit
        gl::ActiveTexture( gl::TEXTURE2 );
        gl::GenTextures( 1, & mut defer_color );
        gl::BindTexture( gl::TEXTURE_2D, defer_color );
        gl::TexImage2D( gl::TEXTURE_2D, 0, gl::RGB as gl::types::GLint, width, height, 0, gl::RGB, gl::UNSIGNED_BYTE, pixels_ptr );
        gl::TexParameteri( gl::TEXTURE_2D, gl::TEXTURE_MIN_FILTER, gl::NEAREST as gl::types::GLint );
        gl::TexParameteri( gl::TEXTURE_2D, gl::TEXTURE_MAG_FILTER, gl::NEAREST as gl::types::GLint );
        //attach images to frame buffer
        gl::FramebufferRenderbuffer( gl::FRAMEBUFFER, gl::DEPTH_ATTACHMENT, gl::RENDERBUFFER, defer_depth );
        gl::FramebufferTexture2D( gl::FRAMEBUFFER, gl::COLOR_ATTACHMENT0, gl::TEXTURE_2D, defer_pos, 0 );
        gl::FramebufferTexture2D( gl::FRAMEBUFFER, gl::COLOR_ATTACHMENT1, gl::TEXTURE_2D, defer_normal, 0 );
        gl::FramebufferTexture2D( gl::FRAMEBUFFER, gl::COLOR_ATTACHMENT2, gl::TEXTURE_2D, defer_color, 0 );
        let drawbuffers = [ gl::NONE, gl::COLOR_ATTACHMENT0, gl::COLOR_ATTACHMENT1, gl::COLOR_ATTACHMENT2 ];
        gl::DrawBuffers( drawbuffers.len() as i32, & drawbuffers[0] );
        
    }

    // let mut running = true;
    // while running {
    //     let mut new_win_dim = None;
    //     window.handle_events( |event| {
    //         match event {
    //             glutin::Event::WindowEvent{ event, .. } => match event {
    //                 glutin::WindowEvent::Closed => running = false,
    //                 glutin::WindowEvent::Resized(w, h) => new_win_dim = Some( (w,h) ),
    //                 _ => (),
    //             },
    //             _ => ()
    //         }
    //     } );
    //     if let Some( ( w, h ) ) = new_win_dim {
    //         window._win._wingl.resize(w, h);
    //     }
    //     unsafe {
    //         gl::ClearColor( 0.3, 0.3, 0.3, 1.0 );
    //         gl::Clear(gl::COLOR_BUFFER_BIT);
    //     }
    //     window.swap_buf();
    // }
}
