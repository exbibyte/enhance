pub trait IAlloc {
    fn newing< T >( & mut self ) -> Result< Box< T >, &'static str >;
    fn freeing< T >( & mut self, Box< T > ) -> Result< (), &'static str >;
}
