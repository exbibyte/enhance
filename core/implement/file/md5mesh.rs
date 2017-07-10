use implement::math::quat::Quat;

pub struct Md5MultiMesh {
    pub _md5ver: u64,
    pub _cmdline: String,
    pub _numjoints: u64,
    pub _nummeshes: u64,
    pub _joints: Vec< Md5Joint >,
    pub _meshes: Vec< Md5Mesh >,
}

pub struct Md5Mesh {
    pub _shader: String,
    pub _numverts: u64,
    pub _numtris: u64,
    pub _numweights: u64,
    pub _verts: Vec< Md5Vert >,
    pub _tris: Vec< Md5Tri >,
    pub _weights: Vec< Md5Weight >,
}

pub struct Md5Joint {
    pub _name: String,
    pub _parent_index: u64,
    pub _pos: [f64;3],
    pub _orient: [f64;3],
    pub _rot: Quat,
}

pub struct Md5Vert {
    pub _index: u64,
    pub _tex_coords: [f64;2],
    pub _weight_start: u64,
    pub _weight_count: u64,
    pub _normal: [f64;3],
    pub _pos: [f64;3],
}

pub struct Md5Tri {
    pub _index: u64,
    pub _vert_indices: [f64;3],
}

pub struct Md5Weight {
    pub _index: u64,
    pub _joint_index: u64,
    pub _weight_bias: f64,
    pub _pos: [f64;3],
}

impl Md5MultiMesh {
    //entry point to user
    pub fn init( path: &str ) -> Md5MultiMesh {
        unimplemented!();
    }
}
