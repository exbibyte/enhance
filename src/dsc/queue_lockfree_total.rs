#[allow(dead_code)]
#[allow(non_camel_case_types)]
pub struct queue < T > {
    _head : T,
}

#[allow(unused_variables)]
impl< T > super::interface::i_pool< T > for queue< T > {
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
