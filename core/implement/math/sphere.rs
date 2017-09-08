use interface::i_shape::{ ShapeType, IShape };
use interface::i_bound::IBound;
use interface::i_vicinity::IVicinity;

use implement::math::mat::Mat3x1;
use implement::math::bound::AxisAlignedBBox;

#[derive(Debug, Clone)]
pub struct Sphere {
    pub _ori: Mat3x1< f64 >,
    pub _radius: f64,
    pub _bound: AxisAlignedBBox,
    pub _vicinity: f64,
}

impl Sphere {
    pub fn init( origin: &[f64], r: f64 ) -> Sphere {
        assert!( origin.len() == 3 );
        Sphere {
            _ori: Mat3x1 { _val: [ origin[0], origin[1], origin[2] ] },
            _radius: r,
            _bound: AxisAlignedBBox::init( ShapeType::SPHERE, &[ &origin[0..3], &[ r ] ].concat() ),
            _vicinity: 0.000001f64,
        }
    }
}

impl IShape for Sphere {
    fn get_shape_data( & self ) -> Vec< f64 > {
        vec![ self._ori[0], self._ori[1], self._ori[2], self._radius ]
    }
    fn get_type( & self ) -> ShapeType {
        ShapeType::SPHERE
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
                ShapeType::SPHERE => {
                    let other_shape_data = other.get_shape_data();
                    let b_off = Mat3x1 { _val: [ other_shape_data[0], other_shape_data[1], other_shape_data[2] ] };
                    let a_r = self._radius;
                    let b_r = other_shape_data[3];

                    let a_off = self._ori;
                    let c = b_off.minus( &a_off ).unwrap();
                    let d = c.magnitude().unwrap();
                    if d > b_r + a_r {
                        return ( false, None )
                    } else {
                        //calculate a mid point average
                        let f = a_r / ( a_r + b_r );
                        let g = c.scale( f ).unwrap();
                        return ( true, Some( a_off.plus( &g ).unwrap() ) )
                    }
                },
                ShapeType::RAY => {
                    //see Ray3 for ray sphere intersection
                    return other.get_intersect( self )
                },
                ShapeType::POINT => {
                    let other_shape_data = other.get_shape_data();
                    let b_off = Mat3x1 { _val: [ other_shape_data[0], other_shape_data[1], other_shape_data[2] ] };
                    let d = b_off.minus( &self._ori ).unwrap();
                    for i in 0..3 {
                        if d[i] > self._radius {
                            return ( false, None )
                        }
                    }
                    return ( true, Some( b_off ) )
                },
                ShapeType::PLANE => {
                    let other_shape_data = other.get_shape_data();
                    let b_off = Mat3x1 { _val: [ other_shape_data[0], other_shape_data[1], other_shape_data[2] ] };
                    let b_nor = Mat3x1 { _val: [ other_shape_data[3], other_shape_data[4], other_shape_data[5] ] };
                    //x = -plane_normal * t + sphere_center
                    //dot( plane_normal, x ) = dot( plane_normal, plane_offset ) = k
                    //substitution:
                    //dot( plane_normal, -plane_normal * t + sphere_center ) = k
                    //-t + dot( plane_normal, sphere_center ) = k
                    //t = dot( plane_normal, sphere_center ) - k

                    let k = b_nor.dot( &b_off ).unwrap();
                    let t = b_nor.dot( &self._ori ).unwrap() - k;
                    if t > self._radius {
                        return ( false, None )
                    } else {
                        return ( true, Some( b_nor.scale( -t ).unwrap().plus( &self._ori ).unwrap() ) )
                    }
                },
                _ => { unimplemented!(); },
            }
        }
    }
}

impl IVicinity< f64 > for Sphere {
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
