use interface::i_bound::BoundType;
use interface::i_bound::IBound;

pub struct AxisAlignedBBox {
    pub _bound_lower: [ f64; 3 ],
    pub _bound_upper: [ f64; 3 ],
}

impl IBound for AxisAlignedBBox {
    fn get_type( & self ) -> BoundType {
        BoundType::AxisAlignBox
    }
    fn intersect( & self, other: & Self ) -> bool {
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
            _ => { unimplemented!(); return false },
        }
    }
    fn get_shortest_separation( & self, other: & Self ) -> f64 {
        unimplemented!{};
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
}
