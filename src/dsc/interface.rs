#[allow(non_camel_case_types)]
pub trait i_pool< T > {
    fn clear( & mut self ) -> bool;
    fn empty( & mut self ) -> bool;
    fn size( & mut self ) -> u64;
    fn put( & mut self, item: T ) -> bool;
    fn get( & mut self ) -> Option< T >;
}
