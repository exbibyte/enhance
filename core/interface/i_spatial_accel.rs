use interface::i_bound::IBound;

/// acceleration interface for building and querying spatial data
pub trait ISpatialAccel {
    /// query for a list of objects intersecting with input
    fn query_intersect( & self, input: &IBound ) -> Result< Vec< u64 >, & 'static str >;
    /// build a acceleration structure with input bounds and object ids
    fn build_all( & mut self, &[ (u64, &IBound) ] ) -> Result< (), & 'static str >;
}
