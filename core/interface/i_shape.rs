// use implement::math::vector::Vector3;

// enum ShapeType {
//     POINT,
//     RAY,
//     SPHERE,
//     PLANE,
//     TRIG,
//     BOX,
//     NGON,
// }

// pub struct Bound {
//     _min: Vector3< f64 >,
//     _max: Vector3< f64 >,
// }

// impl Bound {
//     #[allow(dead_code)]
//     fn init( a: Vector3< f64 >, b: Vector3< f64 > ) -> Option< IBound > {
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
//     fn get_type() -> ShapeType;
//     fn get_bound( & self ) -> IBound;
//     // this shall test for intersection of bounding shapes first before procedding to test intersection using algorithms of higher complexity
//     fn intersect< Other >( & self, other: & Other ) -> Option< [f64;3] > where Other: IShape;
// }
