use implement::math::mat::Mat4;
use std::collections::HashMap;
use std::vec::Vec;

pub struct Vert {
    _id: u64,
    _coord: Vec< f64 >,
    _normal: Vec< f64 >,
    _tc: Vec< f64 >,
}

pub struct Cam {
    _id: u64,
    _proj: Mat4< f64 >,
    _modelview: Mat4< f64 >,
}

pub struct LightImteractAds {
    _id: u64,
    _specular: f64,
    _diffuse: f64,
    _ambient: f64,
}

pub struct Light {
    _id: u64,
    _ambient: [f64; 4],
    _point: [f64; 4],
    _directional: [f64; 4],
    _specular: [f64; 4],
    _coord: Vec< f64 >,
    _modelview: Mat4< f64 >,
}

pub struct VertGroup {
    _id: u64,
    _vert_range: Vec< (usize,usize) >,
    _light_interfact_id: usize,
    _moveLview: Mat4< f64 >,
}

pub struct LightGroup {
    _light_ids: Vec< u64 >,
}

pub struct Scene {
    _lights: Vec< Light >,
    _lightinteract: Vec< LightImteractAds >,
    _cams: Vec< Cam >,
    _verts: Vec< Vert >,
    _vert_groups: Vec< VertGroup >,
}

pub enum TargetType {
    FRAMEBUFFER,
    COLOURBUFFER,
    DEPTHBUFFER,
}

pub struct PipelineTarget {
    _target_type: TargetType,
    _target_id: u64,
}

pub struct PipelineArgRequire {
    _req_lights: Vec< u64 >,
    _req_vert_groups: Vec< u64 >,
    _req_cams: Vec< u64 >,
    _req_target: PipelineTarget,
}

