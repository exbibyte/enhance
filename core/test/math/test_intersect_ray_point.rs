use interface::i_shape::IShape;
use interface::i_comparable::IComparableError;

use implement::math::ray::Ray3;
use implement::math::point::Point3;
use implement::math::mat::Mat3x1;

#[test]
fn test_intersect_ray_point() {
    //ray point intersection
    {
        let a = Ray3::init( &[ 20f64, 0f64, 0f64 ], &[ 1f64, 1f64, 1f64 ] );
        let b = Point3::init( &[ 25f64, 5f64, 5f64 ] );
        match a.get_intersect( &b ) {
            ( true, Some(loc) ) => {
                assert!( loc.is_equal( &b._ori, 0.0001f64 ).unwrap() );
            },
            _ => panic!("unexpected result for ray point intersection" ),
        }
    }
    //ray point no intersection, point behind ray origin and direction
    {
        let a = Ray3::init( &[ 20f64, 0f64, 0f64 ], &[ 1f64, 1f64, 1f64 ] );
        let b = Point3::init( &[ 15f64, -5f64, -5f64 ] );
        match a.get_intersect( &b ) {
            ( false, None ) => (),
            _ => panic!("unexpected result for ray point no intersection, point behind ray" ),
        }
    }
    //ray point no intersection
    {
        let a = Ray3::init( &[ 20f64, 0f64, 0f64 ], &[ 1f64, 1f64, 1f64 ] );
        let b = Point3::init( &[ 25f64, 5f64, 5.1f64 ] );
        match a.get_intersect( &b ) {
            ( false, None ) => (),
            _ => panic!("unexpected result for ray point no intersection" ),
        }
    }


    //repeat the above tests but invoking the method on point
    //ray point intersection
    {
        let a = Ray3::init( &[ 20f64, 0f64, 0f64 ], &[ 1f64, 1f64, 1f64 ] );
        let b = Point3::init( &[ 25f64, 5f64, 5f64 ] );
        match b.get_intersect( &a ) {
            ( true, Some(loc) ) => {
                assert!( loc.is_equal( &b._ori, 0.0001f64 ).unwrap() );
            },
            _ => panic!("unexpected result for ray point intersection" ),
        }
    }
    //ray point no intersection, point behind ray origin and direction
    {
        let a = Ray3::init( &[ 20f64, 0f64, 0f64 ], &[ 1f64, 1f64, 1f64 ] );
        let b = Point3::init( &[ 15f64, -5f64, -5f64 ] );
        match b.get_intersect( &a ) {
            ( false, None ) => (),
            _ => panic!("unexpected result for ray point no intersection, point behind ray" ),
        }
    }
    //ray point no intersection
    {
        let a = Ray3::init( &[ 20f64, 0f64, 0f64 ], &[ 1f64, 1f64, 1f64 ] );
        let b = Point3::init( &[ 25f64, 5f64, 5.1f64 ] );
        match b.get_intersect( &a ) {
            ( false, None ) => (),
            _ => panic!("unexpected result for ray point no intersection" ),
        }
    }
}
