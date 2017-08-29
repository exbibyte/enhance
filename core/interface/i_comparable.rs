pub trait IComparableError< T > {
    fn is_equal( & self, other: & Self, error: T ) -> Option< bool >;
}
