use interface::i_shape::IShape;
use interface::i_comparable::IComparableError;

use implement::math::point::Point3;

#[test]
fn test_intersect_point_point() {
    //point point intersection
    {
        let a = Point3::init( &[ 25f64, 5f64, 5f64 ] );
        let b = Point3::init( &[ 25f64, 5f64, 5f64 ] );
        match a.get_intersect( &b ) {
            ( true, Some(loc) ) => {
                assert!( loc.is_equal( &b._ori, 0.0001f64 ).unwrap() );
            },
            _ => panic!("unexpected result for ray point intersection" ),
        }
    }
    //point point no intersection
    {
        let a = Point3::init( &[ 25f64, 5f64, 5f64 ] );
        let b = Point3::init( &[ 25.1f64, 5f64, 5f64 ] );
        match a.get_intersect( &b ) {
            ( false, None ) => (),
            _ => panic!("unexpected result for ray point intersection" ),
        }
    }
}
