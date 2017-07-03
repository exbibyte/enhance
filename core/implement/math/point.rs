use implement::math::vector::Vector2;
use implement::math::vector::Vector3;
use implement::math::vector::Vector4;

#[derive(Debug, Copy, Clone)]
pub struct Point2 < T > where T: Copy + Clone {
    _coord: Vector2< T >,
}

#[derive(Debug, Copy, Clone)]
pub struct Point3 < T > where T: Copy + Clone {
    _coord: Vector3< T >,
}

#[derive(Debug, Copy, Clone)]
pub struct Point4 < T > where T: Copy + Clone {
    _coord: Vector4< T >,
}
