use interface::i_bound::IBound;
use interface::i_vicinity::IVicinity;

use implement::math::mat::Mat3x1;

pub enum ShapeType {
    //primitive shapes
    POINT,
    RAY,
    SPHERE,
    PLANE,
    TRIG,
    BOX,
    FRUSTUM,
    //custom shapes
    COMPLEX,
}

pub trait IShape : IVicinity< f64 > {
    fn get_shape_data( & self ) -> Vec< f64 >;
    fn get_type( & self ) -> ShapeType;
    fn get_bound( & self ) -> &IBound;
    //optionally returns a location of intersection of bounding shapes, preferrably closest of such locations
    fn get_intersect( & self, other: & IShape ) -> ( bool, Option< Mat3x1< f64 > > );
    //required for gjk intersection test
    fn get_support( & self, v: & Mat3x1< f64 > ) -> Option< Mat3x1< f64 > >;
}
