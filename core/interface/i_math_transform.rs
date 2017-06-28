pub trait IMathTransform {
    fn size( & self ) -> u32;
    fn dot( & self, other: & Self ) -> Self;
}
