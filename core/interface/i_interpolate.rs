use implement::math::mat::*;

pub trait IInterpolateMat4x1f64 : Iterator {
    fn interp_increment( & mut self, increment: u64 ) -> Mat4x1< f64 >;
    fn interp_current( & self ) -> Mat4x1< f64 >;
    fn interp_done( & self ) -> bool;
}
