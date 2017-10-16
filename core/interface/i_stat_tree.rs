pub trait IStatTree {
    fn sum_subtree_child_count( & self ) -> Option< u64 >;
    fn node_degree( & self ) -> Option< u64 >;
    fn max_depth( & self ) -> Option< u64 >;
    fn mean_depth( & self ) -> Option< u64 >;
    fn min_depth( & self ) -> Option< u64 >;
    fn balance_ratio( & self ) -> Option< f64 >;
}
