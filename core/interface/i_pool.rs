pub trait IPool< T > {
    fn clear( & mut self ) -> bool;
    fn empty( & mut self ) -> bool;
    fn size( & mut self ) -> u64;
    fn put( & mut self, item: T ) -> bool;
    fn get( & mut self ) -> Option< T >;
}
