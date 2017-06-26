pub use interface::i_pool::*;

#[allow(dead_code)]
pub struct QueueLockfreeTotal < T > {
    pub _head : T,
}

#[allow(unused_variables)]
impl< T > IPool< T > for QueueLockfreeTotal< T > {
    fn clear( & mut self ) -> bool {
        false
    }
    fn empty( & mut self ) -> bool {
        false
    }
    fn size( & mut self ) -> u64 {
        0
    }
    fn put( & mut self, item: T ) -> bool {
        false
    }
    fn get( & mut self ) -> Option< T > {
        None
    }
}
