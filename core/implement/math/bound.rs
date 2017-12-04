use std::f64;
use std::cmp;

use interface::i_bound::BoundType;
use interface::i_bound::IBound;
use interface::i_shape::ShapeType;

#[derive(Debug)]
#[derive(Clone)]
pub struct AxisAlignedBBox {
    pub _bound_lower: [ f64; 3 ],
    pub _bound_upper: [ f64; 3 ],
}

#[derive(Debug)]
#[derive(Clone)]
pub enum Axis {
    X,
    Y,
    Z,
}

impl AxisAlignedBBox {
    pub fn init( shape_type: ShapeType, vals: &[f64] ) -> AxisAlignedBBox {
        match shape_type {
            ShapeType::RAY => {
                assert!( vals.len() == 6 );
                let mut bounds = [ (0f64,0f64); 3 ];
                for i in 0..3 {
                    let b = if vals[3+i] > 0f64 {
                        ( vals[i], f64::INFINITY )
                    } else if vals[3+i] < 0f64 {
                        ( f64::NEG_INFINITY, vals[i] )
                    } else {
                        ( vals[i], vals[i] )
                    };
                    bounds[i] = b;
                }
                AxisAlignedBBox {
                    _bound_lower: [ bounds[0].0, bounds[1].0, bounds[2].0 ],
                    _bound_upper: [ bounds[0].1, bounds[1].1, bounds[2].1 ],
                }
            },
            ShapeType::POINT => {
                assert!( vals.len() == 3 );
                AxisAlignedBBox {
                    _bound_lower: [ vals[0], vals[1], vals[2] ],
                    _bound_upper: [ vals[0], vals[1], vals[2] ],
                }
            },
            ShapeType::SPHERE => {
                assert!( vals.len() == 4 );
                AxisAlignedBBox {
                    _bound_lower: [ vals[0] - vals[3], vals[1] - vals[3], vals[2] - vals[3] ],
                    _bound_upper: [ vals[0] + vals[3], vals[1] + vals[3], vals[2] + vals[3] ],
                }
            },
            ShapeType::PLANE => {
                assert!( vals.len() == 6 );
                AxisAlignedBBox {
                    _bound_lower: [ f64::NEG_INFINITY; 3 ],
                    _bound_upper: [ f64::INFINITY; 3 ],
                }
            },
            ShapeType::FRUSTUM => {
                unimplemented!();
            },
            _ => { unimplemented!(); },
        }
    }
    pub fn get_longest_axis( &self ) -> ( Axis, f64 ){
        let dx = ( Axis::X, self._bound_upper[0] - self._bound_lower[0] );
        let dy = ( Axis::Y, self._bound_upper[1] - self._bound_lower[1] );
        let dz = ( Axis::Z, self._bound_upper[2] - self._bound_lower[2] );
        let longest = [ dx, dy, dz ]
            .into_iter()
            .cloned()
            .max_by( |x, y|
                      if x.1 < y.1 {
                          cmp::Ordering::Less
                      } else if x.1 < y.1 {
                          cmp::Ordering::Greater
                      } else {
                          cmp::Ordering::Equal
                      }
            ).unwrap();
        longest
    }
}

impl IBound for AxisAlignedBBox {
    fn get_type( & self ) -> BoundType {
        BoundType::AxisAlignBox
    }
    fn intersect( & self, other: & IBound ) -> bool {
        match other.get_type(){
            BoundType::AxisAlignBox => {
                let a_bounds = self.get_bound_data();
                let b_bounds = other.get_bound_data();

                let a_lower = &a_bounds[0..3];
                let a_upper = &a_bounds[3..6];
                let b_lower = &b_bounds[0..3];
                let b_upper = &b_bounds[3..6];

                for i in 0..3 {
                    if a_lower[i] > b_upper[i] ||
                       a_upper[i] < b_lower[i] {
                        return false
                    }
                }
                return true
                
            },
            _ => { unimplemented!(); },
        }
    }
    fn get_shortest_separation( & self, _other: & IBound ) -> f64 {
        unimplemented!();
    }
    fn get_bound_data( &self ) -> [f64;32] {
        let mut arr = [0f64;32];
        for i in 0..3 {
            arr[i] = self._bound_lower[i];
        }
        for i in 0..3 {
            arr[i+3] = self._bound_upper[i];
        }
        arr
    }
    fn get_union( & mut self, bounds: &[ &IBound ] ) {
        self._bound_lower = [ f64::INFINITY; 3 ];
        self._bound_upper = [ f64::NEG_INFINITY; 3 ];
        for i in bounds {
            match i.get_type(){
                BoundType::AxisAlignBox => (),
                _ => { unimplemented!(); },
            }
            let b = i.get_bound_data();
            let b_lower = &b[0..3];
            let b_upper = &b[3..6];
            for j in 0..3 {
                self._bound_lower[j] = self._bound_lower[j].min( b_lower[j] );
                self._bound_upper[j] = self._bound_upper[j].max( b_upper[j] );
            }
        }
    }
    fn get_centroid( & self ) -> [ f64; 3 ] {
        match self.get_type(){
            BoundType::AxisAlignBox => {
                let b = self.get_bound_data();
                let b_lower = &b[0..3];
                let b_upper = &b[3..6];
                return [ ( b_lower[0] + b_upper[0] ) / 2f64,
                         ( b_lower[1] + b_upper[1] ) / 2f64,
                         ( b_lower[2] + b_upper[2] ) / 2f64, ]
            },
            _ => { unimplemented!(); },
        }
    }
}

impl Default for AxisAlignedBBox {
    fn default() -> AxisAlignedBBox {
        AxisAlignedBBox {
            _bound_lower: [ f64::NEG_INFINITY; 3 ],
            _bound_upper: [ f64::INFINITY; 3 ],
        }
    }
}
