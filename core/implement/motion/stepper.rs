use interface::i_interpolate::IInterpolateMat4x1f64;
use implement::math::spline;
use implement::math::mat;

pub struct Stepper {
    pub _current_val: f64,
    pub _range_val: ( f64, f64 ),
}

pub fn step_spline_delta< T >( s: & mut Stepper, step_delta: f64, spline: & mut spline::SplinePiecewise< T > ) -> mat::Mat4x1< f64 >
where T : IInterpolateMat4x1f64
{
    if s._current_val >= s._range_val.1 {
        return spline.interp_current()
    }
    //clamp values to constraints
    let a = s._current_val + step_delta;
    let b = if a < s._range_val.0 {
        s._range_val.0
    }else if a > s._range_val.1 {
        s._range_val.1
    }else {
        a
    };
    let c = b - s._range_val.0;
    let total_steps = spline._total_steps;
    let fraction = c / ( s._range_val.1 - s._range_val.0 );
    let calc_steps = ( fraction * total_steps as f64 ) as u64;
    let mut ret = spline.interp_current();
    s._current_val = if s._current_val + c > s._range_val.1 {
        s._range_val.1
    } else {
        s._current_val + c
    };
    for i in 0..calc_steps as usize - 1 {
        match spline.next() {
            None => { break; },
            Some( o ) => { ret = spline.interp_current();
                           // println!("iteration val: {:?}", ret );
            },
        }
    }
    ret
}

// // pub fn step_spline_abs( s: & Stepper, step_abs: f64 ) -> Mat4x1< f64 > {
    
// // }
