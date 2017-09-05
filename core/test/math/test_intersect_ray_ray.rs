use interface::i_shape::IShape;
use interface::i_comparable::IComparableError;

use implement::math::ray::Ray3;
use implement::math::mat::Mat3x1;
use implement::math::bound::AxisAlignedBBox;

#[test]
fn test_intersect_ray_ray() {
    //parallel rays, no intersection
    {
        let a = Ray3 { _ori: Mat3x1 { _val: [ 20f64, 0f64, 0f64 ] },
                       _dir: Mat3x1 { _val: [ 1f64, 1f64, 1f64 ] },
                       _bound: AxisAlignedBBox { _bound_lower: [ 20f64, 0f64, 0f64 ],
                                                 _bound_upper: [ 99999f64; 3 ], } };

        let b = Ray3 { _ori: Mat3x1 { _val: [ 25f64, 0f64, 0f64 ] },
                       _dir: Mat3x1 { _val: [ 1f64, 1f64, 1f64 ] },
                       _bound: AxisAlignedBBox { _bound_lower: [ 25f64, 0f64, 0f64 ],
                                                 _bound_upper: [ 99999f64; 3 ], } };
        match a.get_intersect( &b ) {
            ( false, None ) => (),
            _ => panic!("unexpected result for parallel rays, no intersection" ),
        }
    }

    //colinear rays, intersection
    {
        let a = Ray3 { _ori: Mat3x1 { _val: [ 20f64, 0f64, 0f64 ] },
                       _dir: Mat3x1 { _val: [ 1f64, 1f64, 1f64 ] },
                       _bound: AxisAlignedBBox { _bound_lower: [ 20f64, 0f64, 0f64 ],
                                                 _bound_upper: [ 99999f64; 3 ], } };

        let b = Ray3 { _ori: Mat3x1 { _val: [ 22f64, 2f64, 2f64 ] },
                       _dir: Mat3x1 { _val: [ 1f64, 1f64, 1f64 ] },
                       _bound: AxisAlignedBBox { _bound_lower: [ 22f64, 2f64, 2f64 ],
                                                 _bound_upper: [ 99999f64; 3 ], } };
        match a.get_intersect( &b ) {
            ( true, Some(loc) ) => {
                assert!( loc.is_equal( &b._ori, 0.0001f64 ).unwrap() );
            },
            _ => panic!("unexpected result for parallel rays, no intersection" ),
        }
    }

    //colinear rays, intersection
    {
        let a = Ray3 { _ori: Mat3x1 { _val: [ 25f64, 5f64, 5f64 ] },
                       _dir: Mat3x1 { _val: [ 1f64, 1f64, 1f64 ] },
                       _bound: AxisAlignedBBox { _bound_lower: [ 25f64, 5f64, 5f64 ],
                                                 _bound_upper: [ 99999f64; 3 ], } };

        let b = Ray3 { _ori: Mat3x1 { _val: [ 22f64, 2f64, 2f64 ] },
                       _dir: Mat3x1 { _val: [ 1f64, 1f64, 1f64 ] },
                       _bound: AxisAlignedBBox { _bound_lower: [ 22f64, 2f64, 2f64 ],
                                                 _bound_upper: [ 99999f64; 3 ], } };
        match a.get_intersect( &b ) {
            ( true, Some(loc) ) => {
                assert!( loc.is_equal( &a._ori, 0.0001f64 ).unwrap() );
            },
            _ => panic!("unexpected result for parallel rays, no intersection" ),
        }
    }

    //rays, intersection
    {
        let a = Ray3 { _ori: Mat3x1 { _val: [ 5f64, 5f64, 0f64 ] },
                       _dir: Mat3x1 { _val: [ -1f64, 0f64, 0f64 ] },
                       _bound: AxisAlignedBBox { _bound_lower: [ -99999f64, 5f64, 0f64 ],
                                                 _bound_upper: [ 5f64, 5f64, 0f64 ], } };

        let b = Ray3 { _ori: Mat3x1 { _val: [ 0f64, 0f64, 0f64 ] },
                       _dir: Mat3x1 { _val: [ 0f64, 1f64, 0f64 ] },
                       _bound: AxisAlignedBBox { _bound_lower: [ 0f64, 0f64, 0f64 ],
                                                 _bound_upper: [ 0f64, 99999f64, 0f64 ], } };
        match a.get_intersect( &b ) {
            ( true, Some(loc) ) => {
                assert!( loc.is_equal( & Mat3x1{ _val: [ 0f64, 5f64, 0f64 ] }, 0.0001f64 ).unwrap() );
            },
            _ => panic!("unexpected result for ray intersection" ),
        }
    }

    //non-coplaner rays, no intersection
    {
        let a = Ray3 { _ori: Mat3x1 { _val: [ 5f64, 5f64, 2f64 ] },
                       _dir: Mat3x1 { _val: [ -1f64, -1f64, 0f64 ] },
                       _bound: AxisAlignedBBox { _bound_lower: [ -99999f64, -99999f64, 2f64 ],
                                                 _bound_upper: [ 5f64, 5f64, 2f64 ], } };

        let b = Ray3 { _ori: Mat3x1 { _val: [ 5f64, 5f64, 0f64 ] },
                       _dir: Mat3x1 { _val: [ 1f64, 1f64, 0f64 ] },
                       _bound: AxisAlignedBBox { _bound_lower: [ 5f64, 5f64, 0f64 ],
                                                 _bound_upper: [ 99999f64, 99999f64, 0f64 ], } };
        match a.get_intersect( &b ) {
            ( false, None ) => (),
            _ => panic!("unexpected result for ray intersection" ),
        }
    }
    
}
