#[allow(unused_imports)]
use std::ops::Index;
#[allow(unused_imports)]
use std::ops::IndexMut;
#[allow(unused_imports)]
use std::ops::Div;
#[allow(unused_imports)]
use std::ops::Mul;

#[derive(Debug, Copy, Clone)]
pub struct Mat2x1 < T > where T: Copy + Clone {
    pub _val: [ T; 2 ],
}
impl < T > Index <u32> for Mat2x1 < T > where T: Copy + Clone {
    type Output = T;
    fn index( &self, index: u32 ) -> &T {
        &self._val[ index as usize ]
    }
}
impl < T > IndexMut <u32> for Mat2x1 < T > where T: Copy + Clone {
    fn index_mut( & mut self, index: u32 ) -> & mut T {
        &mut self._val[ index as usize ]
    }
}

#[derive(Debug, Copy, Clone)]
pub struct Mat1x2 < T > where T: Copy + Clone {
    pub _val: [ T; 2 ],
}
impl < T > Index <u32> for Mat1x2 < T > where T: Copy + Clone {
    type Output = T;
    fn index( &self, index: u32 ) -> &T {
        &self._val[ index as usize ]
    }
}
impl < T > IndexMut <u32> for Mat1x2 < T > where T: Copy + Clone {
    fn index_mut( & mut self, index: u32 ) -> & mut T {
        &mut self._val[ index as usize ]
    }
}

#[derive(Debug, Copy, Clone)]
pub struct Mat3x1 < T > where T: Copy + Clone {
    pub _val: [ T; 3 ],
}

impl < T > Index <u32> for Mat3x1 < T > where T: Copy + Clone {
    type Output = T;
    fn index( &self, index: u32 ) -> &T {
        &self._val[ index as usize ]
    }
}
impl < T > IndexMut <u32> for Mat3x1 < T > where T: Copy + Clone {
    fn index_mut( & mut self, index: u32 ) -> & mut T {
        &mut self._val[ index as usize ]
    }
}

#[derive(Debug, Copy, Clone)]
pub struct Mat1x3 < T > where T: Copy + Clone {
    pub _val: [ T; 3 ],
}

impl < T > Index <u32> for Mat1x3 < T > where T: Copy + Clone {
    type Output = T;
    fn index( &self, index: u32 ) -> &T {
        &self._val[ index as usize ]
    }
}
impl < T > IndexMut <u32> for Mat1x3 < T > where T: Copy + Clone {
    fn index_mut( & mut self, index: u32 ) -> & mut T {
        &mut self._val[ index as usize ]
    }
}

#[derive(Debug, Copy, Clone)]
pub struct Mat4x1 < T > where T: Copy + Clone {
    pub _val: [ T; 4 ],
}
impl < T > Index <u32> for Mat4x1 < T > where T: Copy + Clone {
    type Output = T;
    fn index( &self, index: u32 ) -> &T {
        &self._val[ index as usize ]
    }
}
impl < T > IndexMut <u32> for Mat4x1 < T > where T: Copy + Clone {
    fn index_mut( & mut self, index: u32 ) -> & mut T {
        &mut self._val[ index as usize ]
    }
}

#[derive(Debug, Copy, Clone)]
pub struct Mat1x4 < T > where T: Copy + Clone {
    pub _val: [ T; 4 ],
}
impl < T > Index <u32> for Mat1x4 < T > where T: Copy + Clone {
    type Output = T;
    fn index( &self, index: u32 ) -> &T {
        &self._val[ index as usize ]
    }
}
impl < T > IndexMut <u32> for Mat1x4 < T > where T: Copy + Clone {
    fn index_mut( & mut self, index: u32 ) -> & mut T {
        &mut self._val[ index as usize ]
    }
}

macro_rules! define_mat2x1 {
    ($v_type: tt) => {
        impl Default for Mat2x1 < $v_type > {
            fn default() -> Mat2x1< $v_type > {
                Mat2x1 { _val: [ 0 as $v_type; 2 ] }
            }
        }
        impl Mat2x1 < $v_type > {
            pub fn size( & self ) -> Option< u32 > {
                Some ( self._val.len() as u32 )
            }
            pub fn mul_elem( & self, other: & Mat2x1 < $v_type > ) -> Option< Mat2x1 < $v_type > > {
                Some ( Mat2x1 { _val: [
                    self._val[0] * other._val[0],
                    self._val[1] * other._val[1] ] } )
            }
            pub fn div( & self, other: & Mat2x1 < $v_type > ) -> Option< Mat2x1 < $v_type > > {
                Some ( Mat2x1 { _val: [
                    self._val[0] / other._val[0],
                    self._val[1] / other._val[1] ] } )
            }
            pub fn plus( & self, other: & Mat2x1 < $v_type > ) -> Option< Mat2x1 < $v_type > > {
                Some ( Mat2x1 { _val: [
                    self._val[0] + other._val[0],
                    self._val[1] + other._val[1] ] } )
            }
            pub fn minus( & self, other: & Mat2x1 < $v_type > ) -> Option< Mat2x1 < $v_type > > {
                Some ( Mat2x1 { _val: [
                    self._val[0] - other._val[0],
                    self._val[1] - other._val[1] ] } )
            }
            pub fn dot( & self, other: & Mat2x1 < $v_type > ) -> Option< $v_type > {
                Some (
                    self._val[0] as $v_type * other._val[0] as $v_type +
                        self._val[1] as $v_type * other._val[1] as $v_type )
            }
            #[allow(unused_variables)]
            pub fn cross( & self, other: & Self ) -> Option< Self > {
                None
            }
            pub fn magnitude( & self ) -> Option < $v_type > {
                Some (
                    ( ( self._val[0] as $v_type ).powi( 2 ) +
                        ( self._val[1] as $v_type ).powi( 2 ) ).sqrt() )
            }
            pub fn normalize( self ) -> Option < Self > {
                let m = self.magnitude().expect("magnitude invalid");
                Some ( Mat2x1 { _val: [
                    ( self._val[0] as $v_type ).div( m ) as $v_type,
                    ( self._val[1] as $v_type ).div( m ) as $v_type ] } )
            }
            pub fn scale( & self, s: $v_type ) -> Option < Self > {
                Some ( Mat2x1 { _val: [
                    ( self._val[0] as $v_type ).mul( s ) as $v_type,
                    ( self._val[1] as $v_type ).mul( s ) as $v_type ] } )
            }
            pub fn is_equal( & self, other: & Self, error: $v_type ) -> Option< bool > {
                Some( ( self._val[0] as $v_type >= other._val[0] as $v_type - error ) &&
                        ( self._val[0] as $v_type <= other._val[0] as $v_type + error ) &&
                        ( self._val[1] as $v_type >= other._val[1] as $v_type - error ) &&
                        ( self._val[1] as $v_type <= other._val[1] as $v_type + error ) )
            }
            pub fn transpose( self ) -> Mat1x2< $v_type > {
                Mat1x2 { _val: self._val }
            }
        }
    }
}

macro_rules! define_mat1x2 {
    ($v_type: tt) => {
        impl Default for Mat1x2 < $v_type > {
            fn default() -> Mat1x2< $v_type > {
                Mat1x2 { _val: [ 0 as $v_type; 2 ] }
            }
        }
        impl Mat1x2 < $v_type > {
            pub fn size( & self ) -> Option< u32 > {
                Some ( self._val.len() as u32 )
            }
            pub fn mul_elem( & self, other: & Mat1x2 < $v_type > ) -> Option< Mat1x2 < $v_type > > {
                Some ( Mat1x2 { _val: [
                    self._val[0] * other._val[0],
                    self._val[1] * other._val[1] ] } )
            }
            pub fn div( & self, other: & Mat1x2 < $v_type > ) -> Option< Mat1x2 < $v_type > > {
                Some ( Mat1x2 { _val: [
                    self._val[0] / other._val[0],
                    self._val[1] / other._val[1] ] } )
            }
            pub fn plus( & self, other: & Mat1x2 < $v_type > ) -> Option< Mat1x2 < $v_type > > {
                Some ( Mat1x2 { _val: [
                    self._val[0] + other._val[0],
                    self._val[1] + other._val[1] ] } )
            }
            pub fn minus( & self, other: & Mat1x2 < $v_type > ) -> Option< Mat1x2 < $v_type > > {
                Some ( Mat1x2 { _val: [
                    self._val[0] - other._val[0],
                    self._val[1] - other._val[1] ] } )
            }
            pub fn dot( & self, other: & Mat1x2 < $v_type > ) -> Option< $v_type > {
                Some (
                    self._val[0] as $v_type * other._val[0] as $v_type +
                        self._val[1] as $v_type * other._val[1] as $v_type )
            }
            #[allow(unused_variables)]
            pub fn cross( & self, other: & Self ) -> Option< Self > {
                None
            }
            pub fn magnitude( & self ) -> Option < $v_type > {
                Some (
                    ( ( self._val[0] as $v_type ).powi( 2 ) +
                        ( self._val[1] as $v_type ).powi( 2 ) ).sqrt() )
            }
            pub fn normalize( self ) -> Option < Self > {
                let m = self.magnitude().expect("magnitude invalid");
                Some ( Mat1x2 { _val: [
                    ( self._val[0] as $v_type ).div( m ) as $v_type,
                    ( self._val[1] as $v_type ).div( m ) as $v_type ] } )
            }
            pub fn scale( & self, s: $v_type ) -> Option < Self > {
                Some ( Mat1x2 { _val: [
                    ( self._val[0] as $v_type ).mul( s ) as $v_type,
                    ( self._val[1] as $v_type ).mul( s ) as $v_type ] } )
            }
            pub fn is_equal( & self, other: & Self, error: $v_type ) -> Option< bool > {
                Some( ( self._val[0] as $v_type >= other._val[0] as $v_type - error ) &&
                        ( self._val[0] as $v_type <= other._val[0] as $v_type + error ) &&
                        ( self._val[1] as $v_type >= other._val[1] as $v_type - error ) &&
                        ( self._val[1] as $v_type <= other._val[1] as $v_type + error ) )
            }
            pub fn transpose( self ) -> Mat2x1< $v_type > {
                Mat2x1 { _val: self._val }
            }
        }
    }
}

macro_rules! define_mat3x1 {
    ($v_type: tt) => {
        impl Default for Mat3x1 < $v_type > {
            fn default() -> Mat3x1< $v_type > {
                Mat3x1 { _val: [ 0 as $v_type; 3 ] }
            }
        }
        impl Mat3x1 < $v_type > {
            pub fn size( & self ) -> Option< u32 > {
                Some ( self._val.len() as u32 )
            }
            pub fn mul_elem( & self, other: & Mat3x1 < $v_type > ) -> Option< Mat3x1 < $v_type > > {
                Some ( Mat3x1 { _val: [
                    self._val[0] * other._val[0],
                    self._val[1] * other._val[1],
                    self._val[2] * other._val[2] ] } )
            }
            pub fn div( & self, other: & Mat3x1 < $v_type > ) -> Option< Mat3x1 < $v_type > > {
                Some ( Mat3x1 { _val: [
                    self._val[0] / other._val[0],
                    self._val[1] / other._val[1],
                    self._val[2] / other._val[2] ] } )
            }
            pub fn plus( & self, other: & Mat3x1 < $v_type > ) -> Option< Mat3x1 < $v_type > > {
                Some ( Mat3x1 { _val: [
                    self._val[0] + other._val[0],
                    self._val[1] + other._val[1],
                    self._val[2] + other._val[2] ] } )
            }
            pub fn minus( & self, other: & Mat3x1 < $v_type > ) -> Option< Mat3x1 < $v_type > > {
                Some ( Mat3x1 { _val: [
                    self._val[0] - other._val[0],
                    self._val[1] - other._val[1],
                    self._val[2] - other._val[2] ] } )
            }
            pub fn dot( & self, other: & Mat3x1 < $v_type > ) -> Option< $v_type > {
                Some (
                    self._val[0] as $v_type * other._val[0] as $v_type +
                        self._val[1] as $v_type * other._val[1] as $v_type +
                        self._val[2] as $v_type * other._val[2] as $v_type )
            }
            pub fn cross( & self, other: & Self ) -> Option< Self > {
                Some ( Mat3x1 { _val: [
                    ( ( self._val[1] as $v_type * other._val[2] as $v_type ) -
                        ( self._val[2] as $v_type * other._val[1] as $v_type ) ) as $v_type,
                    ( ( self._val[2] as $v_type * other._val[0] as $v_type ) -
                        ( self._val[0] as $v_type * other._val[2] as $v_type ) ) as $v_type,
                    ( ( self._val[0] as $v_type * other._val[1] as $v_type ) -
                        ( self._val[1] as $v_type * other._val[0] as $v_type ) ) as $v_type ] } )
            }
            pub fn magnitude( & self ) -> Option < $v_type > {
                Some (
                    ( ( self._val[0] as $v_type ).powi( 2 ) +
                        ( self._val[1] as $v_type ).powi( 2 ) +
                        ( self._val[2] as $v_type ).powi( 2 ) ).sqrt() )
            }
            pub fn normalize( self ) -> Option < Self > {
                let m = self.magnitude().expect("magnitude invalid");
                Some ( Mat3x1 { _val: [
                    ( self._val[0] as $v_type ).div( m ) as $v_type,
                    ( self._val[1] as $v_type ).div( m ) as $v_type,
                    ( self._val[2] as $v_type ).div( m ) as $v_type ] } )
            }
            pub fn scale( & self, s: $v_type ) -> Option < Self > {
                Some ( Mat3x1 { _val: [
                    ( self._val[0] as $v_type ).mul( s ) as $v_type,
                    ( self._val[1] as $v_type ).mul( s ) as $v_type,
                    ( self._val[2] as $v_type ).mul( s ) as $v_type ] } )
            }
            pub fn is_equal( & self, other: & Self, error: $v_type ) -> Option< bool > {
                Some( ( self._val[0] as $v_type >= other._val[0] as $v_type - error ) &&
                        ( self._val[0] as $v_type <= other._val[0] as $v_type + error ) &&
                        ( self._val[1] as $v_type >= other._val[1] as $v_type - error ) &&
                        ( self._val[1] as $v_type <= other._val[1] as $v_type + error ) &&
                        ( self._val[2] as $v_type >= other._val[2] as $v_type - error ) &&
                        ( self._val[2] as $v_type <= other._val[2] as $v_type + error ) )
            }
            pub fn transpose( self ) -> Mat1x3< $v_type > {
                Mat1x3 { _val: self._val }
            }
        }
    }
}

macro_rules! define_mat1x3 {
    ($v_type: tt) => {
        impl Default for Mat1x3 < $v_type > {
            fn default() -> Mat1x3< $v_type > {
                Mat1x3 { _val: [ 0 as $v_type; 3 ] }
            }
        }
        impl Mat1x3 < $v_type > {
            pub fn size( & self ) -> Option< u32 > {
                Some ( self._val.len() as u32 )
            }
            pub fn mul_elem( & self, other: & Mat1x3 < $v_type > ) -> Option< Mat1x3 < $v_type > > {
                Some ( Mat1x3 { _val: [
                    self._val[0] * other._val[0],
                    self._val[1] * other._val[1],
                    self._val[2] * other._val[2] ] } )
            }
            pub fn mul_mat3x3( & self, other: & Mat3 < $v_type > ) -> Option< Mat1x3 < $v_type > > {
                let mut m = Mat1x3::default();
                if other._is_row_major {
                    for x in 0..3 {
                        m._val[ x ] = ( other._val[ x + 0 ] * self._val[ 0 ] +
                                        other._val[ x + 3 ] * self._val[ 1 ] +
                                        other._val[ x + 6 ] * self._val[ 2 ] ) as $v_type;
                    }
                } else { //column major
                    for y in 0..3 {
                        m._val[ y ] = ( other._val[ y * 3 + 0 ] * self._val[ 0 ] +
                                        other._val[ y * 3 + 1 ] * self._val[ 1 ] +
                                        other._val[ y * 3 + 2 ] * self._val[ 2 ] ) as $v_type;
                    }
                }
                Some( m )
            }
            pub fn div( & self, other: & Mat1x3 < $v_type > ) -> Option< Mat1x3 < $v_type > > {
                Some ( Mat1x3 { _val: [
                    self._val[0] / other._val[0],
                    self._val[1] / other._val[1],
                    self._val[2] / other._val[2] ] } )
            }
            pub fn plus( & self, other: & Mat1x3 < $v_type > ) -> Option< Mat1x3 < $v_type > > {
                Some ( Mat1x3 { _val: [
                    self._val[0] + other._val[0],
                    self._val[1] + other._val[1],
                    self._val[2] + other._val[2] ] } )
            }
            pub fn minus( & self, other: & Mat1x3 < $v_type > ) -> Option< Mat1x3 < $v_type > > {
                Some ( Mat1x3 { _val: [
                    self._val[0] - other._val[0],
                    self._val[1] - other._val[1],
                    self._val[2] - other._val[2] ] } )
            }
            pub fn dot( & self, other: & Mat1x3 < $v_type > ) -> Option< $v_type > {
                Some (
                    self._val[0] as $v_type * other._val[0] as $v_type +
                        self._val[1] as $v_type * other._val[1] as $v_type +
                        self._val[2] as $v_type * other._val[2] as $v_type )
            }
            pub fn cross( & self, other: & Self ) -> Option< Self > {
                Some ( Mat1x3 { _val: [
                    ( ( self._val[1] as $v_type * other._val[2] as $v_type ) -
                        ( self._val[2] as $v_type * other._val[1] as $v_type ) ) as $v_type,
                    ( ( self._val[2] as $v_type * other._val[0] as $v_type ) -
                        ( self._val[0] as $v_type * other._val[2] as $v_type ) ) as $v_type,
                    ( ( self._val[0] as $v_type * other._val[1] as $v_type ) -
                        ( self._val[1] as $v_type * other._val[0] as $v_type ) ) as $v_type ] } )
            }
            pub fn magnitude( & self ) -> Option < $v_type > {
                Some (
                    ( ( self._val[0] as $v_type ).powi( 2 ) +
                        ( self._val[1] as $v_type ).powi( 2 ) +
                        ( self._val[2] as $v_type ).powi( 2 ) ).sqrt() )
            }
            pub fn normalize( self ) -> Option < Self > {
                let m = self.magnitude().expect("magnitude invalid");
                Some ( Mat1x3 { _val: [
                    ( self._val[0] as $v_type ).div( m ) as $v_type,
                    ( self._val[1] as $v_type ).div( m ) as $v_type,
                    ( self._val[2] as $v_type ).div( m ) as $v_type ] } )
            }
            pub fn scale( & self, s: $v_type ) -> Option < Self > {
                Some ( Mat1x3 { _val: [
                    ( self._val[0] as $v_type ).mul( s ) as $v_type,
                    ( self._val[1] as $v_type ).mul( s ) as $v_type,
                    ( self._val[2] as $v_type ).mul( s ) as $v_type ] } )
            }
            pub fn is_equal( & self, other: & Self, error: $v_type ) -> Option< bool > {
                Some( ( self._val[0] as $v_type >= other._val[0] as $v_type - error ) &&
                        ( self._val[0] as $v_type <= other._val[0] as $v_type + error ) &&
                        ( self._val[1] as $v_type >= other._val[1] as $v_type - error ) &&
                        ( self._val[1] as $v_type <= other._val[1] as $v_type + error ) &&
                        ( self._val[2] as $v_type >= other._val[2] as $v_type - error ) &&
                        ( self._val[2] as $v_type <= other._val[2] as $v_type + error ) )
            }
            pub fn transpose( self ) -> Mat3x1< $v_type > {
                Mat3x1 { _val: self._val }
            }
        }
    }
}
macro_rules! define_mat4x1 {
    ($v_type: tt) => {
        impl Default for Mat4x1 < $v_type > {
            fn default() -> Mat4x1< $v_type > {
                Mat4x1 { _val: [ 0 as $v_type; 4 ] }
            }
        }
        impl Mat4x1 < $v_type > {
            pub fn size( & self ) -> Option< u32 > {
                Some ( self._val.len() as u32 )
            }
            pub fn mul_elem( & self, other: & Mat4x1 < $v_type > ) -> Option< Mat4x1 < $v_type > > {
                Some ( Mat4x1 { _val: [
                    self._val[0] * other._val[0],
                    self._val[1] * other._val[1],
                    self._val[2] * other._val[2],
                    self._val[3] * other._val[3]] } )
            }
            pub fn div( & self, other: & Mat4x1 < $v_type > ) -> Option< Mat4x1 < $v_type > > {
                Some ( Mat4x1 { _val: [
                    self._val[0] / other._val[0],
                    self._val[1] / other._val[1],
                    self._val[2] / other._val[2],
                    self._val[3] / other._val[3]] } )
            }
            pub fn plus( & self, other: & Mat4x1 < $v_type > ) -> Option< Mat4x1 < $v_type > > {
                Some ( Mat4x1 { _val: [
                    self._val[0] + other._val[0],
                    self._val[1] + other._val[1],
                    self._val[2] + other._val[2],
                    self._val[3] + other._val[3]] } )
            }
            pub fn minus( & self, other: & Mat4x1 < $v_type > ) -> Option< Mat4x1 < $v_type > > {
                Some ( Mat4x1 { _val: [
                    self._val[0] - other._val[0],
                    self._val[1] - other._val[1],
                    self._val[2] - other._val[2],
                    self._val[3] - other._val[3]] } )
            }
            pub fn dot( & self, other: & Mat4x1 < $v_type > ) -> Option < $v_type > {
                Some (
                    self._val[0] as $v_type * other._val[0] as $v_type +
                        self._val[1] as $v_type * other._val[1] as $v_type +
                        self._val[2] as $v_type * other._val[2] as $v_type +
                        self._val[3] as $v_type * other._val[3] as $v_type )
            }
            #[allow(unused_variables)]
            pub fn cross( & self, other: & Self ) -> Option< Self > {
                None
            }
            pub fn magnitude( & self ) -> Option < $v_type > {
                Some (
                    ( ( self._val[0] as $v_type ).powi( 2 ) +
                        ( self._val[1] as $v_type ).powi( 2 ) +
                        ( self._val[2] as $v_type ).powi( 2 ) +
                        ( self._val[3] as $v_type ).powi( 2 ) ).sqrt() )
            }
            pub fn normalize( self ) -> Option < Self > {
                let m = self.magnitude().expect("magnitude invalid");
                Some ( Mat4x1 { _val: [
                    ( self._val[0] as $v_type ).div( m ) as $v_type,
                    ( self._val[1] as $v_type ).div( m ) as $v_type,
                    ( self._val[2] as $v_type ).div( m ) as $v_type,
                    ( self._val[3] as $v_type ).div( m ) as $v_type ] } )
            }
            pub fn scale( & self, s: $v_type ) -> Option < Self > {
                Some ( Mat4x1 { _val: [
                    ( self._val[0] as $v_type ).mul( s ) as $v_type,
                    ( self._val[1] as $v_type ).mul( s ) as $v_type,
                    ( self._val[2] as $v_type ).mul( s ) as $v_type,
                    ( self._val[3] as $v_type ).mul( s ) as $v_type ] } )
            }
            pub fn is_equal( & self, other: & Self, error: $v_type ) -> Option< bool > {
                Some( ( self._val[0] as $v_type >= other._val[0] as $v_type - error ) &&
                        ( self._val[0] as $v_type <= other._val[0] as $v_type + error ) &&
                        ( self._val[1] as $v_type >= other._val[1] as $v_type - error ) &&
                        ( self._val[1] as $v_type <= other._val[1] as $v_type + error ) &&
                        ( self._val[2] as $v_type >= other._val[2] as $v_type - error ) &&
                        ( self._val[2] as $v_type <= other._val[2] as $v_type + error ) &&
                        ( self._val[3] as $v_type >= other._val[3] as $v_type - error ) &&
                        ( self._val[3] as $v_type <= other._val[3] as $v_type + error ) )
            }
            pub fn transpose( self ) -> Mat1x4< $v_type > {
                Mat1x4 { _val: self._val }
            }
        }
    }
}

macro_rules! define_mat1x4 {
    ($v_type: tt) => {
       impl Default for Mat1x4 < $v_type > {
            fn default() -> Mat1x4< $v_type > {
                Mat1x4 { _val: [ 0 as  $v_type; 4 ] }
            }
        }
        impl Mat1x4 < $v_type > {
            pub fn size( & self ) -> Option< u32 > {
                Some ( self._val.len() as u32 )
            }
            pub fn mul_elem( & self, other: & Mat1x4 < $v_type > ) -> Option< Mat1x4 < $v_type > > {
                Some ( Mat1x4 { _val: [
                    self._val[0] * other._val[0],
                    self._val[1] * other._val[1],
                    self._val[2] * other._val[2],
                    self._val[3] * other._val[3]] } )
            }
            pub fn mul_mat4x4( & self, other: & Mat4 < $v_type > ) -> Option< Mat1x4 < $v_type > > {
                let mut m = Mat1x4::default();
                if other._is_row_major {
                    for x in 0..4 {
                        m._val[ x ] = ( other._val[ x + 0 ] * self._val[ 0 ] +
                                        other._val[ x + 4 ] * self._val[ 1 ] +
                                        other._val[ x + 8 ] * self._val[ 2 ] +
                                        other._val[ x + 12 ] * self._val[ 3 ] ) as $v_type;
                    }
                } else { //column major
                    for y in 0..4 {
                        m._val[ y ] = ( other._val[ y * 4 + 0 ] * self._val[ 0 ] +
                                        other._val[ y * 4 + 1 ] * self._val[ 1 ] +
                                        other._val[ y * 4 + 2 ] * self._val[ 2 ] +
                                        other._val[ y * 4 + 3 ] * self._val[ 3 ] ) as $v_type;
                    }
                }
                Some( m )
            }
            pub fn div( & self, other: & Mat1x4 < $v_type > ) -> Option< Mat1x4 < $v_type > > {
                Some ( Mat1x4 { _val: [
                    self._val[0] / other._val[0],
                    self._val[1] / other._val[1],
                    self._val[2] / other._val[2],
                    self._val[3] / other._val[3]] } )
            }
            pub fn plus( & self, other: & Mat1x4 < $v_type > ) -> Option< Mat1x4 < $v_type > > {
                Some ( Mat1x4 { _val: [
                    self._val[0] + other._val[0],
                    self._val[1] + other._val[1],
                    self._val[2] + other._val[2],
                    self._val[3] + other._val[3]] } )
            }
            pub fn minus( & self, other: & Mat1x4 < $v_type > ) -> Option< Mat1x4 < $v_type > > {
                Some ( Mat1x4 { _val: [
                    self._val[0] - other._val[0],
                    self._val[1] - other._val[1],
                    self._val[2] - other._val[2],
                    self._val[3] - other._val[3]] } )
            }
            pub fn dot( & self, other: & Mat1x4 < $v_type > ) -> Option < $v_type > {
                Some (
                    self._val[0] as $v_type * other._val[0] as $v_type +
                        self._val[1] as $v_type * other._val[1] as $v_type +
                        self._val[2] as $v_type * other._val[2] as $v_type +
                        self._val[3] as $v_type * other._val[3] as $v_type )
            }
            #[allow(unused_variables)]
            pub fn cross( & self, other: & Self ) -> Option< Self > {
                None
            }
            pub fn magnitude( & self ) -> Option < $v_type > {
                Some (
                    ( ( self._val[0] as $v_type ).powi( 2 ) +
                        ( self._val[1] as $v_type ).powi( 2 ) +
                        ( self._val[2] as $v_type ).powi( 2 ) +
                        ( self._val[3] as $v_type ).powi( 2 ) ).sqrt() )
            }
            pub fn normalize( self ) -> Option < Self > {
                let m = self.magnitude().expect("magnitude invalid");
                Some ( Mat1x4 { _val: [
                    ( self._val[0] as $v_type ).div( m ) as $v_type,
                    ( self._val[1] as $v_type ).div( m ) as $v_type,
                    ( self._val[2] as $v_type ).div( m ) as $v_type,
                    ( self._val[3] as $v_type ).div( m ) as $v_type ] } )
            }
            pub fn scale( & self, s: $v_type ) -> Option < Self > {
                Some ( Mat1x4 { _val: [
                    ( self._val[0] as $v_type ).mul( s ) as $v_type,
                    ( self._val[1] as $v_type ).mul( s ) as $v_type,
                    ( self._val[2] as $v_type ).mul( s ) as $v_type,
                    ( self._val[3] as $v_type ).mul( s ) as $v_type ] } )
            }
            pub fn is_equal( & self, other: & Self, error: $v_type ) -> Option< bool > {
                Some( ( self._val[0] as $v_type >= other._val[0] as $v_type - error ) &&
                        ( self._val[0] as $v_type <= other._val[0] as $v_type + error ) &&
                        ( self._val[1] as $v_type >= other._val[1] as $v_type - error ) &&
                        ( self._val[1] as $v_type <= other._val[1] as $v_type + error ) &&
                        ( self._val[2] as $v_type >= other._val[2] as $v_type - error ) &&
                        ( self._val[2] as $v_type <= other._val[2] as $v_type + error ) &&
                        ( self._val[3] as $v_type >= other._val[3] as $v_type - error ) &&
                        ( self._val[3] as $v_type <= other._val[3] as $v_type + error ) )
            }
            pub fn transpose( self ) -> Mat4x1< $v_type > {
                Mat4x1 { _val: self._val }
            }
        }
    }
}

#[derive(Debug, Copy, Clone)]
pub struct Mat3 < T > where T: Copy + Clone {
    pub _val: [ T ; 9 ],
    pub _is_row_major: bool,
}

macro_rules! define_mat3 {
    ($v_type: tt) => {
        
        impl Default for Mat3 < $v_type > {
            fn default() -> Mat3< $v_type > {
                Mat3 { _val: [ 0 as $v_type; 9 ],
                        _is_row_major: true,
                }
            }
        }
        
        impl Mat3 < $v_type > {
            #[allow(dead_code)]
            pub fn init( arr: [ $v_type; 9 ], row_major: bool ) -> Mat3 < $v_type > {
                match row_major {
                    true => Mat3 { _val: arr,
                                    _is_row_major: true },
                    _ =>    Mat3 { _val: [ arr[0], arr[3], arr[6],
                                           arr[1], arr[4], arr[7],
                                           arr[2], arr[5], arr[8],
                    ],
                                    _is_row_major: false }
                }

            }
            #[allow(dead_code)]
            pub fn trace( & self ) -> $v_type {
                self._val[ 0 ] + self._val[ 5 ] + self._val[ 10 ]
            }
            #[allow(dead_code)]
            pub fn index( & self, m: u64, n: u64 ) -> $v_type {
                if self._is_row_major {
                    self._val[ ( m as usize ) * 3 + n as usize ]
                } else {
                    self._val[ ( m as usize ) + ( n as usize ) * 3 ]
                }
            }
            #[allow(dead_code)]
            pub fn index_mut( & mut self, m: u64, n: u64 ) -> & mut $v_type {
                if self._is_row_major {
                    & mut self._val[ ( m as usize ) * 3 + n as usize ]
                } else {
                    & mut self._val[ ( m as usize ) + ( n as usize ) * 3 ]
                }
            }
            #[allow(unused_variables)]
            pub fn size( & self ) -> Option< u32 > {
                Some ( self._val.len() as u32 )
            }
            #[allow(unused_variables)]
            pub fn mul_elem( & self, other: & Mat3 < $v_type > ) -> Option< Mat3 < $v_type > > {
                Some ( Mat3 { _val: [
                    self._val[0] * other._val[0],
                    self._val[1] * other._val[1],
                    self._val[2] * other._val[2],
                    self._val[3] * other._val[3],
                    self._val[4] * other._val[4],
                    self._val[5] * other._val[5],
                    self._val[6] * other._val[6],
                    self._val[7] * other._val[7],
                    self._val[8] * other._val[8], ],
                               _is_row_major: self._is_row_major } )
            }
            pub fn mul( & self, other: & Mat3 < $v_type > ) -> Option< Mat3 < $v_type > > {
                assert!( self._is_row_major == other._is_row_major );
                let mut m = Mat3 { _val: [0 as $v_type; 9], _is_row_major: self._is_row_major, };
                if self._is_row_major {
                    for y in 0..3 { //row index
                        for x in 0..3 { //column index
                            m._val[ y * 3 + x ] = ( self._val[ y * 3 + 0 ] * other._val[ x + 0 ] +
                                                    self._val[ y * 3 + 1 ] * other._val[ x + 3 ] +
                                                    self._val[ y * 3 + 2 ] * other._val[ x + 6 ] ) as $v_type;
                        }
                    }
                } else { //column major
                    for x in 0..3 { //column index
                        for y in 0..3 { //row index
                            m._val[ x * 3 + y ] = ( self._val[ y + 0 ] * other._val[ x * 3 + 0 ] +
                                                    self._val[ y + 3 ] * other._val[ x * 3 + 1 ] +
                                                    self._val[ y + 6 ] * other._val[ x * 3 + 2 ] ) as $v_type;
                        }
                    }
                }
                Some( m )
            }
            pub fn mul_mat3x1( & self, other: & Mat3x1 < $v_type > ) -> Option< Mat3x1 < $v_type > > {
                let mut m = Mat3x1::default();
                if self._is_row_major {
                    for y in 0..3 { //row index
                        m._val[ y ] = ( self._val[ y * 3 + 0 ] * other._val[ 0 ] +
                                        self._val[ y * 3 + 1 ] * other._val[ 1 ] +
                                        self._val[ y * 3 + 2 ] * other._val[ 2 ] ) as $v_type;
                    }
                } else { //column major
                    for x in 0..3 { //column index
                        m._val[ x ] = ( self._val[ x + 0 ] * other._val[ 0 ] +
                                        self._val[ x + 3 ] * other._val[ 1 ] +
                                        self._val[ x + 6 ] * other._val[ 2 ] ) as $v_type;
                    }
                }
                Some( m )
            }
            #[allow(unused_variables)]
            pub fn div( & self, other: & Mat3 < $v_type > ) -> Option< Mat3 < $v_type > > {
                Some ( Mat3 { _val: [
                    self._val[0] / other._val[0],
                    self._val[1] / other._val[1],
                    self._val[2] / other._val[2],
                    self._val[3] / other._val[3],
                    self._val[4] / other._val[4],
                    self._val[5] / other._val[5],
                    self._val[6] / other._val[6],
                    self._val[7] / other._val[7],
                    self._val[8] / other._val[8] ],
                               _is_row_major: self._is_row_major } )
            }
            #[allow(unused_variables)]
            pub fn plus( & self, other: & Mat3 < $v_type > ) -> Option< Mat3 < $v_type > > {
                Some ( Mat3 { _val: [
                    self._val[0] + other._val[0],
                    self._val[1] + other._val[1],
                    self._val[2] + other._val[2],
                    self._val[3] + other._val[3],
                    self._val[4] + other._val[4],
                    self._val[5] + other._val[5],
                    self._val[6] + other._val[6],
                    self._val[7] + other._val[7],
                    self._val[8] + other._val[8] ],
                               _is_row_major: self._is_row_major } )
            }
            #[allow(unused_variables)]
            pub fn minus( & self, other: & Mat3 < $v_type > ) -> Option< Mat3 < $v_type > > {
                Some ( Mat3 { _val: [
                    self._val[0] - other._val[0],
                    self._val[1] - other._val[1],
                    self._val[2] - other._val[2],
                    self._val[3] - other._val[3],
                    self._val[4] - other._val[4],
                    self._val[5] - other._val[5],
                    self._val[6] - other._val[6],
                    self._val[7] - other._val[7],
                    self._val[8] - other._val[8] ],
                               _is_row_major: self._is_row_major } )
            }
            #[allow(unused_variables)]
            pub fn dot( & self, other: & Mat3 < $v_type > ) -> Option < $v_type > {
                None
            }
            #[allow(unused_variables)]
            pub fn cross( & self, other: & Self ) -> Option< Self > {
                None
            }
            pub fn magnitude( & self ) -> Option < $v_type > {
                unimplemented!();
            }
            pub fn normalize( self ) -> Option < Self > {
                //normalize all elements of the matrix to the last element of the matrix
                let mut m = Mat3 { _val: self._val, _is_row_major: self._is_row_major };
                for x in 0..self._val.len() {
                    m._val[x] = ( m._val[x] as $v_type / m._val[8] as $v_type ) as $v_type;
                }
                Some( m )
            }
            pub fn scale( & self, s: $v_type ) -> Option < Self > {
                let mut m = Mat3 { _val: self._val, _is_row_major: self._is_row_major };
                for x in 0..self._val.len() {
                    m._val[x] = ( m._val[x] as $v_type * s ) as $v_type;
                }
                Some( m )
            }
            pub fn is_equal( & self, other: & Self, error: $v_type ) -> Option< bool > {
                if self._is_row_major != other._is_row_major {
                    return Some( false )
                }
                for x in 0..self._val.len() {
                    let e = ( self._val[x] as $v_type >= (other._val[x] as $v_type - error) ) &&
                        ( self._val[x] as $v_type <= (other._val[x] as $v_type + error) );
                    if !e {
                        return Some( false )
                    }
                }
                Some( true )
            }
            pub fn transpose_self( & mut self ) {
                let copy = self._val.clone();
                for i in 0..9 {
                    if i < 3 {
                        self._val[i*3] = copy[i];
                    } else if i >= 3 && i < 6 {
                        self._val[(i-3)*3 + 1] = copy[i];
                    } else {
                        self._val[(i-6)*3 + 2] = copy[i];
                    }
                }
            }
            pub fn transpose( self ) -> Mat3< $v_type > {
                let mut copy = Mat3 { _val: self._val, _is_row_major: self._is_row_major };
                for i in 0..9 {
                    if i < 3 {
                        copy._val[i*3] = self._val[i];
                    } else if i >= 3 && i < 6 {
                        copy._val[(i-3)*3 + 1] = self._val[i];
                    } else {
                        copy._val[(i-6)*3 + 2] = self._val[i];
                    }
                }
                copy
            }
            pub fn invert( &self ) -> Option< Mat3< $v_type > > {
                let determinant = self.index(0,0)*(self.index(1,1)*self.index(2,2)-self.index(2,1)*self.index(1,2))
                    - self.index(1,0)*(self.index(0,1)*self.index(2,2)-self.index(2,1)*self.index(0,2))
                    + self.index(2,0)*(self.index(0,1)*self.index(1,2)-self.index(1,1)*self.index(0,2));
                if ( determinant < 0.0000000001 ) && ( determinant > -0.00000000001 ){
                    return None
                }
                let t = self.transpose();
                let mut out = *self;
                *out.index_mut(0,0) = t.index(1,1)*t.index(2,2)-t.index(2,1)*t.index(1,2) / determinant;
                *out.index_mut(1,0) = -(t.index(0,1)*t.index(2,2)-t.index(2,1)*t.index(0,2)) / determinant;
                *out.index_mut(2,0) = t.index(0,1)*t.index(1,2)-t.index(1,1)*t.index(0,2) / determinant;
                *out.index_mut(0,1) = t.index(1,0)*t.index(2,2)-t.index(2,0)*t.index(1,2) / determinant;
                *out.index_mut(1,1) = -(t.index(0,0)*t.index(2,2)-t.index(2,0)*t.index(0,2)) / determinant;
                *out.index_mut(2,1) = t.index(0,0)*t.index(1,2)-t.index(1,0)*t.index(0,2) / determinant;
                *out.index_mut(0,2) = t.index(1,0)*t.index(2,1)-t.index(2,0)*t.index(1,1) / determinant;
                *out.index_mut(1,2) = -(t.index(0,0)*t.index(2,1)-t.index(2,0)*t.index(0,1)) / determinant;
                *out.index_mut(2,2) = t.index(0,0)*t.index(1,1)-t.index(1,0)*t.index(0,1) / determinant;
                
                Some(out)
            }
        }
    }
}

#[derive(Debug, Copy, Clone)]
pub struct Mat4 < T > where T : Copy + Clone {
    pub _val: [ T ; 16 ],
    pub _is_row_major: bool,
}

macro_rules! define_mat4 {
    ($v_type: tt) => {
        
        impl Default for Mat4 < $v_type > {
            fn default() -> Mat4< $v_type > {
                Mat4 { _val: [ 0 as $v_type; 16 ],
                        _is_row_major: true,
                }
            }
        }

        impl Mat4 < $v_type > {
            #[allow(dead_code)]
            pub fn init( arr: [ $v_type; 16 ], row_major: bool ) -> Mat4 < $v_type > {
                match row_major {
                    true => Mat4 { _val: arr,
                                    _is_row_major: true },
                    _ =>    Mat4 { _val: [ arr[0], arr[4], arr[8], arr[12],
                                           arr[1], arr[5], arr[9], arr[13],
                                           arr[2], arr[6], arr[10], arr[14],
                                           arr[3], arr[7], arr[11], arr[15],
                    ],
                                    _is_row_major: false }
                }

            }
            #[allow(dead_code)]
            pub fn trace( & self ) -> $v_type {
                self._val[ 0 ] + self._val[ 5 ] + self._val[ 10 ] + self._val[ 15 ]
            }
            #[allow(dead_code)]
            pub fn index( & self, m: u64, n: u64 ) -> $v_type {
                if self._is_row_major {
                    self._val[ ( m as usize ) * 4 + n as usize ]
                } else {
                    self._val[ ( m as usize ) + ( n as usize ) * 4 ]
                }
            }
            #[allow(dead_code)]
            pub fn index_mut( & mut self, m: u64, n: u64 ) -> & mut $v_type {
                if self._is_row_major {
                    & mut self._val[ ( m as usize ) * 4 + n as usize ]
                } else {
                    & mut self._val[ ( m as usize ) + ( n as usize ) * 4 ]
                }
            }
            #[allow(unused_variables)]
            pub fn size( & self ) -> Option< u32 > {
                Some ( self._val.len() as u32 )
            }
            #[allow(unused_variables)]
            pub fn mul_elem( & self, other: & Mat4 < $v_type > ) -> Option< Mat4 < $v_type > > {
                Some ( Mat4 { _val: [
                    self._val[0] * other._val[0],
                    self._val[1] * other._val[1],
                    self._val[2] * other._val[2],
                    self._val[3] * other._val[3],
                    self._val[4] * other._val[4],
                    self._val[5] * other._val[5],
                    self._val[6] * other._val[6],
                    self._val[7] * other._val[7],
                    self._val[8] * other._val[8],
                    self._val[9] * other._val[9],
                    self._val[10] * other._val[10],
                    self._val[11] * other._val[11],
                    self._val[12] * other._val[12],
                    self._val[13] * other._val[13],
                    self._val[14] * other._val[14],
                    self._val[15] * other._val[15],],
                               _is_row_major: self._is_row_major } )
            }
            pub fn mul( & self, other: & Mat4 < $v_type > ) -> Option< Mat4 < $v_type > > {
                assert!( self._is_row_major == other._is_row_major );
                let mut m = Mat4 { _val: [0 as $v_type;16] , _is_row_major: self._is_row_major };
                if self._is_row_major {
                    for y in 0..4 { //row index
                        for x in 0..4 { //column index
                            m._val[ y * 4 + x ] = ( self._val[ y * 4 + 0 ] * other._val[ x + 0 ] +
                                                    self._val[ y * 4 + 1 ] * other._val[ x + 4 ] +
                                                    self._val[ y * 4 + 2 ] * other._val[ x + 8 ] +
                                                    self._val[ y * 4 + 3 ] * other._val[ x + 12 ] ) as $v_type;
                        }
                    }
                } else { //column major
                    for x in 0..4 { //column index
                        for y in 0..4 { //row index
                            m._val[ x * 4 + y ] = ( self._val[ y + 0 ] * other._val[ x * 4 + 0 ] +
                                                    self._val[ y + 4 ] * other._val[ x * 4 + 1 ] +
                                                    self._val[ y + 8 ] * other._val[ x * 4 + 2 ] +
                                                    self._val[ y + 12 ] * other._val[ x * 4 + 3 ] ) as $v_type;
                        }
                    }
                }
                Some( m )
            }
            pub fn mul_mat4x1( & self, other: & Mat4x1 < $v_type > ) -> Option< Mat4x1 < $v_type > > {
                let mut m = Mat4x1::default();
                if self._is_row_major {
                    for y in 0..4 { //row index
                        m._val[ y ] = ( self._val[ y * 4 + 0 ] * other._val[ 0 ] +
                                        self._val[ y * 4 + 1 ] * other._val[ 1 ] +
                                        self._val[ y * 4 + 2 ] * other._val[ 2 ] +
                                        self._val[ y * 4 + 3 ] * other._val[ 3 ] ) as $v_type;
                    }
                } else { //column major
                    for x in 0..4 { //column index
                        m._val[ x ] = ( self._val[ x + 0 ] * other._val[ 0 ] +
                                        self._val[ x + 4 ] * other._val[ 1 ] +
                                        self._val[ x + 8 ] * other._val[ 2 ] +
                                        self._val[ x + 12 ] * other._val[ 3 ] ) as $v_type;
                    }
                }
                Some( m )
            }
            #[allow(unused_variables)]
            pub fn div( & self, other: & Mat4 < $v_type > ) -> Option< Mat4 < $v_type > > {
                Some ( Mat4 { _val: [
                    self._val[0] / other._val[0],
                    self._val[1] / other._val[1],
                    self._val[2] / other._val[2],
                    self._val[3] / other._val[3],
                    self._val[4] / other._val[4],
                    self._val[5] / other._val[5],
                    self._val[6] / other._val[6],
                    self._val[7] / other._val[7],
                    self._val[8] / other._val[8],
                    self._val[9] / other._val[9],
                    self._val[10] / other._val[10],
                    self._val[11] / other._val[11],
                    self._val[12] / other._val[12],
                    self._val[13] / other._val[13],
                    self._val[14] / other._val[14],
                    self._val[15] / other._val[15],],
                               _is_row_major: self._is_row_major } )
            }
            #[allow(unused_variables)]
            pub fn plus( & self, other: & Mat4 < $v_type > ) -> Option< Mat4 < $v_type > > {
                Some ( Mat4 { _val: [
                    self._val[0] + other._val[0],
                    self._val[1] + other._val[1],
                    self._val[2] + other._val[2],
                    self._val[3] + other._val[3],
                    self._val[4] + other._val[4],
                    self._val[5] + other._val[5],
                    self._val[6] + other._val[6],
                    self._val[7] + other._val[7],
                    self._val[8] + other._val[8],
                    self._val[9] + other._val[9],
                    self._val[10] + other._val[10],
                    self._val[11] + other._val[11],
                    self._val[12] + other._val[12],
                    self._val[13] + other._val[13],
                    self._val[14] + other._val[14],
                    self._val[15] + other._val[15],],
                               _is_row_major: self._is_row_major } )
            }
            #[allow(unused_variables)]
            pub fn minus( & self, other: & Mat4 < $v_type > ) -> Option< Mat4 < $v_type > > {
                Some ( Mat4 { _val: [
                    self._val[0] - other._val[0],
                    self._val[1] - other._val[1],
                    self._val[2] - other._val[2],
                    self._val[3] - other._val[3],
                    self._val[4] - other._val[4],
                    self._val[5] - other._val[5],
                    self._val[6] - other._val[6],
                    self._val[7] - other._val[7],
                    self._val[8] - other._val[8],
                    self._val[9] - other._val[9],
                    self._val[10] - other._val[10],
                    self._val[11] - other._val[11],
                    self._val[12] - other._val[12],
                    self._val[13] - other._val[13],
                    self._val[14] - other._val[14],
                    self._val[15] - other._val[15],],
                               _is_row_major: self._is_row_major } )
            }
            #[allow(unused_variables)]
            pub fn dot( & self, other: & Mat4 < $v_type > ) -> Option < $v_type > {
                None
            }
            #[allow(unused_variables)]
            pub fn cross( & self, other: & Self ) -> Option< Self > {
                None
            }
            pub fn magnitude( & self ) -> Option < $v_type > {
                unimplemented!();
            }
            pub fn normalize( self ) -> Option < Self > {
                //normalize all elements of the matrix to the last element of the matrix
                let mut m = Mat4 { _val: self._val, _is_row_major: self._is_row_major };
                for x in 0..self._val.len() {
                    m._val[x] = ( m._val[x] as $v_type / m._val[15] as $v_type ) as $v_type;
                }
                Some( m )
            }
            pub fn scale( & self, s: $v_type ) -> Option < Self > {
                let mut m = Mat4 { _val: self._val, _is_row_major: self._is_row_major };
                for x in 0..self._val.len() {
                    m._val[x] = ( m._val[x] as $v_type * s ) as $v_type;
                }
                Some( m )
            }
            pub fn is_equal( & self, other: & Self, error: $v_type ) -> Option< bool > {
                if self._is_row_major != other._is_row_major {
                    return Some( false )
                }
                for x in 0..self._val.len() {
                    let e = ( self._val[x] as $v_type >= (other._val[x] as $v_type - error) ) &&
                        ( self._val[x] as $v_type <= (other._val[x] as $v_type + error) );
                    if !e {
                        return Some( false )
                    }
                }
                Some( true )
            }
            pub fn transpose_self( & mut self ) {
                let copy = self._val.clone();
                for i in 0..16 {
                    if i < 4 {
                        self._val[i*4] = copy[i];
                    } else if i >= 4 && i < 8 {
                        self._val[(i-4)*4 + 1] = copy[i];
                    } else if i >= 8 && i < 12 {
                        self._val[(i-8)*4 + 2] = copy[i];
                    } else {
                        self._val[(i-12)*4 + 3] = copy[i];
                    }
                }
            }
            pub fn transpose( self ) -> Mat4< $v_type > {
                let mut copy = Mat4 { _val: self._val, _is_row_major: self._is_row_major };
                for i in 0..16 {
                    if i < 4 {
                        copy._val[i*4] = self._val[i];
                    } else if i >= 4 && i < 8 {
                        copy._val[(i-4)*4 + 1] = self._val[i];
                    } else if i >= 8 && i < 12 {
                        copy._val[(i-8)*4 + 2] = self._val[i];
                    } else {
                        copy._val[(i-12)*4 + 3] = self._val[i];
                    }
                }
                copy
            }
            /// Returns upper left corner 3x3 matrix
            ///
            /// # Arguments
            ///
            /// * `self` - instance of Mat4x4
            ///
            /// # Remarks
            ///
            /// This is a convenience function to extract the upper left 3x3 matrix.
            ///
            pub fn submat_mat3( &self ) -> Mat3< $v_type > {
                Mat3 { _val: [ self._val[0], self._val[1], self._val[2],
                               self._val[4], self._val[5], self._val[6],
                               self._val[8], self._val[9], self._val[10] ],
                        _is_row_major: self._is_row_major }
            }
        }
    }
}

define_mat1x2!( f32 );
define_mat1x2!( f64 );

define_mat2x1!( f32 );
define_mat2x1!( f64 );

define_mat1x3!( f32 );
define_mat1x3!( f64 );

define_mat3x1!( f32 );
define_mat3x1!( f64 );

define_mat3!( f32 );
define_mat3!( f64 );

define_mat1x4!( f32 );
define_mat1x4!( f64 );

define_mat4x1!( f32 );
define_mat4x1!( f64 );

define_mat4!( f32 );
define_mat4!( f64 );

