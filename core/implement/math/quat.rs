#[allow(unused_imports)]
use std::ops::Index;
#[allow(unused_imports)]
use std::ops::IndexMut;
#[allow(unused_imports)]
use std::ops::Div;
#[allow(unused_imports)]
use std::ops::Mul;

use std::f64::consts::PI;

use implement::math::mat::Mat3x1;
use implement::math::mat::Mat4;

#[derive(Debug, Copy, Clone)]
struct Quat {
    pub _x: f64,
    pub _y: f64,
    pub _z: f64,
    pub _w: f64,
}
impl Quat {
    #[allow(dead_code)]
    pub fn init() -> Quat {
        Quat { _x: 0f64, _y: 0f64, _z: 0f64, _w: 0f64 }
    }
    #[allow(dead_code)]
    pub fn init_from_vals( x: f64, y:f64, z:f64, w: f64 ) -> Quat {
        Quat { _x: x, _y: y, _z: z, _w: w }
    }
    #[allow(dead_code)]
    pub fn init_from_vals_auto_w( x: f64, y:f64, z:f64 ) -> Quat {
        let w = 1f64 - x*x - y*y - z*z;
        if w < 0f64 {
            return Quat { _x: x, _y: y, _z: z, _w: w }
        } else {
            return Quat { _x: x, _y: y, _z: z, _w: -1f64 * w.sqrt() }
        }
    }
    #[allow(dead_code)]
    pub fn init_from_translation( trans: Mat3x1< f64 > ) -> Quat {
        Quat { _x: trans[0] / 2f64, _y: trans[1] / 2f64, _z: trans[2] / 2f64, _w: 0f64 }
    }
    #[allow(dead_code)]
    pub fn to_translation_matrix( & self, row_major: bool ) -> Mat4< f64 > {
        match row_major {
            true => Mat4::init( [ 0f64, 0f64, 0f64, self._x,
                                  0f64, 0f64, 0f64, self._y,
                                  0f64, 0f64, 0f64, self._z,
                                  0f64, 0f64, 0f64, 1f64 ], true ),
            _ => Mat4::init( [ 0f64, 0f64, 0f64, 0f64,
                               0f64, 0f64, 0f64, 0f64,
                               0f64, 0f64, 0f64, 0f64,
                               self._x, self._y, self._z, 1f64 ], false )
        }
    }
    #[allow(dead_code)]
    pub fn to_rotation_matrix( & self, row_major: bool ) -> Mat4< f64 > {
        //this assums that the current quaternion is a unit quaternion
        match row_major {
            true => Mat4::init( [ 1f64 - 2f64 * ( self._y * self._y + self._z * self._z ), //first row
                                      2f64 * ( self._x * self._y - self._z * self._w ),
                                      2f64 * ( self._x * self._z + self._y * self._w ),
                                      0f64,
                                  2f64 * (self._x * self._y + self._z * self._w), //second row
                                      1f64 - 2f64 * ( self._x * self._x + self._z * self._z ),
                                      2f64 * ( self._y * self._z - self._x * self._w ),
                                      0f64,
                                  2f64 * (self._x * self._z - self._y * self._w), //third row
                                      2f64 * (self._z * self._y + self._x * self._w ),
                                      1f64 - 2f64 * ( self._x * self._x + self._y * self._y ),
                                      0f64,
                                  0f64, 0f64, 0f64, 1f64 ], true ), //last row
            _ => Mat4::init( [ 1f64 - 2f64 * ( self._y * self._y + self._z * self._z ), //first column
                                   2f64 * (self._x * self._y + self._z * self._w),
                                   2f64 * (self._x * self._z - self._y * self._w),
                                   0f64,
                               2f64 * ( self._x * self._y - self._z * self._w ), //second column
                                   1f64 - 2f64 * ( self._x * self._x + self._z * self._z ),
                                   2f64 * (self._z * self._y + self._x * self._w ),
                                   0f64,
                               2f64 * ( self._x * self._z + self._y * self._w ), //third column
                                   2f64 * ( self._y * self._z - self._x * self._w ),
                                   1f64 - 2f64 * ( self._x * self._x + self._y * self._y ),
                                   0f64,
                               0f64, 0f64, 0f64, 1f64 ], false ) //last column
        }
    }
    #[allow(dead_code)]
    pub fn to_axis_angle( & self ) -> ( Mat3x1< f64 >, f64 ) {
        let k = ( 1f64 - self._w * self._w ).sqrt();
        if k < 0.000001 {
            return ( Mat3x1 { _val: [ 1f64, 0f64, 0f64 ] }, 0f64 ) // ( <x,y,z>, angle )
        } else {
            let vec_x = self._x / k;
            let vec_y = self._y / k;
            let vec_z = self._z / k;
            let l = ( vec_x * vec_x + vec_y * vec_y + vec_z * vec_z ).sqrt();
            assert!( l!= 0f64 );
            return ( Mat3x1 { _val: [ vec_x / l, vec_y / l, vec_z / l ] }, 2f64 * self._w.acos() )
        }
    }
    #[allow(dead_code)]
    pub fn init_from_axis_angle_degree( axis_angle: ( Mat3x1< f64 >, f64 ) ) -> Quat {
        let ( axis, angle ) = axis_angle;
        let radians = angle / 180f64 * PI;
        Self::init_from_axis_angle_radian( ( axis, radians ) )
    }
    #[allow(dead_code)]
    pub fn init_from_axis_angle_radian( axis_angle: ( Mat3x1< f64 >, f64 ) ) -> Quat {
        let ( axis, radians ) = axis_angle;
        let axis_adjust = axis.normalize().expect("axis normalize invalid");
        let sine_half = ( radians / 2f64 ).sin();
        Quat { _x: axis_adjust[0] * sine_half,
               _y: axis_adjust[1] * sine_half,
               _z: axis_adjust[2] * sine_half,
               _w: ( radians / 2f64 ).cos()
        }
    }
    #[allow(dead_code)]
    pub fn mul_vector( & self, v: Mat3x1< f64 > ) -> Quat {
        Quat { _x: ( self._w * v[0] + self._y * v[2] + self._z * v[1] ),
               _y: ( self._w * v[1] + self._z * v[0] + self._x * v[2] ),
               _z: ( self._w * v[2] + self._x * v[1] + self._y * v[0] ),
               _w: ( -self._x * v[0] - self._y * v[1] - self._z * v[2] )
        }
    }
    #[allow(dead_code)]
    pub fn rotate_point( & self, p: Mat3x1< f64 > ) -> Mat3x1< f64 > {
        let inv = self.inverse();
        let inv_normalized = inv.normalize();
        let temp = self.mul_vector( p );
        let temp2 = temp.mul( inv_normalized );
        Mat3x1 { _val: [ temp2._x, temp2._y, temp2._z ] }
    }
    #[allow(dead_code)]
    pub fn add( & self, other: Self ) -> Quat {
        Quat { _x: self._x + other._x,
               _y: self._y + other._y,
               _z: self._z + other._z,
               _w: self._w + other._w
        }
    }
    #[allow(dead_code)]
    pub fn minus( & self, other: Self ) -> Quat {
        Quat { _x: self._x - other._x,
               _y: self._y - other._y,
               _z: self._z - other._z,
               _w: self._w - other._w
        }
    }
    #[allow(dead_code)]
    pub fn mul( & self, other: Self ) -> Quat {
        Quat { _x: self._w * other._x + self._x * other._w + self._y * other._z - self._z * other._y,
               _y: self._w * other._y - self._x * other._z + self._y * other._w + self._z * other._x,
               _z: self._w * other._z + self._x * other._y - self._y * other._x + self._z * other._w,
               _w: self._w * other._w - self._x * other._x - self._y * other._y - self._z * other._z
        }
    }
    #[allow(dead_code)]
    pub fn length_squared( & self ) -> f64 {
        self._x * self._x + self._y * self._y + self._z * self._z + self._w * self._w
    }
    #[allow(dead_code)]
    pub fn length( & self ) -> f64 {
        ( self._x * self._x + self._y * self._y + self._z * self._z + self._w * self._w ).sqrt()
    }
    #[allow(dead_code)]
    pub fn normalize( & self ) -> Quat {
        let l = self.length();
        assert!( l != 0f64 );
        Quat { _x: self._x/l, _y: self._y/l, _z: self._z/l, _w: self._w/l }
    }
    #[allow(dead_code)]
    pub fn ln( & self ) -> Quat {
        let l = self.length();
        let w_ln = self._w.ln();
        //normalize x,y,z vector -> v/||v||
        let vec_length = ( self._x * self._x + self._y * self._y + self._z * self._z ).sqrt();
        assert!( vec_length != 0f64 );
        let vec_x = self._x / vec_length;
        let vec_y = self._y / vec_length;
        let vec_z = self._z / vec_length;
        //scale x,y,z by acos( w/l )
        let s = (w_ln / l).acos();
        Quat { _x: vec_x * s, _y: vec_y * s, _z: vec_z * s, _w: w_ln }
    }
    #[allow(dead_code)]
    pub fn pow( & self, t: f64 ) -> Quat {
        let vec_length = ( self._x * self._x + self._y * self._y + self._z * self._z ).sqrt();
        assert!( vec_length != 0f64 );
        let vec_x = self._x / vec_length;
        let vec_y = self._y / vec_length;
        let vec_z = self._z / vec_length;
        let l = self.length();
        //original angle
        let alpha = (self._w / l).acos();
        //new angle
        let beta = t * alpha;
        let coeff = l.powf( t );
        Quat { _x: coeff * vec_x * beta.sin(),
               _y: coeff * vec_y * beta.sin(),
               _z: coeff * vec_z * beta.sin(),
               _w: coeff * beta.cos() }
    }
    #[allow(dead_code)]
    pub fn negate( & self ) -> Quat {
        Quat { _x: -self._x, _y: -self._y, _z: -self._z, _w: -self._w }
    }
    #[allow(dead_code)]
    pub fn conjugate( & self ) -> Quat {
        Quat { _x: -self._x, _y: -self._y, _z: -self._z, _w: self._w }
    }
    #[allow(dead_code)]
    pub fn scale( & self, s: f64 ) -> Quat {
        Quat { _x: self._x * s, _y: self._y * s, _z: self._z * s, _w: self._w * s }
    }
    #[allow(dead_code)]
    pub fn inverse( & self ) -> Quat {
        let conj = self.conjugate();
        let norm = conj.length_squared();
        assert!( norm != 0f64 );
        conj.scale( 1f64/norm )
    }
    #[allow(dead_code)]
    pub fn interpolate_linear( start: Quat, end: Quat, t: f64 ) -> Quat {
        let clamp_upper = if t > 1f64 { 1f64 } else { t };
        let clamp = if clamp_upper < 0f64 { 0f64 } else { clamp_upper };
        Quat { _x: start._x * (1f64 - clamp) + end._x * clamp,
               _y: start._y * (1f64 - clamp) + end._y * clamp,
               _z: start._z * (1f64 - clamp) + end._z * clamp,
               _w: start._w * (1f64 - clamp) + end._w * clamp
        }
    }
    #[allow(dead_code)]
    pub fn interpolate_slerp( start: Quat, end: Quat, t: f64 ) -> Quat {
        let t_clamp_upper = if t > 1f64 { 1f64 } else { t };
        let t_clamp = if t_clamp_upper < 0f64 { 0f64 } else { t_clamp_upper };

        let cos_omega = start._w * end._w + start._x * end._x + start._y * end._y + start._z * end._z;      
        let cos_omega_adjust = if cos_omega < 0f64 {
            -cos_omega
        }else{
            cos_omega
        };

        let end_adjust = if cos_omega < 0f64 {
            //inverted
            Quat { _x: -end._x, _y: -end._y, _z: -end._z, _w: -end._w }
        } else {
            Quat { _x: end._x, _y: end._y, _z: end._z, _w: end._w }
        };

        let ( k0, k1 ) =
            if cos_omega_adjust > 0.9999f64 {
                ( 1f64 - t_clamp, t_clamp )
            } else {
                let sin_omega = (1f64 - cos_omega * cos_omega).sqrt();
                let omega = sin_omega.atan2( cos_omega );
                let inv_sin_omega = 1f64 / sin_omega;
                
                ( ((1f64 - t_clamp) * omega).sin() * inv_sin_omega,
                    (t_clamp * omega).sin() * inv_sin_omega )
            };
        Quat { _x: start._x * k0 + end_adjust._x * k1,
               _y: start._y * k0 + end_adjust._y * k1,
               _z: start._z * k0 + end_adjust._z * k1,
               _w: start._w * k0 + end_adjust._w * k1
        }
    }
}
