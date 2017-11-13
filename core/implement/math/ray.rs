use interface::i_shape::{ ShapeType, IShape };
use interface::i_bound::IBound;
use interface::i_vicinity::IVicinity;
use interface::i_comparable::IComparableError;

use implement::math::mat::Mat3x1;
use implement::math::bound::AxisAlignedBBox;

#[derive(Debug, Clone)]
pub struct Ray3 {
    pub _ori: Mat3x1< f64 >,
    pub _dir: Mat3x1< f64 >,
    pub _bound: AxisAlignedBBox,
    pub _vicinity: f64,
}

impl Ray3 {
    pub fn init( origin: &[f64], dir: &[f64] ) -> Ray3 {
        assert!( origin.len() == 3 );
        assert!( dir.len() == 3 );
        Ray3 {
            _ori: Mat3x1 { _val: [ origin[0], origin[1], origin[2] ] },
            _dir: Mat3x1 { _val: [ dir[0], dir[1], dir[2] ] }.normalize().unwrap(),
            _bound: AxisAlignedBBox::init( ShapeType::RAY, &[ &origin[0..3], &dir[0..3] ].concat() ),
            _vicinity: 0.000001f64,
        }
    }
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

                    let dot_v_c = v.dot( &c ).unwrap();
                    if !self.within_vicinity( dot_v_c, 0f64 ) {
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
                        // println!( "triangle area: {}", triangle_area );
                        if !self.within_vicinity( triangle_area, 0f64 ) {
                            //no overlap
                            // println!( "parallel but non-overlapping lines" );
                            return ( false, None )
                        } else {
                            //lines are colinear
                            let direction = if d.dot( &a_dir ).unwrap() < 0f64 { -1f64 } else { 1f64 };
                            let distance = direction * d.magnitude().unwrap() / a_dir.magnitude().unwrap();
                            // println!( "colinear lines, distance: {}", distance );
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
                ShapeType::POINT => {
                    let other_shape_data = other.get_shape_data();
                    let b_off = Mat3x1 { _val: [ other_shape_data[0], other_shape_data[1], other_shape_data[2] ] };
                    let a_dir = self._dir;
                    let a_off = self._ori;
                    //a_dir * t + a_off = b_off
                    //t = (b_off - a_off) / a_dir
                    let t = b_off.minus( &a_off ).unwrap().div( &a_dir ).unwrap();
                    if !self.within_vicinity( t[0], t[1] ) ||
                       !self.within_vicinity( t[1], t[2] ) {
                           return ( false, None )
                    } else {
                        if t[0] >= 0f64 {
                            return ( true, Some( a_dir.scale( t[0] ).unwrap().plus( &a_off ).unwrap() ) )
                        } else {
                            //the point is behind the ray origin and direction
                            return ( false, None )
                        }
                    }    
                },
                ShapeType::SPHERE => {
                    let other_shape_data = other.get_shape_data();
                    let b_off = Mat3x1 { _val: [ other_shape_data[0], other_shape_data[1], other_shape_data[2] ] };
                    let b_r = other_shape_data[3];

                    let a_dir = self._dir;
                    let a_off = self._ori;

                    //sub in the ray equation into sphere equation
                    // b := projection of relative offset onto ray direction
                    // c := (minimal possible distance between sphere and ray origin )^2
                    let relative_offset = a_off.minus( &b_off ).unwrap();
                    let b = relative_offset.dot( &a_dir ).unwrap();
                    let c = relative_offset.dot( &relative_offset ).unwrap() - b_r * b_r;

                    if b > 0f64 && c > 0f64 {
                        //ray is outside of the sphere and points away from sphere
                        //thus no intersection occurs
                        return ( false, None )
                    }

                    let d = b * b - c;
                    if d < 0f64 {
                        //ray misses sphere
                        return ( false, None )
                    }

                    let t1 = -b - d.sqrt();
                    let t2 = -b + d.sqrt();

                    let t = if t1 < 0f64 {
                        t2
                    } else if t2 < 0f64 {
                        t1
                    } else if t1 < t2 {
                        t1
                    } else {
                        t2
                    };
                    
                    return ( true, Some( a_dir.scale( t ).unwrap().plus( &a_off ).unwrap() ) )
                },
                ShapeType::PLANE => {
                    let other_shape_data = other.get_shape_data();
                    let b_off = Mat3x1 { _val: [ other_shape_data[0], other_shape_data[1], other_shape_data[2] ] };
                    let b_nor = Mat3x1 { _val: [ other_shape_data[3], other_shape_data[4], other_shape_data[5] ] };
                    //ray equation: r(t) = r.offset + r.dir * t
                    //plane: p(x) = dot(normal, x-p.offset) = 0
                    //p(x) = -dot(p.normal, p.offset) + dot(p.normal, x) = 0
                    //substitution:
                    // p(t) = -dot(p.fofset,p.normal) + dot(p.normal, r.offset + r.dir*t) = 0
                    //      = -dot(p.fofset,p.normal) + dot(p.normal, r.offset) + t*dot(p.normal, r.dir) = 0
                    //t = ( dot(p.offset, p.normal) - dot(p.normal, r.offset) )/ dot(p.normal, r.dir )
                    let constant = b_off.dot( &b_nor ).unwrap();
                    let numerator = constant - b_nor.dot( &self._ori ).unwrap();
                    let denominator = b_nor.dot( &self._dir ).unwrap();
                    if denominator == 0f64 {
                        //ray direction is colplaner to the plane
                        if constant == self._ori.dot( &b_nor ).unwrap() {
                            return ( true, Some( self._ori ) )
                        } else {
                            return ( false, None )
                        }
                    } else if denominator > 0f64 {
                        //ray direction is not facing plane normal
                        return ( false, None )
                    }
                    let t = numerator / denominator;
                    if t < 0f64 {
                        return ( false, None )
                    }
                    return ( true, Some( self._dir.scale( t ).unwrap().plus( &self._ori ).unwrap() ) )
                },
                _ => { unimplemented!(); },
            }
        }
    }
    fn get_support( & self, v: & Mat3x1< f64 > ) -> Option< Mat3x1< f64 > > {
        None
    }
}

impl IVicinity< f64 > for Ray3 {
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
