
pub trait IIntersect< Other, Result > {
    fn intersect( & mut self, other: & Other ) -> Option< Result >;
}
