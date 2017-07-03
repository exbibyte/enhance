pub trait IMathTransform {
    fn size( & self ) -> Option < u32 >;
    fn dot( & self, other: & Self ) -> Option< f64 >;
    fn cross( & self, other: & Self ) -> Option< Self > where Self : Sized;
    fn mul_elem( & self, other: & Self ) -> Option< Self > where Self : Sized;
    fn mul( & self, other: & Self ) -> Option< Self > where Self : Sized;
    fn div( & self, other: & Self ) -> Option< Self > where Self : Sized;
    fn plus( & self, other: & Self ) -> Option< Self > where Self : Sized;
    fn minus( & self, other: & Self ) -> Option< Self > where Self : Sized;
    fn magnitude( & self ) -> Option< f64 > where Self : Sized;
    fn normalize( self ) -> Option< Self > where Self : Sized;
    fn scale( & self, s: f64 ) -> Option< Self > where Self : Sized;
    fn is_equal( & self, other: & Self, error: f64  ) -> Option< bool > where Self : Sized;
}
