use interface::i_bound::IBound;

/// acceleration interface for building and querying spatial data
pub trait ISpatialAccel {
    /// query for a list of objects within or partially inside the bound
    fn query( input: & IBound ) -> Vec< u64 >;
    /// build a acceleration structure with input bounds and object ids
    fn build( &[ (u64, &IBound) ] ) -> Result< (), & 'static str >;
}
