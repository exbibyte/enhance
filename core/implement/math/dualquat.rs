#[allow(unused_imports)]
use std::ops::Index;
#[allow(unused_imports)]
use std::ops::IndexMut;
#[allow(unused_imports)]
use std::ops::Div;
#[allow(unused_imports)]
use std::ops::Mul;

use std::f64::consts::PI;

use implement::math::mat::Mat3x1;
use implement::math::mat::Mat4;
use implement::math::quat::Quat;

#[derive(Debug, Copy, Clone)]
pub struct DualQuat {
    pub _rot: Quat,
    pub _tra: Quat,
}
impl DualQuat {
    #[allow(dead_code)]
    pub fn init( rotate: Quat, translate: Quat ) -> DualQuat {
        DualQuat { _rot: rotate, _tra: translate }
    }
}
