use interface::i_shape::IShape;
use interface::i_comparable::IComparableError;

use implement::math::sphere::Sphere;
use implement::math::point::Point3;

use implement::math::mat::Mat3x1;

#[test]
fn test_intersect_sphere_point() {
    //sphere point intersection
    {
        let a = Sphere::init( &[ 10f64, 0f64, 0f64 ], 5f64 );
        let b = Point3::init( &[ 8f64, 2f64, 3f64 ] );
        match a.get_intersect( &b ) {
            ( true, Some(loc) ) => {
                assert!( loc.is_equal( &Mat3x1{ _val: [ 8f64, 2f64, 3f64 ] }, 0.0001f64 ).unwrap() );
            },
            _ => panic!("unexpected result for ray point intersection" ),
        }
    }
    //sphere point intersection
    {
        let a = Sphere::init( &[ 10f64, 0f64, 0f64 ], 5f64 );
        let b = Point3::init( &[ 10f64, 5f64, 0f64 ] );
        match a.get_intersect( &b ) {
            ( true, Some(loc) ) => {
                assert!( loc.is_equal( &Mat3x1{ _val: [ 10f64, 5f64, 0f64 ] }, 0.0001f64 ).unwrap() );
            },
            _ => panic!("unexpected result for ray point intersection" ),
        }
    }
    //sphere point no intersection
    {
        let a = Sphere::init( &[ 10f64, 0f64, 0f64 ], 5f64 );
        let b = Point3::init( &[ 0f64, 5.1f64, 0f64 ] );
        match a.get_intersect( &b ) {
            ( false, None ) => (),
            _ => panic!("unexpected result for ray point no intersection" ),
        }
    }
}
