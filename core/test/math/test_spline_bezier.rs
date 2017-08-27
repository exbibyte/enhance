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
    while !spline.interp_is_end() {
        let p = spline.interp_delta( 1 ).expect("forward interpolation result unexpected");
        println!("interp: {:?}", p );
    }
    //iteration end value check
    {
        let p = spline.interp_current();
        assert!( p.is_equal( &cp3, 0.00001f64 ).expect( "iteration end value unexpected" ) );
    }
    //iteration stop condition check
    #[allow(unused_variables)]
    for x in 0..3 {
        let p = spline.interp_delta( 1 ).expect("forward interpolation result unexpected");
        println!("{:?}", p );
        assert!( p.is_equal( &cp3, 0.00001f64 ).expect( "iteration stop behaviour unexpected" ) );
    }
    // //backward iteration
    // while !spline.interp_is_start() {
    //     let p = spline.interp_decrement( 1 );
    //     println!("interp: {:?}", p );
    // }
    // //iteration start value check
    // {
    //     let p = spline.interp_current();
    //     assert!( p.is_equal( &cp0, 0.00001f64 ).expect( "iteration start value unexpected" ) );
    // }
    // //iteration stop condition check for back iteration
    // #[allow(unused_variables)]
    // for x in 0..3 {
    //     let p = spline.interp_decrement( 1 );
    //     println!("{:?}", p );
    //     assert!( p.is_equal( &cp0, 0.00001f64 ).expect( "iteration stop behaviour unexpected" ) );
    // }
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

#[test]
fn test_spline_piecewise_bezier(){
    use implement::math::spline::SplineBezier;
    use implement::math::spline::SplinePiecewise;
    let mut splines = SplinePiecewise::init();
    let cp0 = Mat4x1 { _val: [ 0f64, 1f64, 2f64, 3f64 ] };
    let cp1 = Mat4x1 { _val: [ 5f64, 6f64, 7f64, 8f64 ] };
    let cp2 = Mat4x1 { _val: [ 10f64, 16f64, 17f64, 18f64 ] };
    let cp3 = Mat4x1 { _val: [ 0f64, 1f64, -2f64, -3f64 ] };
    for x in 0..4 {
        let spline = SplineBezier::init( 10 * (x+1) , cp0, cp1, cp2, cp3 );
        splines.add( spline );
    }
    assert!(splines._splines.len() == 4 );
    let mut max_index = 0usize;
    for (i, x) in splines.enumerate() {
        max_index = i;
        match i {
            0 => assert!( x.is_equal( &cp0, 0.00001f64 ).expect("is_equal invalid") ),
            9 => assert!( x.is_equal( &cp3, 0.00001f64 ).expect("is_equal invalid") ),
            10 => assert!( x.is_equal( &cp0, 0.00001f64 ).expect("is_equal invalid") ),
            29 => assert!( x.is_equal( &cp3, 0.00001f64 ).expect("is_equal invalid") ),
            30 => assert!( x.is_equal( &cp0, 0.00001f64 ).expect("is_equal invalid") ),
            59 => assert!( x.is_equal( &cp3, 0.00001f64 ).expect("is_equal invalid") ),
            60 => assert!( x.is_equal( &cp0, 0.00001f64 ).expect("is_equal invalid") ),
            99 => assert!( x.is_equal( &cp3, 0.00001f64 ).expect("is_equal invalid") ),
            _ => ()
        }
    }
    assert!( max_index == 99 );
}

