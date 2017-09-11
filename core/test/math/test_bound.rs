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

        assert!( aabb0.intersect( &aabb1 ) == true );
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

        assert!( aabb0.intersect( &aabb1 ) == true );
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

        assert!( aabb1.intersect( &aabb0 ) == true );
    }

    {
        let aabb0 = AxisAlignedBBox {
            _bound_lower: [ 0f64, 0f64, 0f64 ],
            _bound_upper: [ 10f64, 20f64, 30f64 ],
        };

        let aabb1 = AxisAlignedBBox {
            _bound_lower: [ 11f64, 21f64, 31f64 ],
            _bound_upper: [ 20f64, 30f64, 40f64 ],
        };

        assert!( aabb0.intersect( &aabb1 ) == false );
    }

    {
        let aabb0 = AxisAlignedBBox {
            _bound_lower: [ 0f64, 0f64, 0f64 ],
            _bound_upper: [ 10f64, 20f64, 30f64 ],
        };

        let aabb1 = AxisAlignedBBox {
            _bound_lower: [ 9f64, 19f64, 31f64 ],
            _bound_upper: [ 20f64, 30f64, 40f64 ],
        };

        assert!( aabb0.intersect( &aabb1 ) == false );
    }

    {
        let aabb0 = AxisAlignedBBox {
            _bound_lower: [ 0f64, 0f64, 0f64 ],
            _bound_upper: [ 10f64, 20f64, 30f64 ],
        };

        let aabb1 = AxisAlignedBBox {
            _bound_lower: [ 9f64, 19f64, 31f64 ],
            _bound_upper: [ 20f64, 30f64, 40f64 ],
        };

        assert!( aabb1.intersect( &aabb0 ) == false );
    }
    {

        let aabb0 = AxisAlignedBBox {
            _bound_lower: [ 0f64, -9f64, -18f64 ],
            _bound_upper: [ 10f64, 20f64, 40f64 ],
        };

        let aabb1 = AxisAlignedBBox {
            _bound_lower: [ 9f64, -20f64, 31f64 ],
            _bound_upper: [ 20f64, 30f64, 10f64 ],
        };
        
        let mut aabb_union : AxisAlignedBBox = Default::default();
        let bounds = [ &aabb0 as &IBound, &aabb1 as &IBound ];
        aabb_union.get_union( &bounds[..] );

        assert!( aabb_union._bound_lower[0] == 0f64 );
        assert!( aabb_union._bound_lower[1] == -20f64 );
        assert!( aabb_union._bound_lower[2] == -18f64 );
        
        assert!( aabb_union._bound_upper[0] == 20f64 );
        assert!( aabb_union._bound_upper[1] == 30f64 );
        assert!( aabb_union._bound_upper[2] == 40f64 );
    }
}
