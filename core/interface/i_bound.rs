pub enum BoundType {
    AxisAlignBox,
    Sphere,
}

pub trait IBound {
    fn get_type( & self ) -> BoundType;
    fn intersect( & self, other: & Self ) -> bool;
    fn get_shortest_separation( & self, other: & Self ) -> f64;
    fn get_bound_data( &self ) -> [f64;32];
}
