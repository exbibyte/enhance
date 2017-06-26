use std::vec;
// use std::thread;

use init;
use init::i_init;

#[allow(non_camel_case_types)]
pub struct tp0< F > where F: Fn()->() {
    pub _f : F,
    _v : vec::Vec<u64>,
}

impl<F> super::interface::i_threadpool for tp0<F> where F: Fn()->() {
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

impl<F> init::i_init for tp0<F> where F: Fn()->() {
    fn init( & mut self ) -> bool {
        self._v = vec::Vec::new();
        for i in 0..3 {
            self._v.push(i);
        }
        true
    }
    fn deinit( & mut self ) -> bool {
        println!("threadpoo0 deinit.");
        self._v.clear();
        true
    }
}

impl<F> Drop for tp0<F> where F: Fn()->() {
    fn drop( & mut self ) {
        self.deinit();
    }
}

impl<F> tp0<F> where F:Fn()->() {
    pub fn new( f: F ) -> tp0<F> {
        tp0{ _f: f,
             _v: vec::Vec::new(),
        }
    }
}

// fn default_func() -> () {
//     println!("default_func!");
// }

// impl Default for tp0<F> where F:Fn()->() {
//     fn default() -> tp0<F> {
//         tp0<F>::new( default_func )
//     }
// }
