use std::collections::HashMap;
use std::vec::Vec;

use interface::i_renderobj;

use implement::render::rendercommon;
use implement::render::camera;
use implement::math;

pub struct Cam {
    /// # helper id for the camera
    pub _id: u64,

    pub _proj_xform: math::mat::Mat4< f32 >,
    /// # The following generates the projection matrix
    pub _fov: f32,
    pub _aspect: f32,
    pub _near: f32,
    pub _far: f32,

    pub _view_xform: math::mat::Mat4< f32 >,
    /// # The following generates the view matrix
    pub _pos: math::mat::Mat3x1< f32 >,
    pub _up: math::mat::Mat3x1< f32 >,
    pub _focus: math::mat::Mat3x1< f32 >,
}

impl Cam {
    pub fn init( id: u64, fov: f32, aspect: f32, near: f32, far: f32, pos: math::mat::Mat3x1< f32 >, focus: math::mat::Mat3x1< f32 >, up: math::mat::Mat3x1< f32 > ) -> Cam {
        Cam {
            _id: id,
            _fov: fov,
            _aspect: aspect,
            _near: near,
            _far: far,
            _pos: pos,
            _up: up,
            _focus: focus,
            _proj_xform: math::util::perspective( fov, aspect, near, far ),
            _view_xform: math::util::look_at( pos, focus, up ),
        }
    }    
}

impl i_renderobj::ObjPos for Cam {
    fn get_pos( & self ) -> math::mat::Mat3x1< f32 > {
        self._pos
    }
}
