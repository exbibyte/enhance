use implement::math::mat::*;

pub trait IInterpolateMat4x1f64 : DoubleEndedIterator {
    type InterpVal;
    fn interp_delta( & mut self, steps: i64 ) -> Option< Mat4x1< f64 > >;
    fn interp_current( & self ) -> Mat4x1< f64 >;
    fn interp_is_end( & self ) -> bool;
    fn interp_is_start( & self ) -> bool;
    fn num_steps( & self ) -> u64;
    fn reset( & mut self );
}
