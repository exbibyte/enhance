use std::f64;

use interface::i_bound::BoundType;
use interface::i_bound::IBound;
use interface::i_shape::ShapeType;

#[derive(Debug)]
#[derive(Clone)]
pub struct BoundSphere {
    //todo
}

impl BoundSphere {
    pub fn init( _shape_type: ShapeType, _vals: &[f64] ) -> BoundSphere {
        BoundSphere {}
    }
}

impl IBound for BoundSphere {
    fn get_type( & self ) -> BoundType {
        BoundType::Sphere
    }
    fn intersect( & self, _other: & IBound ) -> bool {
        unimplemented!();
    }
    fn get_shortest_separation( & self, _other: & IBound ) -> f64 {
        unimplemented!();
    }
    fn get_bound_data( &self ) -> [f64;32] {
        unimplemented!();
    }
    fn get_union( & mut self, _bounds: &[ &IBound ] ) {
        unimplemented!();
    }
    fn get_centroid( & self ) -> [ f64; 3 ] {
        unimplemented!();
    }
}

impl Default for BoundSphere {
    fn default() -> BoundSphere {
        unimplemented!();
    }
}
