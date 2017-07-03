// use implement::math::vector::Vector4;

// pub struct Bound {
//     _min: Vector4< f64 >,
//     _max: Vector4< f64 >,
// }

// impl Bound {
//     #[allow(dead_code)]
//     fn init( a: Vector4< f64 >, b: Vector4< f64 > ) -> Option< Bound > {
//         if  a._val[0] <= b._val[0] &&
//             a._val[1] <= b._val[1] &&
//             a._val[2] <= b._val[2] {
//             Some( Bound { _min: a, _max: b } )
//         } else if
//             a._val[0] >= b._val[0] &&
//             a._val[1] >= b._val[1] &&
//             a._val[2] >= b._val[2] {
//             Some( Bound { _min: b, _max: a } )
//         } else {
//             None
//         }
//     }
// }

// pub trait IShape {
//     fn bound( & self ) -> Option< Bound >;
//     fn intersect( & self, other: & Self ) -> Option< Bound >;
// }
