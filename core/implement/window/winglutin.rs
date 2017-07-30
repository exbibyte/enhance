extern crate gl;
extern crate glutin;
extern crate libc;

use std::ops::FnMut;

use self::glutin::GlContext;
use self::glutin::Event;

use interface::i_window::IWindow;

pub struct WinGlutinBase {
    pub _eventsloop: glutin::EventsLoop,
}

pub struct WinGlutinWin {
    pub _wingl: glutin::GlWindow,
}

pub struct WinGlutin {
    pub _base: WinGlutinBase,
    pub _win: WinGlutinWin,
}

impl IWindow< Event > for WinGlutin {
    fn init( w: u64, h: u64 ) -> WinGlutin {
        let gl_request = glutin::GlRequest::Latest;
        let c = glutin::ContextBuilder::new().with_vsync( true ).with_gl( gl_request );
        let wb = glutin::WindowBuilder::new().with_dimensions( w as u32, h as u32 );
        let base = WinGlutinBase {
            _eventsloop: glutin::EventsLoop::new(),
        };
        let w = WinGlutinWin {
            _wingl: glutin::GlWindow::new( wb, c, &base._eventsloop ).unwrap(),
        };
        WinGlutin {
            _base: base,
            _win: w,
        }
    }
    fn make_current( & self ) -> Result< (), & 'static str > {
        unsafe {
            self._win._wingl.make_current().unwrap();
        }
        unsafe {
            gl::load_with( |symbol| self._win._wingl.get_proc_address(symbol) as * const _ );
        }
        Ok( () )
    }
    fn handle_events < F > ( & mut self, mut cb: F ) -> ()
        where F : FnMut( Event ) -> () {
        self._base._eventsloop.poll_events( cb );
        ()
    }
    fn swap_buf( & self ) -> () {
        self._win._wingl.swap_buffers().unwrap();
        ()
    }
}
