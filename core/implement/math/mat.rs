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
impl Default for Mat2x1 < f64 > {
    fn default() -> Mat2x1< f64 > {
        Mat2x1 { _val: [ 0f64; 2 ] }
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
impl Default for Mat3x1 < f64 > {
    fn default() -> Mat3x1< f64 > {
        Mat3x1 { _val: [ 0f64; 3 ] }
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
impl Default for Mat4x1 < f64 > {
    fn default() -> Mat4x1< f64 > {
        Mat4x1 { _val: [ 0f64; 4 ] }
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
impl Default for Mat1x4 < f64 > {
    fn default() -> Mat1x4< f64 > {
        Mat1x4 { _val: [ 0f64; 4 ] }
    }
}

impl Mat2x1 < f64 > {
    pub fn size( & self ) -> Option< u32 > {
        Some ( self._val.len() as u32 )
    }
    pub fn mul_elem( & self, other: & Mat2x1 < f64 > ) -> Option< Mat2x1 < f64 > > {
        Some ( Mat2x1 { _val: [
            self._val[0] * other._val[0],
            self._val[1] * other._val[1] ] } )
    }
    // pub fn mul( & self, other: & Mat2x1 < f64 > ) -> Option< Mat2x1 < f64 > > {
    //     Some ( Mat2x1 { _val: [
    //         self._val[0] * other._val[0],
    //         self._val[1] * other._val[1] ] } )
    // }
    pub fn div( & self, other: & Mat2x1 < f64 > ) -> Option< Mat2x1 < f64 > > {
        Some ( Mat2x1 { _val: [
            self._val[0] / other._val[0],
            self._val[1] / other._val[1] ] } )
    }
    pub fn plus( & self, other: & Mat2x1 < f64 > ) -> Option< Mat2x1 < f64 > > {
        Some ( Mat2x1 { _val: [
            self._val[0] + other._val[0],
            self._val[1] + other._val[1] ] } )
    }
    pub fn minus( & self, other: & Mat2x1 < f64 > ) -> Option< Mat2x1 < f64 > > {
        Some ( Mat2x1 { _val: [
            self._val[0] - other._val[0],
            self._val[1] - other._val[1] ] } )
    }
    pub fn dot( & self, other: & Mat2x1 < f64 > ) -> Option< f64 > {
        Some (
            self._val[0] as f64 * other._val[0] as f64 +
                self._val[1] as f64 * other._val[1] as f64 )
    }
    #[allow(unused_variables)]
    pub fn cross( & self, other: & Self ) -> Option< Self > {
        None
    }
    pub fn magnitude( & self ) -> Option < f64 > {
        Some (
            ( ( self._val[0] as f64 ).powi( 2 ) +
                ( self._val[1] as f64 ).powi( 2 ) ).sqrt() )
    }
    pub fn normalize( self ) -> Option < Self > {
        let m = self.magnitude().expect("magnitude invalid");
        Some ( Mat2x1 { _val: [
            ( self._val[0] as f64 ).div( m ) as f64,
            ( self._val[1] as f64 ).div( m ) as f64 ] } )
    }
    pub fn scale( & self, s: f64 ) -> Option < Self > {
        Some ( Mat2x1 { _val: [
            ( self._val[0] as f64 ).mul( s ) as f64,
            ( self._val[1] as f64 ).mul( s ) as f64 ] } )
    }
    pub fn is_equal( & self, other: & Self, error: f64 ) -> Option< bool > {
        Some( ( self._val[0] as f64 >= other._val[0] as f64 - error ) &&
                ( self._val[0] as f64 <= other._val[0] as f64 + error ) &&
                ( self._val[1] as f64 >= other._val[1] as f64 - error ) &&
                ( self._val[1] as f64 <= other._val[1] as f64 + error ) )
    }
}

impl Mat3x1 < f64 > {
    pub fn size( & self ) -> Option< u32 > {
        Some ( self._val.len() as u32 )
    }
    pub fn mul_elem( & self, other: & Mat3x1 < f64 > ) -> Option< Mat3x1 < f64 > > {
        Some ( Mat3x1 { _val: [
            self._val[0] * other._val[0],
            self._val[1] * other._val[1],
            self._val[2] * other._val[2] ] } )
    }
    // pub fn mul( & self, other: & Mat3x1 < f64 > ) -> Option< Mat3x1 < f64 > > {
    //     Some ( Mat3x1 { _val: [
    //         self._val[0] * other._val[0],
    //         self._val[1] * other._val[1],
    //         self._val[2] * other._val[2] ] } )
    // }
    pub fn div( & self, other: & Mat3x1 < f64 > ) -> Option< Mat3x1 < f64 > > {
        Some ( Mat3x1 { _val: [
            self._val[0] / other._val[0],
            self._val[1] / other._val[1],
            self._val[2] / other._val[2] ] } )
    }
    pub fn plus( & self, other: & Mat3x1 < f64 > ) -> Option< Mat3x1 < f64 > > {
        Some ( Mat3x1 { _val: [
            self._val[0] + other._val[0],
            self._val[1] + other._val[1],
            self._val[2] + other._val[2] ] } )
    }
    pub fn minus( & self, other: & Mat3x1 < f64 > ) -> Option< Mat3x1 < f64 > > {
        Some ( Mat3x1 { _val: [
            self._val[0] - other._val[0],
            self._val[1] - other._val[1],
            self._val[2] - other._val[2] ] } )
    }
    pub fn dot( & self, other: & Mat3x1 < f64 > ) -> Option< f64 > {
        Some (
            self._val[0] as f64 * other._val[0] as f64 +
                self._val[1] as f64 * other._val[1] as f64 +
                self._val[2] as f64 * other._val[2] as f64 )
    }
    pub fn cross( & self, other: & Self ) -> Option< Self > {
        Some ( Mat3x1 { _val: [
            ( ( self._val[1] as f64 * other._val[2] as f64 ) -
                ( self._val[2] as f64 * other._val[1] as f64 ) ) as f64,
            ( ( self._val[2] as f64 * other._val[0] as f64 ) -
                ( self._val[0] as f64 * other._val[2] as f64 ) ) as f64,
            ( ( self._val[0] as f64 * other._val[1] as f64 ) -
                ( self._val[1] as f64 * other._val[0] as f64 ) ) as f64 ] } )
    }
    pub fn magnitude( & self ) -> Option < f64 > {
        Some (
            ( ( self._val[0] as f64 ).powi( 2 ) +
                ( self._val[1] as f64 ).powi( 2 ) +
                ( self._val[2] as f64 ).powi( 2 ) ).sqrt() )
    }
    pub fn normalize( self ) -> Option < Self > {
        let m = self.magnitude().expect("magnitude invalid");
        Some ( Mat3x1 { _val: [
            ( self._val[0] as f64 ).div( m ) as f64,
            ( self._val[1] as f64 ).div( m ) as f64,
            ( self._val[2] as f64 ).div( m ) as f64 ] } )
    }
    pub fn scale( & self, s: f64 ) -> Option < Self > {
        Some ( Mat3x1 { _val: [
            ( self._val[0] as f64 ).mul( s ) as f64,
            ( self._val[1] as f64 ).mul( s ) as f64,
            ( self._val[2] as f64 ).mul( s ) as f64 ] } )
    }
    pub fn is_equal( & self, other: & Self, error: f64 ) -> Option< bool > {
        Some( ( self._val[0] as f64 >= other._val[0] as f64 - error ) &&
                ( self._val[0] as f64 <= other._val[0] as f64 + error ) &&
                ( self._val[1] as f64 >= other._val[1] as f64 - error ) &&
                ( self._val[1] as f64 <= other._val[1] as f64 + error ) &&
                ( self._val[2] as f64 >= other._val[2] as f64 - error ) &&
                ( self._val[2] as f64 <= other._val[2] as f64 + error ) )
    }
}

impl Mat4x1 < f64 > {
    pub fn size( & self ) -> Option< u32 > {
        Some ( self._val.len() as u32 )
    }
    pub fn mul_elem( & self, other: & Mat4x1 < f64 > ) -> Option< Mat4x1 < f64 > > {
        Some ( Mat4x1 { _val: [
            self._val[0] * other._val[0],
            self._val[1] * other._val[1],
            self._val[2] * other._val[2],
            self._val[3] * other._val[3]] } )
    }
    // pub fn mul( & self, other: & Mat4x1 < f64 > ) -> Option< Mat4x1 < f64 > > {
    //     Some ( Mat4x1 { _val: [
    //         self._val[0] * other._val[0],
    //         self._val[1] * other._val[1],
    //         self._val[2] * other._val[2],
    //         self._val[3] * other._val[3]] } )
    // }
    pub fn div( & self, other: & Mat4x1 < f64 > ) -> Option< Mat4x1 < f64 > > {
        Some ( Mat4x1 { _val: [
            self._val[0] / other._val[0],
            self._val[1] / other._val[1],
            self._val[2] / other._val[2],
            self._val[3] / other._val[3]] } )
    }
    pub fn plus( & self, other: & Mat4x1 < f64 > ) -> Option< Mat4x1 < f64 > > {
        Some ( Mat4x1 { _val: [
            self._val[0] + other._val[0],
            self._val[1] + other._val[1],
            self._val[2] + other._val[2],
            self._val[3] + other._val[3]] } )
    }
    pub fn minus( & self, other: & Mat4x1 < f64 > ) -> Option< Mat4x1 < f64 > > {
        Some ( Mat4x1 { _val: [
            self._val[0] - other._val[0],
            self._val[1] - other._val[1],
            self._val[2] - other._val[2],
            self._val[3] - other._val[3]] } )
    }
    pub fn dot( & self, other: & Mat4x1 < f64 > ) -> Option < f64 > {
        Some (
            self._val[0] as f64 * other._val[0] as f64 +
                self._val[1] as f64 * other._val[1] as f64 +
                self._val[2] as f64 * other._val[2] as f64 +
                self._val[3] as f64 * other._val[3] as f64 )
    }
    #[allow(unused_variables)]
    pub fn cross( & self, other: & Self ) -> Option< Self > {
        None
    }
    pub fn magnitude( & self ) -> Option < f64 > {
        Some (
            ( ( self._val[0] as f64 ).powi( 2 ) +
                ( self._val[1] as f64 ).powi( 2 ) +
                ( self._val[2] as f64 ).powi( 2 ) +
                ( self._val[3] as f64 ).powi( 2 ) ).sqrt() )
    }
    pub fn normalize( self ) -> Option < Self > {
        let m = self.magnitude().expect("magnitude invalid");
        Some ( Mat4x1 { _val: [
            ( self._val[0] as f64 ).div( m ) as f64,
            ( self._val[1] as f64 ).div( m ) as f64,
            ( self._val[2] as f64 ).div( m ) as f64,
            ( self._val[3] as f64 ).div( m ) as f64 ] } )
    }
    pub fn scale( & self, s: f64 ) -> Option < Self > {
        Some ( Mat4x1 { _val: [
            ( self._val[0] as f64 ).mul( s ) as f64,
            ( self._val[1] as f64 ).mul( s ) as f64,
            ( self._val[2] as f64 ).mul( s ) as f64,
            ( self._val[3] as f64 ).mul( s ) as f64 ] } )
    }
    pub fn is_equal( & self, other: & Self, error: f64 ) -> Option< bool > {
        Some( ( self._val[0] as f64 >= other._val[0] as f64 - error ) &&
                ( self._val[0] as f64 <= other._val[0] as f64 + error ) &&
                ( self._val[1] as f64 >= other._val[1] as f64 - error ) &&
                ( self._val[1] as f64 <= other._val[1] as f64 + error ) &&
                ( self._val[2] as f64 >= other._val[2] as f64 - error ) &&
                ( self._val[2] as f64 <= other._val[2] as f64 + error ) &&
                ( self._val[3] as f64 >= other._val[3] as f64 - error ) &&
                ( self._val[3] as f64 <= other._val[3] as f64 + error ) )
    }
}

impl Mat1x4 < f64 > {
    pub fn size( & self ) -> Option< u32 > {
        Some ( self._val.len() as u32 )
    }
    pub fn mul_elem( & self, other: & Mat1x4 < f64 > ) -> Option< Mat1x4 < f64 > > {
        Some ( Mat1x4 { _val: [
            self._val[0] * other._val[0],
            self._val[1] * other._val[1],
            self._val[2] * other._val[2],
            self._val[3] * other._val[3]] } )
    }
    pub fn mul_mat4x4( & self, other: & Mat4 < f64 > ) -> Option< Mat1x4 < f64 > > {
        let mut m = Mat1x4::default();
        if other._is_row_major {
            for x in 0..4 {
                m._val[ x ] = ( other._val[ x + 0 ] * self._val[ 0 ] +
                                other._val[ x + 4 ] * self._val[ 1 ] +
                                other._val[ x + 8 ] * self._val[ 2 ] +
                                other._val[ x + 12 ] * self._val[ 3 ] ) as f64;
            }
        } else { //column major
            for y in 0..4 {
                m._val[ y ] = ( other._val[ y * 4 + 0 ] * self._val[ 0 ] +
                                other._val[ y * 4 + 1 ] * self._val[ 1 ] +
                                other._val[ y * 4 + 2 ] * self._val[ 2 ] +
                                other._val[ y * 4 + 3 ] * self._val[ 3 ] ) as f64;
            }
        }
        Some( m )
    }
    pub fn div( & self, other: & Mat1x4 < f64 > ) -> Option< Mat1x4 < f64 > > {
        Some ( Mat1x4 { _val: [
            self._val[0] / other._val[0],
            self._val[1] / other._val[1],
            self._val[2] / other._val[2],
            self._val[3] / other._val[3]] } )
    }
    pub fn plus( & self, other: & Mat1x4 < f64 > ) -> Option< Mat1x4 < f64 > > {
        Some ( Mat1x4 { _val: [
            self._val[0] + other._val[0],
            self._val[1] + other._val[1],
            self._val[2] + other._val[2],
            self._val[3] + other._val[3]] } )
    }
    pub fn minus( & self, other: & Mat1x4 < f64 > ) -> Option< Mat1x4 < f64 > > {
        Some ( Mat1x4 { _val: [
            self._val[0] - other._val[0],
            self._val[1] - other._val[1],
            self._val[2] - other._val[2],
            self._val[3] - other._val[3]] } )
    }
    pub fn dot( & self, other: & Mat1x4 < f64 > ) -> Option < f64 > {
        Some (
            self._val[0] as f64 * other._val[0] as f64 +
                self._val[1] as f64 * other._val[1] as f64 +
                self._val[2] as f64 * other._val[2] as f64 +
                self._val[3] as f64 * other._val[3] as f64 )
    }
    #[allow(unused_variables)]
    pub fn cross( & self, other: & Self ) -> Option< Self > {
        None
    }
    pub fn magnitude( & self ) -> Option < f64 > {
        Some (
            ( ( self._val[0] as f64 ).powi( 2 ) +
                ( self._val[1] as f64 ).powi( 2 ) +
                ( self._val[2] as f64 ).powi( 2 ) +
                ( self._val[3] as f64 ).powi( 2 ) ).sqrt() )
    }
    pub fn normalize( self ) -> Option < Self > {
        let m = self.magnitude().expect("magnitude invalid");
        Some ( Mat1x4 { _val: [
            ( self._val[0] as f64 ).div( m ) as f64,
            ( self._val[1] as f64 ).div( m ) as f64,
            ( self._val[2] as f64 ).div( m ) as f64,
            ( self._val[3] as f64 ).div( m ) as f64 ] } )
    }
    pub fn scale( & self, s: f64 ) -> Option < Self > {
        Some ( Mat1x4 { _val: [
            ( self._val[0] as f64 ).mul( s ) as f64,
            ( self._val[1] as f64 ).mul( s ) as f64,
            ( self._val[2] as f64 ).mul( s ) as f64,
            ( self._val[3] as f64 ).mul( s ) as f64 ] } )
    }
    pub fn is_equal( & self, other: & Self, error: f64 ) -> Option< bool > {
        Some( ( self._val[0] as f64 >= other._val[0] as f64 - error ) &&
                ( self._val[0] as f64 <= other._val[0] as f64 + error ) &&
                ( self._val[1] as f64 >= other._val[1] as f64 - error ) &&
                ( self._val[1] as f64 <= other._val[1] as f64 + error ) &&
                ( self._val[2] as f64 >= other._val[2] as f64 - error ) &&
                ( self._val[2] as f64 <= other._val[2] as f64 + error ) &&
                ( self._val[3] as f64 >= other._val[3] as f64 - error ) &&
                ( self._val[3] as f64 <= other._val[3] as f64 + error ) )
    }
}

#[derive(Debug, Copy, Clone)]
pub struct Mat4 < T > where T: Copy + Clone {
    pub _val: [ T ; 16 ],
    pub _is_row_major: bool,
}

impl Default for Mat4 < f64 > {
    fn default() -> Mat4< f64 > {
        Mat4 { _val: [ 0f64; 16 ],
               _is_row_major: true,
        }
    }
}
    
impl Mat4 < f64 > {
    #[allow(dead_code)]
    pub fn init( arr: [ f64; 16 ], row_major: bool ) -> Mat4 < f64 > {
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
    pub fn index( & self, m: u64, n: u64 ) -> Option< f64 > {
        if self._is_row_major {
            Some( self._val[ ( m as usize ) * 4 + n as usize ] )
        } else {
            Some( self._val[ ( m as usize ) + ( n as usize ) * 4 ] )
        }
    }
    #[allow(unused_variables)]
    pub fn size( & self ) -> Option< u32 > {
        Some ( self._val.len() as u32 )
    }
    #[allow(unused_variables)]
    pub fn mul_elem( & self, other: & Mat4 < f64 > ) -> Option< Mat4 < f64 > > {
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
    pub fn mul( & self, other: & Mat4 < f64 > ) -> Option< Mat4 < f64 > > {
        assert!( self._is_row_major == other._is_row_major );
        let mut m = Mat4 { _val: self._val, _is_row_major: self._is_row_major };
        if self._is_row_major {
            for y in 0..4 { //row index
                for x in 0..4 { //column index
                    m._val[ y * 4 + x ] = ( self._val[ y * 4 + 0 ] * other._val[ x + 0 ] +
                                            self._val[ y * 4 + 1 ] * other._val[ x + 4 ] +
                                            self._val[ y * 4 + 2 ] * other._val[ x + 8 ] +
                                            self._val[ y * 4 + 3 ] * other._val[ x + 12 ] ) as f64;
                }
            }
        } else { //column major
            for x in 0..4 { //column index
                for y in 0..4 { //row index
                    m._val[ x * 4 + y ] = ( self._val[ y + 0 ] * other._val[ x * 4 + 0 ] +
                                            self._val[ y + 4 ] * other._val[ x * 4 + 1 ] +
                                            self._val[ y + 8 ] * other._val[ x * 4 + 2 ] +
                                            self._val[ y + 12 ] * other._val[ x * 4 + 3 ] ) as f64;
                }
            }
        }
        Some( m )
    }
    pub fn mul_mat4x1( & self, other: & Mat4x1 < f64 > ) -> Option< Mat4x1 < f64 > > {
        let mut m = Mat4x1::default();
        if self._is_row_major {
            for y in 0..4 { //row index
                m._val[ y ] = ( self._val[ y * 4 + 0 ] * other._val[ 0 ] +
                                self._val[ y * 4 + 1 ] * other._val[ 1 ] +
                                self._val[ y * 4 + 2 ] * other._val[ 2 ] +
                                self._val[ y * 4 + 3 ] * other._val[ 3 ] ) as f64;
            }
        } else { //column major
            for x in 0..4 { //column index
                m._val[ x ] = ( self._val[ x + 0 ] * other._val[ 0 ] +
                                self._val[ x + 4 ] * other._val[ 1 ] +
                                self._val[ x + 8 ] * other._val[ 2 ] +
                                self._val[ x + 12 ] * other._val[ 3 ] ) as f64;
            }
        }
        Some( m )
    }
    #[allow(unused_variables)]
    pub fn div( & self, other: & Mat4 < f64 > ) -> Option< Mat4 < f64 > > {
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
    pub fn plus( & self, other: & Mat4 < f64 > ) -> Option< Mat4 < f64 > > {
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
    pub fn minus( & self, other: & Mat4 < f64 > ) -> Option< Mat4 < f64 > > {
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
    pub fn dot( & self, other: & Mat4 < f64 > ) -> Option < f64 > {
        None
    }
    #[allow(unused_variables)]
    pub fn cross( & self, other: & Self ) -> Option< Self > {
        None
    }
    pub fn magnitude( & self ) -> Option < f64 > {
        unimplemented!();
    }
    pub fn normalize( self ) -> Option < Self > {
        //normalize all elements of the matrix to the last element of the matrix
        let mut m = Mat4 { _val: self._val, _is_row_major: self._is_row_major };
        for x in 0..self._val.len() {
            m._val[x] = ( m._val[x] as f64 / m._val[15] as f64 ) as f64;
        }
        Some( m )
    }
    pub fn scale( & self, s: f64 ) -> Option < Self > {
        let mut m = Mat4 { _val: self._val, _is_row_major: self._is_row_major };
        for x in 0..self._val.len() {
            m._val[x] = ( m._val[x] as f64 * s ) as f64;
        }
        Some( m )
    }
    pub fn is_equal( & self, other: & Self, error: f64 ) -> Option< bool > {
        let mut ret = true;
        for x in 0..self._val.len() {
            let e = ( self._val[x] as f64 >= other._val[x] as f64 - error ) &&
                ( self._val[x] as f64 <= other._val[x] as f64 + error );
            ret = ret && e;
            if !ret {
                break;
            }
        }
        Some( ret )
    }
}
