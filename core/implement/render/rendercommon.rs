// use implement::math::mat::Mat4;
// use implement::math::mat::Mat3x1;
// use implement::render::camera;
// use std::collections::HashMap;
// use std::vec::Vec;

// pub struct Vert {
//     _id: u64,
//     _coord: Vec< f64 >,
//     _normal: Vec< f64 >,
//     _tc: Vec< f64 >,
// }

// pub struct VertGroup {
//     _id: u64,
//     _vert_range: Vec< (usize,usize) >,
//     _light_interfact_id: usize,
//     _moveLview: Mat4< f64 >,
// }

// pub struct LightGroup {
//     _light_ids: Vec< u64 >,
// }

// pub enum TargetType {
//     FRAMEBUFFER,
//     COLOURBUFFER,
//     DEPTHBUFFER,
// }

// pub struct PipelineTarget {
//     _target_type: TargetType,
//     _target_id: u64,
// }

// pub struct PipelineArgRequire {
//     _req_lights: Vec< u64 >,
//     _req_vert_groups: Vec< u64 >,
//     _req_cams: Vec< u64 >,
//     _req_target: PipelineTarget,
// }
