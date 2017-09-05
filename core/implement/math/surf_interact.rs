use implement::math::vector::Mat2x1< f64 >;
use implement::math::vector::Mat3x1< f64 >;
use implement::math::vector::Mat4x1< f64 >;

///todo
pub struct SurfInteract {
    _point: Mat2x1< f64 >,
    _dpdu: Mat3x1< f64 >,
    _dpdv: Mat3x1< f64 >,
    _normal: Mat3x1< f64 >,
    _dndu: Mat3x1< f64 >,
    _dndv: Mat3x1< f64 >,
}
