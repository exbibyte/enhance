#[cfg(test)]
#[allow(unused_imports)]
use implement::motion::stepper;

#[allow(unused_imports)]
use implement::math::mat::Mat4x1;
#[allow(unused_imports)]
use interface::i_interpolate::IInterpolateMat4x1f64;

#[allow(unused_imports)]
use implement::math::spline::SplineBezier;

#[allow(unused_imports)]
use implement::math::spline::SplinePiecewise;

#[test]
fn test_stepper(){

    let mut splines = SplinePiecewise::init();
    let cp0 = Mat4x1 { _val: [ 0f64, 1f64, 2f64, 3f64 ] };
    let cp1 = Mat4x1 { _val: [ 5f64, 6f64, 7f64, 8f64 ] };
    let cp2 = Mat4x1 { _val: [ 10f64, 16f64, 17f64, 18f64 ] };
    let cp3 = Mat4x1 { _val: [ 0f64, 1f64, -2f64, -3f64 ] };
    for x in 0..2 {
        let spline = SplineBezier::init( 10 , cp0, cp1, cp2, cp3 );
        splines.add( spline );
    }
    assert!(splines._splines.len() == 2 );

    let mut s = stepper::Stepper {
        _current_val: 10.0,
        _range_val: ( 10.0, 20.0 ),
    };

    let step_delta = 5f64;

    // for i in 0..5 {
        stepper::step_spline_delta( & mut s, step_delta, & mut splines );
    // }
    let val = splines.interp_current();
    println!("interp: {:?}", val );
    assert!( val.is_equal( &cp3, 0.00001f64 ).expect("is_equal invalid") );
}
