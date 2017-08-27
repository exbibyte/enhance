#[allow(unused_imports)]
use std::ops::Div;
#[allow(unused_imports)]
use std::cmp::Ordering;

#[allow(unused_imports)]
use implement::math::mat::Mat4x1;
#[allow(unused_imports)]
use interface::i_interpolate::IInterpolateMat4x1f64;

#[test]
fn test_linearinterp(){
    use implement::math::linearinterp::LinearInterp;
    let cp0 = Mat4x1 { _val: [ 0f64, 1f64, 2f64, 3f64 ] };
    let cp1 = Mat4x1 { _val: [ 10f64, 16f64, 17f64, 18f64 ] };
    
    let mut interp = LinearInterp::init( 10, cp0, cp1 );
    #[allow(unused_variables)]
    while !interp.interp_is_end() {
        let p = interp.interp_delta( 1 ).expect("forward interpolation result unexpected");
        println!("interp: {:?}", p );
    }
    //iteration end value check
    {
        let p = interp.interp_current();
        println!("interp: {:?}", p );
        assert!( p.is_equal( &cp1, 0.00001f64 ).expect( "iteration end value unexpected" ) );
    }
    //iteration stop condition check
    #[allow(unused_variables)]
    for x in 0..3 {
        let p = interp.interp_delta( 1 ).expect("forward interpolation result unexpected");
        println!("{:?}", p );
        assert!( p.is_equal( &cp1, 0.00001f64 ).expect( "iteration stop behaviour unexpected" ) );
    }
    //backward iteration
    while !interp.interp_is_start() {
        let p = interp.interp_delta( -1 ).expect("backward interpolation result unexpected");
        println!("interp: {:?}", p );
    }
    //iteration start value check
    {
        let p = interp.interp_current();
        assert!( p.is_equal( &cp0, 0.00001f64 ).expect( "iteration start value unexpected" ) );
    }
    //iteration stop condition check for back iteration
    #[allow(unused_variables)]
    for x in 0..3 {
        let p = interp.interp_delta( -1 ).expect("backward interpolation result unexpected");
        println!("{:?}", p );
        assert!( p.is_equal( &cp0, 0.00001f64 ).expect( "iteration stop behaviour unexpected" ) );
    }
}
