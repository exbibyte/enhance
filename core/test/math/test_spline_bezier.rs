#[allow(unused_imports)]
use std::ops::Div;
#[allow(unused_imports)]
use std::cmp::Ordering;

#[allow(unused_imports)]
use implement::math::mat::Mat4x1;
#[allow(unused_imports)]
use interface::i_interpolate::IInterpolateMat4x1f64;

#[test]
fn test_spline(){
    use implement::math::spline::SplineBezier;
    let cp0 = Mat4x1 { _val: [ 0f64, 1f64, 2f64, 3f64 ] };
    let cp1 = Mat4x1 { _val: [ 5f64, 6f64, 7f64, 8f64 ] };
    let cp2 = Mat4x1 { _val: [ 10f64, 16f64, 17f64, 18f64 ] };
    let cp3 = Mat4x1 { _val: [ 0f64, 1f64, -2f64, -3f64 ] };
    
    let mut spline = SplineBezier::init( 10, cp0, cp1, cp2, cp3 );
    #[allow(unused_variables)]
    while !spline.interp_done() {
        let p = spline.interp_increment( 1 );
        println!("interp: {:?}", p );
    }
    #[allow(unused_variables)]
    for x in 0..3 {
        let p = spline.interp_increment( 1 );
        println!("{:?}", p );
    }
}

#[test]
fn test_spline_iterator(){
    
    use implement::math::spline::SplineBezier;
    let cp0 = Mat4x1 { _val: [ 0f64, 1f64, 2f64, 3f64 ] };
    let cp1 = Mat4x1 { _val: [ 5f64, 6f64, 7f64, 8f64 ] };
    let cp2 = Mat4x1 { _val: [ 10f64, 16f64, 17f64, 18f64 ] };
    let cp3 = Mat4x1 { _val: [ 0f64, 1f64, -2f64, -3f64 ] };
    {    
        let spline = SplineBezier::init( 10, cp0, cp1, cp2, cp3 );
        let count_interpolated = spline.count();
        assert!( count_interpolated == 10 );
    }
    {
        let spline = SplineBezier::init( 10, cp0, cp1, cp2, cp3 );
        for x in spline.enumerate().filter( |i| i.0 == 0 || i.0 == 9 ) {
            match x {
                (0,val) => {
                    assert!( val.is_equal( &cp0, 0.00001f64 ).expect("is_equal invalid") );
                },
                (9,val) => {
                    assert!( val.is_equal( &cp3, 0.00001f64 ).expect("is_equal invalid") );
                },
                _ => panic!("unexpected index")
                    
            }
        }
        
    }
}
