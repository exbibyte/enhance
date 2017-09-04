use interface::i_bound::IBound;
use implement::math::bound::AxisAlignedBBox;

#[test]
fn test_bound_aabb(){

    {
        let aabb0 = AxisAlignedBBox {
            _bound_lower: [ 0f64, 0f64, 0f64 ],
            _bound_upper: [ 10f64, 20f64, 30f64 ],
        };

        let aabb1 = AxisAlignedBBox {
            _bound_lower: [ 0f64, 0f64, 0f64 ],
            _bound_upper: [ 10f64, 20f64, 30f64 ],
        };

        assert!( aabb0.intersect( &aabb1 ), true );
    }
    {
        let aabb0 = AxisAlignedBBox {
            _bound_lower: [ 0f64, 0f64, 0f64 ],
            _bound_upper: [ 10f64, 20f64, 30f64 ],
        };

        let aabb1 = AxisAlignedBBox {
            _bound_lower: [ 10f64, 20f64, 30f64 ],
            _bound_upper: [ 20f64, 30f64, 40f64 ],
        };

        assert!( aabb0.intersect( &aabb1 ), true );
    }
}
