
pub trait IInterpolate < T > : DoubleEndedIterator< Item = T >  where T: Clone {
    fn interp_delta( & mut self, steps: i64 ) -> Option< T >;
    fn interp_current( & self ) -> T;
    fn interp_is_end( & self ) -> bool;
    fn interp_is_start( & self ) -> bool;
    fn num_steps( & self ) -> u64;
    fn reset( & mut self );
}
