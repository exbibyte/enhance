use implement::math::mat::Mat4;
use implement::math::mat::Mat3x1;
use implement::render::camera;
use std::collections::HashMap;
use std::vec::Vec;

pub struct Scene {
    _lights: Vec< Light >,
    _lightinteract: Vec< LightImteractAds >,
    _cams: Vec< camera::Cam >,
    _verts: Vec< Vert >,
    _vert_groups: Vec< VertGroup >,
}
