#[allow(non_camel_case_types)]
pub trait i_threadpool {
    fn spawn( & mut self ) -> bool;
    fn terminate( & mut self ) -> bool;
    fn withdraw( & mut self ) -> Option<u64>;
    fn deposit( & mut self, handle: u64 ) -> bool;
    fn query_total( & mut self ) -> u64;
    fn query_available( & mut self ) -> u64;
    fn query_unavailable( & mut self ) -> u64;
}
