use interface::i_shape::{ ShapeType, IShape };
use interface::i_bound::IBound;
use interface::i_vicinity::IVicinity;

use implement::math::mat::Mat3x1;
use implement::math::bound::AxisAlignedBBox;

#[derive(Debug, Clone)]
pub struct Point3 {
    pub _ori: Mat3x1< f64 >,
    pub _bound: AxisAlignedBBox,
    pub _vicinity: f64,
}

impl Point3 {
    pub fn init( origin: &[f64] ) -> Point3 {
        assert!( origin.len() == 3 );
        Point3 {
            _ori: Mat3x1 { _val: [ origin[0], origin[1], origin[2] ] },
            _bound: AxisAlignedBBox::init( ShapeType::POINT, &origin[0..3] ),
            _vicinity: 0.000001f64,
        }
    }
}

impl IShape for Point3 {
    fn get_shape_data( & self ) -> Vec< f64 > {
        vec![ self._ori[0], self._ori[1], self._ori[2] ]
    }
    fn get_type( & self ) -> ShapeType {
        ShapeType::POINT
    }
    fn get_bound( & self ) -> &IBound {
        &self._bound
    }
    // this shall test for intersection of bounding shapes first before procedding to test intersection using algorithms of higher complexity
    fn get_intersect( & self, other: & IShape ) -> ( bool, Option< Mat3x1< f64 > > ){
        if !self.get_bound().intersect( other.get_bound() ){
            return ( false, None )
        }else{
            match other.get_type() {
                ShapeType::POINT => {
                    let other_shape_data = other.get_shape_data();
                    if !self.within_vicinity( self._ori[0], other_shape_data[0] ) ||
                       !self.within_vicinity( self._ori[1], other_shape_data[1] ) ||
                       !self.within_vicinity( self._ori[2], other_shape_data[2] ) {
                        return ( false, None )
                    } else {
                        return ( true, Some( self._ori ) )
                    }
                },
                ShapeType::RAY => {
                    //see Ray3 for ray point intersection
                    other.get_intersect( self )
                },
                ShapeType::SPHERE => {
                    //see sphere for sphere point intersection
                    other.get_intersect( self )
                },
                ShapeType::PLANE => {
                    //see plane for plane point intersection
                    other.get_intersect( self )
                },
                _ => { unimplemented!(); },
            }
        }
    }
    fn get_support( & self, v: & Mat3x1< f64 > ) -> Option< Mat3x1< f64 > > {
        None
    }
}

impl IVicinity< f64 > for Point3 {
    fn set_vicinity( & mut self, epsilon: f64 ) {
        self._vicinity = epsilon.abs();
    }
    fn within_vicinity( & self, a: f64, b: f64 ) -> bool {
        if a + self._vicinity >= b &&
           a - self._vicinity <= b {
            true
        } else {
            false
        }
    }
}
