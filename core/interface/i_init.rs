#[allow(non_camel_case_types)]
pub trait i_init {
    fn init( & mut self ) -> bool;
    fn deinit( & mut self ) -> bool;
}
