use implement::math::vector::Vector4;
use interface::i_shape;

#[derive(Debug, Copy, Clone)]
pub struct Ray4 < T > where T: Copy + Clone {
    pub _ori: Vector4< T >,
    pub _dir: Vector4< T >,
}

macro_rules! define_IShape_for_Ray4 {
    ($type_val:ty) => (
        impl IShape for Ray4 < $type_val > {
            fn bound( & self ) -> Option< Bound > {
                None
            }
            fn intersect( & self, other: & Self ) -> Option< Bound > {
                
            }
        }
    )
}

define_IMathTransform_for_Vector4!( i8 );
define_IMathTransform_for_Vector4!( i16 );
define_IMathTransform_for_Vector4!( i32 );
define_IMathTransform_for_Vector4!( i64 );
define_IMathTransform_for_Vector4!( u8 );
define_IMathTransform_for_Vector4!( u16 );
define_IMathTransform_for_Vector4!( u32 );
define_IMathTransform_for_Vector4!( u64 );
define_IMathTransform_for_Vector4!( f32 );
define_IMathTransform_for_Vector4!( f64 );
