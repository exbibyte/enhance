use std::vec;

use init;

#[allow(non_camel_case_types)]
#[derive(Default)]
pub struct tp0 {
    // _t : thread::Thread,
    _v : vec::Vec<u64>,
}

impl super::interface::i_threadpool for tp0 {
    fn spawn( & mut self ) -> bool {
        false
    }
    fn terminate( & mut self ) -> bool {
        false
    }
    fn withdraw( & mut self ) -> Option<u64> {
        self._v.pop()
    }
    #[allow(unused_variables)]
    fn deposit( & mut self, handle: u64 ) -> bool {
        false
    }
    fn query_total( & mut self ) -> u64 {
        0
    }
    fn query_available( & mut self ) -> u64 {
        0
    }
    fn query_unavailable( & mut self ) -> u64 {
        0
    }
}

impl init::i_init for tp0 {
    fn init( & mut self ) -> bool {
        self._v = vec::Vec::new();
        for i in 0..3 {
            self._v.push(i);
        }
        true
    }
    fn deinit( & mut self ) -> bool {
        false
    }
}
