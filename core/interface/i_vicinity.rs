pub trait IVicinity< T > {
    fn set_vicinity( & mut self, epsilon: T );
    fn within_vicinity( & self, a: T, b: T ) -> bool;
}
