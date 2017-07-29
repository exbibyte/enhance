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
pub struct Quat < T > {
    pub _x: T,
    pub _y: T,
    pub _z: T,
    pub _w: T,
}

macro_rules! define_quat {
    ($v_type: tt) => {
        
        impl Default for Quat< $v_type > {
            fn default() -> Quat< $v_type > {
                Quat { _x: 0 as $v_type, _y: 0 as $v_type, _z: 0 as $v_type, _w: 1 as $v_type }
            }
        }

        impl Quat< $v_type > {
            #[allow(dead_code)]
            pub fn init() -> Quat< $v_type > {
                Quat { _x: 0 as $v_type, _y: 0 as $v_type, _z: 0 as $v_type, _w: 1 as $v_type }
            }
            #[allow(dead_code)]
            pub fn init_from_vals( x: $v_type, y:$v_type, z:$v_type, w: $v_type ) -> Quat< $v_type > {
                let q = Quat { _x: x, _y: y, _z: z, _w: w };
                q.normalize()
            }
            #[allow(dead_code)]
            pub fn init_from_vals_auto_w( x: $v_type, y:$v_type, z:$v_type ) -> Quat< $v_type > {
                let w = 1 as $v_type - x * x - y * y - z * z;
                if w < 0 as $v_type {
                    return Quat { _x: x, _y: y, _z: z, _w: w }
                } else {
                    return Quat { _x: x, _y: y, _z: z, _w: -1 as $v_type * w.sqrt() }
                }
            }
            #[allow(dead_code)]
            pub fn init_from_translation( trans: Mat3x1< $v_type > ) -> Quat< $v_type > {
                Quat { _x: trans[0] / 2 as $v_type, _y: trans[1] / 2 as $v_type, _z: trans[2] / 2 as $v_type, _w: 0 as $v_type }
            }
            #[allow(dead_code)]
            pub fn to_translation_matrix( & self, row_major: bool ) -> Mat4< $v_type > {
                match row_major {
                    true => Mat4::<$v_type>::init( [ 0 as $v_type, 0 as $v_type, 0 as $v_type, self._x,
                                                 0 as $v_type, 0 as $v_type, 0 as $v_type, self._y,
                                                 0 as $v_type, 0 as $v_type, 0 as $v_type, self._z,
                                                 0 as $v_type, 0 as $v_type, 0 as $v_type, 1 as $v_type ], true ),
                    _ => Mat4::<$v_type>::init( [ 0 as $v_type, 0 as $v_type, 0 as $v_type, 0 as $v_type,
                                              0 as $v_type, 0 as $v_type, 0 as $v_type, 0 as $v_type,
                                              0 as $v_type, 0 as $v_type, 0 as $v_type, 0 as $v_type,
                                              self._x, self._y, self._z, 1 as $v_type ], false )
                }
            }
            #[allow(dead_code)]
            pub fn to_rotation_matrix( & self, row_major: bool ) -> Mat4< $v_type > {
                //this assums that the current quaternion is a unit quaternion
                match row_major {
                    true => Mat4::<$v_type>::init( [ 1 as $v_type - 2 as $v_type * ( self._y * self._y + self._z * self._z ), //first row
                                                 2 as $v_type * ( self._x * self._y - self._z * self._w ),
                                                 2 as $v_type * ( self._x * self._z + self._y * self._w ),
                                                 0 as $v_type,
                                                 2 as $v_type * (self._x * self._y + self._z * self._w), //second row
                                                 1 as $v_type - 2 as $v_type * ( self._x * self._x + self._z * self._z ),
                                                 2 as $v_type * ( self._y * self._z - self._x * self._w ),
                                                 0 as $v_type,
                                                 2 as $v_type * (self._x * self._z - self._y * self._w), //third row
                                                 2 as $v_type * (self._z * self._y + self._x * self._w ),
                                                 1 as $v_type - 2 as $v_type * ( self._x * self._x + self._y * self._y ),
                                                 0 as $v_type,
                                                 0 as $v_type, 0 as $v_type, 0 as $v_type, 1 as $v_type ], true ), //last row
                    _ => Mat4::<$v_type>::init( [ 1 as $v_type - 2 as $v_type * ( self._y * self._y + self._z * self._z ), //first column
                                              2 as $v_type * (self._x * self._y + self._z * self._w),
                                              2 as $v_type * (self._x * self._z - self._y * self._w),
                                              0 as $v_type,
                                              2 as $v_type * ( self._x * self._y - self._z * self._w ), //second column
                                              1 as $v_type - 2 as $v_type * ( self._x * self._x + self._z * self._z ),
                                              2 as $v_type * (self._z * self._y + self._x * self._w ),
                                              0 as $v_type,
                                              2 as $v_type * ( self._x * self._z + self._y * self._w ), //third column
                                              2 as $v_type * ( self._y * self._z - self._x * self._w ),
                                              1 as $v_type - 2 as $v_type * ( self._x * self._x + self._y * self._y ),
                                              0 as $v_type,
                                              0 as $v_type, 0 as $v_type, 0 as $v_type, 1 as $v_type ], false ) //last column
                }
            }
            #[allow(dead_code)]
            pub fn to_axis_angle( & self ) -> ( Mat3x1< $v_type >, $v_type ) {
                let k = ( 1 as $v_type - self._w * self._w ).sqrt();
                if k < 0.000001 {
                    return ( Mat3x1 { _val: [ 1 as $v_type, 0 as $v_type, 0 as $v_type ] }, 0 as $v_type ) // ( <x,y,z>, angle )
                } else {
                    let vec_x = self._x / k;
                    let vec_y = self._y / k;
                    let vec_z = self._z / k;
                    let l = ( vec_x * vec_x + vec_y * vec_y + vec_z * vec_z ).sqrt();
                    assert!( l!= 0 as $v_type );
                    return ( Mat3x1 { _val: [ vec_x / l, vec_y / l, vec_z / l ] }, 2 as $v_type * self._w.acos() )
                }
            }
            #[allow(dead_code)]
            pub fn init_from_axis_angle_degree( axis_angle: ( Mat3x1< $v_type >, $v_type ) ) -> Quat< $v_type > {
                let ( axis, angle ) = axis_angle;
                let radians = angle / 180 as $v_type * PI as $v_type;
                Self::init_from_axis_angle_radian( ( axis, radians ) )
            }
            #[allow(dead_code)]
            pub fn init_from_axis_angle_radian( axis_angle: ( Mat3x1< $v_type >, $v_type ) ) -> Quat< $v_type > {
                let ( axis, radians ) = axis_angle;
                let axis_adjust = axis.normalize().expect("axis normalize invalid");
                let sine_half = ( radians / 2 as $v_type ).sin();
                Quat { _x: axis_adjust[0] * sine_half,
                        _y: axis_adjust[1] * sine_half,
                        _z: axis_adjust[2] * sine_half,
                        _w: ( radians / 2 as $v_type ).cos()
                }
            }
            #[allow(dead_code)]
            pub fn rotate_vector( & self, p: Mat3x1< $v_type > ) -> Mat3x1< $v_type > {
                let conj = self.conjugate();
                let quat_p = Quat { _x: p._val[0], _y: p._val[1], _z: p._val[2], _w: 0 as $v_type };
                let temp = self.mul( quat_p );
                let temp2 = temp.mul( conj );
                Mat3x1 { _val: [ temp2._x, temp2._y, temp2._z ] }
            }
            #[allow(dead_code)]
            pub fn reflection_in_plane( & self, p: Mat3x1< $v_type > ) -> Mat3x1< $v_type > {
                let quat_p = Quat { _x: p._val[0], _y: p._val[1], _z: p._val[2], _w: 0 as $v_type };
                let temp = self.mul( quat_p );
                let temp2 = temp.mul( *self );
                Mat3x1 { _val: [ temp2._x, temp2._y, temp2._z ] }
            }
            #[allow(dead_code)]
            pub fn parallel_component_of_plane( & self, p: Mat3x1< $v_type > ) -> Mat3x1< $v_type > {
                let quat_p = Quat { _x: p._val[0], _y: p._val[1], _z: p._val[2], _w: 0 as $v_type };
                let temp = self.mul( quat_p );
                let temp2 = temp.mul( *self );
                let temp3 = quat_p.add( temp2 );
                let temp4 = temp3.scale( 0.5 as $v_type );
                Mat3x1 { _val: [ temp4._x, temp4._y, temp4._z ] }
            }
            #[allow(dead_code)]
            pub fn orthogonal_component_of_plane( & self, p: Mat3x1< $v_type > ) -> Mat3x1< $v_type > {
                let quat_p = Quat { _x: p._val[0], _y: p._val[1], _z: p._val[2], _w: 0 as $v_type };
                let temp = self.mul( quat_p );
                let temp2 = temp.mul( *self );
                let temp3 = quat_p.minus( temp2 );
                let temp4 = temp3.scale( 0.5 as $v_type );
                Mat3x1 { _val: [ temp4._x, temp4._y, temp4._z ] }
            }
            #[allow(dead_code)]
            pub fn add( & self, other: Self ) -> Quat< $v_type > {
                Quat { _x: self._x + other._x,
                        _y: self._y + other._y,
                        _z: self._z + other._z,
                        _w: self._w + other._w
                }
            }
            #[allow(dead_code)]
            pub fn minus( & self, other: Self ) -> Quat< $v_type > {
                Quat { _x: self._x - other._x,
                        _y: self._y - other._y,
                        _z: self._z - other._z,
                        _w: self._w - other._w
                }
            }
            #[allow(dead_code)]
            pub fn mul( & self, other: Self ) -> Quat< $v_type > {
                Quat { _x: self._w * other._x + self._x * other._w + self._y * other._z - self._z * other._y,
                        _y: self._w * other._y - self._x * other._z + self._y * other._w + self._z * other._x,
                        _z: self._w * other._z + self._x * other._y - self._y * other._x + self._z * other._w,
                        _w: self._w * other._w - self._x * other._x - self._y * other._y - self._z * other._z
                }
            }
            #[allow(dead_code)]
            pub fn length_squared( & self ) -> $v_type {
                self._x * self._x + self._y * self._y + self._z * self._z + self._w * self._w
            }
            #[allow(dead_code)]
            pub fn length( & self ) -> $v_type {
                ( self._x * self._x + self._y * self._y + self._z * self._z + self._w * self._w ).sqrt()
            }
            #[allow(dead_code)]
            pub fn normalize( & self ) -> Quat< $v_type > {
                let l = self.length();
                assert!( l != 0 as $v_type );
                Quat { _x: self._x/l, _y: self._y/l, _z: self._z/l, _w: self._w/l }
            }
            #[allow(dead_code)]
            pub fn ln( & self ) -> Quat< $v_type > {
                let l = self.length();
                let w_ln = self._w.ln();
                //normalize x,y,z vector -> v/||v||
                let vec_length = ( self._x * self._x + self._y * self._y + self._z * self._z ).sqrt();
                assert!( vec_length != 0 as $v_type );
                let vec_x = self._x / vec_length;
                let vec_y = self._y / vec_length;
                let vec_z = self._z / vec_length;
                //scale x,y,z by acos( w/l )
                let s = (w_ln / l).acos();
                Quat { _x: vec_x * s, _y: vec_y * s, _z: vec_z * s, _w: w_ln }
            }
            #[allow(dead_code)]
            pub fn pow( & self, t: $v_type ) -> Quat< $v_type > {
                let vec_length = ( self._x * self._x + self._y * self._y + self._z * self._z ).sqrt();
                assert!( vec_length != 0 as $v_type );
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
            pub fn negate( & self ) -> Quat< $v_type > {
                Quat { _x: -self._x, _y: -self._y, _z: -self._z, _w: -self._w }
            }
            #[allow(dead_code)]
            pub fn conjugate( & self ) -> Quat< $v_type > {
                Quat { _x: -self._x, _y: -self._y, _z: -self._z, _w: self._w }
            }
            #[allow(dead_code)]
            pub fn scale( & self, s: $v_type ) -> Quat< $v_type > {
                Quat { _x: self._x * s, _y: self._y * s, _z: self._z * s, _w: self._w * s }
            }
            #[allow(dead_code)]
            pub fn inverse( & self ) -> Quat< $v_type > {
                let conj = self.conjugate();
                let norm = conj.length_squared();
                assert!( norm != 0 as $v_type );
                conj.scale( 1 as $v_type/norm )
            }
            #[allow(dead_code)]
            pub fn interpolate_linear( start: Quat< $v_type >, end: Quat< $v_type >, t: $v_type ) -> Quat< $v_type > {
                let clamp_upper = if t > 1 as $v_type { 1 as $v_type } else { t };
                let clamp = if clamp_upper < 0 as $v_type { 0 as $v_type } else { clamp_upper };
                Quat { _x: start._x * (1 as $v_type - clamp) + end._x * clamp,
                        _y: start._y * (1 as $v_type - clamp) + end._y * clamp,
                        _z: start._z * (1 as $v_type - clamp) + end._z * clamp,
                        _w: start._w * (1 as $v_type - clamp) + end._w * clamp
                }
            }
            #[allow(dead_code)]
            pub fn interpolate_slerp( start: Quat< $v_type >, end: Quat< $v_type >, t: $v_type ) -> Quat< $v_type > {
                let t_clamp_upper = if t > 1 as $v_type { 1 as $v_type } else { t };
                let t_clamp = if t_clamp_upper < 0 as $v_type { 0 as $v_type } else { t_clamp_upper };

                let cos_omega = start._w * end._w + start._x * end._x + start._y * end._y + start._z * end._z;      
                let cos_omega_adjust = if cos_omega < 0 as $v_type {
                    -cos_omega
                }else{
                    cos_omega
                };

                let end_adjust = if cos_omega < 0 as $v_type {
                    //inverted
                    Quat { _x: -end._x, _y: -end._y, _z: -end._z, _w: -end._w }
                } else {
                    Quat { _x: end._x, _y: end._y, _z: end._z, _w: end._w }
                };

                let ( k0, k1 ) =
                    if cos_omega_adjust > 0.9999 as $v_type {
                        ( 1 as $v_type - t_clamp, t_clamp )
                    } else {
                        let sin_omega = (1 as $v_type - cos_omega * cos_omega).sqrt();
                        let omega = sin_omega.atan2( cos_omega );
                        let inv_sin_omega = 1 as $v_type / sin_omega;
                        
                        ( ((1 as $v_type - t_clamp) * omega).sin() * inv_sin_omega,
                            (t_clamp * omega).sin() * inv_sin_omega )
                    };
                Quat { _x: start._x * k0 + end_adjust._x * k1,
                        _y: start._y * k0 + end_adjust._y * k1,
                        _z: start._z * k0 + end_adjust._z * k1,
                        _w: start._w * k0 + end_adjust._w * k1
                }
            }
        }
    }
}

define_quat!( f32 );
define_quat!( f64 );
