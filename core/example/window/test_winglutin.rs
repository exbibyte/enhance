extern crate gl;
extern crate glutin;
extern crate libc;

extern crate e2rcore;

use self::glutin::GlContext;

use self::e2rcore::interface::i_window::IWindow;
use self::e2rcore::implement::window::winglutin::WinGlutin;

fn main() {
    let mut window : WinGlutin = IWindow::init( 500, 500 );
    window.make_current().expect("window make_current failed");
    
    let mut running = true;
    while running {
        let mut new_win_dim = None;
        window.handle_events( |event| {
            match event {
                glutin::Event::WindowEvent{ event, .. } => match event {
                    glutin::WindowEvent::Closed => running = false,
                    glutin::WindowEvent::Resized(w, h) => new_win_dim = Some( (w,h) ),
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
