#[derive(Debug, Clone)]
pub struct ClusterBox {
    ///stores instance ids into grid box clusters
    pub _dim: [f32;3],
    pub _divisions: [usize;3],
}

