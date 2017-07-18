extern crate gl;
extern crate glutin;
extern crate libc;

use glutin::GlContext;

pub struct WinGlutinBase {
    // pub _win: glutin::WindowBuilder,
    pub _eventsloop: glutin::EventsLoop,
    // pub _context: glutin::ContextBuilder <'a>,
}

pub struct WinGlutinGL {
    pub _glwin: glutin::GlWindow,
}

impl WinGlutinBase {
    pub fn init_with_dim( w: u32, h: u32 ) -> ( WinGlutinBase, WinGlutinGL ) {
        let _context = glutin::ContextBuilder::new().with_vsync( true );
        let _win = glutin::WindowBuilder::new().with_dimensions( w, h );
        let _base = WinGlutinBase {
            _eventsloop: glutin::EventsLoop::new(),
        };
        let _gl = WinGlutinGL {
            _glwin: glutin::GlWindow::new( _win, _context, &_base._eventsloop ).unwrap(),
        };
        ( _base, _gl )
    }
}

impl WinGlutinGL {
    pub fn make_current( & self ) {
        unsafe {
            self._glwin.make_current().unwrap();
        }
        unsafe {
            gl::load_with(|symbol| self._glwin.get_proc_address(symbol) as *const _);
        }
    }
}

fn main() {

    let ( mut win, mut wingl ) = WinGlutinBase::init_with_dim( 500, 500 );
    // let mut events_loop = glutin::EventsLoop::new();
    // let window = glutin::WindowBuilder::new()
    //     .with_title("Hello, world!")
    //     .with_dimensions(1024, 768);
    // let context = glutin::ContextBuilder::new()
    //     .with_vsync(true);
    // let gl_window = glutin::GlWindow::new(window, context, &events_loop).unwrap();
    wingl.make_current();
    // unsafe {
    //     gl_window.make_current().unwrap();
    // }

    // unsafe {
    //     gl::load_with(|symbol| gl_window.get_proc_address(symbol) as *const _);
    //     gl::ClearColor(0.0, 1.0, 0.0, 1.0);
    // }

    let mut running = true;
    while running {
        win._eventsloop.poll_events(|event| {
            match event {
                glutin::Event::WindowEvent{ event, .. } => match event {
                    glutin::WindowEvent::Closed => running = false,
                    glutin::WindowEvent::Resized(w, h) => wingl._glwin.resize(w, h),
                    _ => (),
                },
                _ => ()
            }
        });

        unsafe {
            gl::Clear(gl::COLOR_BUFFER_BIT);
        }

        wingl._glwin.swap_buffers().unwrap();
    }
}
