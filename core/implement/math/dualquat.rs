#[allow(unused_imports)]
use std::ops::Index;
#[allow(unused_imports)]
use std::ops::IndexMut;
#[allow(unused_imports)]
use std::ops::Div;
#[allow(unused_imports)]
use std::ops::Mul;

use implement::math::quat::Quat;

#[derive(Debug, Copy, Clone)]
pub struct DualQuat < T > {
    pub _rot: Quat< T >,
    pub _tra: Quat< T >,
}

macro_rules! define_dualquat {
    ($v_type: tt) => {
        
        impl DualQuat< $v_type > {
            #[allow(dead_code)]
            pub fn init( rotate: Quat< $v_type >, translate: Quat< $v_type > ) -> DualQuat< $v_type > {
                DualQuat { _rot: rotate, _tra: translate }
            }
        }
    }
}


define_dualquat!( f32 );
define_dualquat!( f64 );
