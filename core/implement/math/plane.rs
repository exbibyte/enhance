use interface::i_shape::{ ShapeType, IShape };
use interface::i_bound::IBound;
use interface::i_vicinity::IVicinity;

use implement::math::mat::Mat3x1;
use implement::math::bound::AxisAlignedBBox;

#[derive(Debug, Clone)]
pub struct Plane {
    pub _offset: Mat3x1< f64 >,
    pub _normal: Mat3x1< f64 >,
    pub _bound: AxisAlignedBBox,
    pub _vicinity: f64,
}

impl Plane {
    pub fn init( offset: &[f64], normal: &[f64] ) -> Plane {
        assert!( offset.len() == 3 );
        assert!( normal.len() == 3 );
        Plane {
            _offset: Mat3x1 { _val: [ offset[0], offset[1], offset[2] ] },
            _normal: Mat3x1 { _val: [ normal[0], normal[1], normal[2] ] }.normalize().unwrap(),
            _bound: AxisAlignedBBox::init( ShapeType::PLANE, &[ &offset[0..3], &normal[0..3] ].concat() ),
            _vicinity: 0.000001f64,
        }
    }
}

impl IShape for Plane {
    fn get_shape_data( & self ) -> Vec< f64 > {
        vec![ self._offset[0], self._offset[1], self._offset[2],
              self._normal[0], self._normal[1], self._normal[2], ]
    }
    fn get_type( & self ) -> ShapeType {
        ShapeType::PLANE
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
                ShapeType::PLANE => {
                    unimplemented!();
                },
                ShapeType::RAY => {
                    //see Ray3 for ray plane intersection
                    return other.get_intersect( self )
                },
                ShapeType::SPHERE => {
                    //see sphere for sphere plane intersection
                    return other.get_intersect( self )
                },
                ShapeType::POINT => {
                    let other_shape_data = other.get_shape_data();
                    let b_off = Mat3x1 { _val: [ other_shape_data[0], other_shape_data[1], other_shape_data[2] ] };
                    let k = self._normal.dot( &self._offset ).unwrap();
                    let c = self._normal.dot( &b_off ).unwrap();
                    let d = k - c;
                    if !self.within_vicinity( d, 0f64 ) {
                        return ( false, None )
                    }
                    return ( true, Some( b_off ) )
                },
                _ => { unimplemented!(); },
            }
        }
    }
}

impl IVicinity< f64 > for Plane {
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
