use interface::i_shape::IShape;
use interface::i_comparable::IComparableError;

use implement::math::sphere::Sphere;

use implement::math::mat::Mat3x1;

#[test]
fn test_intersect_sphere_sphere() {
    //Sphere Sphere intersection
    {
        let a = Sphere::init( &[ 10f64, 0f64, 0f64 ], 5f64 );
        let b = Sphere::init( &[ 20f64, 0f64, 0f64 ], 5f64 );
        match a.get_intersect( &b ) {
            ( true, Some(loc) ) => {
                assert!( loc.is_equal( &Mat3x1 { _val: [ 15f64, 0f64, 0f64 ] }, 0.0001f64 ).unwrap() );
            },
            _ => panic!("unexpected result for ray sphere intersection" ),
        }
    }
    //Sphere Sphere intersection
    {
        let a = Sphere::init( &[ 10f64, 0f64, 0f64 ], 5f64 );
        let b = Sphere::init( &[ 13f64, 4f64, 0f64 ], 5f64 );
        match a.get_intersect( &b ) {
            ( true, Some(loc) ) => {
                println!( "loc: {:?}", loc );
                assert!( loc.is_equal( &Mat3x1 { _val: [ 11.5f64, 2f64, 0f64 ] }, 0.0001f64 ).unwrap() );
            },
            _ => panic!("unexpected result for ray sphere intersection" ),
        }
    }

    //Sphere Sphere no intersection
    {
        let a = Sphere::init( &[ 10f64, 0f64, 0f64 ], 5f64 );
        let b = Sphere::init( &[ 20f64, 0.1f64, 0f64 ], 5f64 );
        match a.get_intersect( &b ) {
            ( false, None ) => (),
            _ => panic!("unexpected result for ray sphere intersection" ),
        }
    }
}
