use std::ops::Index;
use std::ops::IndexMut;
#[allow(unused_imports)]
use std::ops::Mul;
pub use interface::i_math_transform::*;

#[derive(Debug, Copy, Clone)]
pub struct Vector3 < T > where T: Copy + Clone {
    pub _val: [ T; 3 ],
}

impl < T > Index <u32> for Vector3< T > where T: Copy + Clone {
    type Output = T;
    fn index( &self, index: u32 ) -> &T {
        &self._val[ index as usize ]
    }
}

impl < T > IndexMut <u32> for Vector3< T > where T: Copy + Clone {
    fn index_mut( & mut self, index: u32 ) -> & mut T {
        &mut self._val[ index as usize ]
    }
}
macro_rules! define_IMathTransform_for_Vector {
    ($my_type:ty) => (
        impl IMathTransform for Vector3< $my_type > {
            fn size( & self ) -> u32 {
                self._val.len() as u32
            }
            fn dot( & self, other: & Vector3< $my_type> ) -> Vector3< $my_type > {
                Vector3 { _val: [
                    self._val[0] * other._val[0],
                    self._val[1] * other._val[1],
                    self._val[2] * other._val[2] ] }
            }
        }
    )
}
define_IMathTransform_for_Vector!( i8 );
define_IMathTransform_for_Vector!( i16 );
define_IMathTransform_for_Vector!( i32 );
define_IMathTransform_for_Vector!( i64 );
define_IMathTransform_for_Vector!( u8 );
define_IMathTransform_for_Vector!( u16 );
define_IMathTransform_for_Vector!( u32 );
define_IMathTransform_for_Vector!( u64 );
define_IMathTransform_for_Vector!( f32 );
define_IMathTransform_for_Vector!( f64 );
