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
    //custom shapes
    COMPLEX,
}

pub trait IShape : IVicinity< f64 > {
    fn get_shape_data( & self ) -> Vec< f64 >;
    fn get_type( & self ) -> ShapeType;
    fn get_bound( & self ) -> &IBound;
    // this shall test for intersection of bounding shapes first before procedding to test intersection using algorithms of higher complexity
    //optionally returns a location of intersection, preferrably closest of such locations
    fn get_intersect( & self, other: & IShape ) -> ( bool, Option< Mat3x1< f64 > > );
}
