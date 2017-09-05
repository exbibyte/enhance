use interface::i_shape::{ ShapeType, IShape };
use interface::i_bound::IBound;
use interface::i_comparable::IComparableError;

use implement::math::mat::Mat3x1;
use implement::math::bound::AxisAlignedBBox;

#[derive(Debug, Clone)]
pub struct Ray3 {
    pub _ori: Mat3x1< f64 >,
    pub _dir: Mat3x1< f64 >,
    pub _bound: AxisAlignedBBox,
}

impl IShape for Ray3 {
    fn get_shape_data( & self ) -> Vec< f64 > {
        vec![ self._ori[0], self._ori[1], self._ori[2],
              self._dir[0], self._dir[1], self._dir[2] ]
    }
    fn get_type( & self ) -> ShapeType {
        ShapeType::RAY
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
                ShapeType::RAY => {
                    let other_shape_data = other.get_shape_data();
                    let a_dir = self._dir;
                    let b_dir = Mat3x1 { _val: [ other_shape_data[3], other_shape_data[4], other_shape_data[5] ] };
                    let a_off = self._ori;
                    let b_off = Mat3x1 { _val: [ other_shape_data[0], other_shape_data[1], other_shape_data[2] ] };

                    let c = b_dir.minus( &a_dir ).unwrap();
                    let v = a_dir.cross( &b_dir ).unwrap();

                    let error_thresh = 0.0001f64;

                    let dot_v_c = v.dot( &c ).unwrap();
                    if dot_v_c < -error_thresh &&
                       dot_v_c > error_thresh {
                        //they are not in the same place, so no intersection occurs
                        return ( false, None )
                    }
                    //test for colinearity
                    let zero : Mat3x1< f64 > = Default::default();
                    let d = b_off.minus( &a_off ).unwrap();
                    if v.is_equal( &zero, 0.0001f64 ).unwrap() {
                        //lines are parallel
                        //check triangle area formed by points on ray a and b
                        let point1 = a_dir;
                        let point2 = b_off.minus( &a_off ).unwrap();
                        let triangle_area = point1.cross( &point2 ).unwrap().magnitude().unwrap();
                        println!( "triangle area: {}", triangle_area );
                        if triangle_area < -error_thresh ||
                           triangle_area > error_thresh
                        {
                            //no overlap
                            println!( "parallel but non-overlapping lines" );
                            return ( false, None )
                        } else {
                            //lines are colinear
                            let direction = if d.dot( &a_dir ).unwrap() < 0f64 { -1f64 } else { 1f64 };
                            let distance = direction * d.magnitude().unwrap() / a_dir.magnitude().unwrap();
                            println!( "colinear lines, distance: {}", distance );
                            if distance < 0f64 {
                                //intersection at offset of ray a, so clamp t to 0
                                return ( true, Some( a_off ) )
                            } else {
                                //intersection at offset of ray b
                                return ( true, Some( self._dir.scale( distance ).unwrap().plus( &self._ori ).unwrap() ) )
                            }
                        }
                    } else {
                        //solvable intersection exists
                        let numerator = d.cross( & b_dir ).unwrap();
                        let t = numerator.magnitude().unwrap() / v.magnitude().unwrap();
                        if t < 0f64 {
                            return ( false, None )
                        } else {
                            return ( true, Some( self._dir.scale( t ).unwrap().plus( &self._ori ).unwrap() ) )
                        }
                    }
                },
                _ => { unimplemented!(); },
            }
        }
    }
}
