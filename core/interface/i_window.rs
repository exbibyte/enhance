use std::ops::FnMut;

pub trait IWindow < EventType >{
    fn init( dimx: u64, dimy: u64 ) -> Self;
    fn make_current( & self ) -> Result< (), & 'static str >;
    fn handle_events < F > ( & mut self, mut cb: F ) -> () where F: FnMut( EventType ) -> ();
    fn swap_buf( & self ) -> ();
}
