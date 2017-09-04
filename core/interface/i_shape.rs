use interface::i_bound::IBound;
// use implement::math::vector::Vector3;

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

pub trait IShape {
    fn get_type( & self ) -> ShapeType;
    fn get_bound( & self ) -> IBound;
    // this shall test for intersection of bounding shapes first before procedding to test intersection using algorithms of higher complexity
    fn does_intersect( & self, other: & IShape ) -> bool;
    //returns a location of intersection, preferrably closest of such locations
    fn get_intersect( & self, other: & IShape ) -> [ f64; 3 ];
}
