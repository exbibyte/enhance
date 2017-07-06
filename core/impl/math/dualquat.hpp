///@author Bill Liu
///dual quaternion port from Java (Antonio Sanchez)

#ifndef E2_DUALQUAT_HPP
#define E2_DUALQUAT_HPP

#include <stdexcept>

#include "quat.hpp"
#include "vec.hpp"
#include "dualscalar.hpp"

namespace e2 { namespace math {

class dualquat{
 public:

  //store quaternions: a (real = rotation) + e*b (dual = translation)
  quat              _a;		
  quat              _b;

                    dualquat(); 
                    dualquat(quat & a, quat & b);
                    dualquat(vec A, float a, vec B, float b);
                    dualquat(dualquat & a);

  void     	    set_identity(); // identity dual quaternion 1 + e0
  void              set_zero(); // dual quaternion 0 + e0
  void   	    set_quats(quat & a, quat & b); // a + eb
  void  	    set_quats_vecs(vec A, float a, vec B, float b); //A+a + e(B+b)
  void              set_array(float a []);
  void              get_array(float a []) const;

  dualquat &        operator = ( const dualquat & q ); //copy
  dualquat          operator + ( const dualquat & q ) const;
  dualquat &        operator += ( const dualquat & q );
  dualquat          operator - ( const dualquat & q ) const;
  dualquat &        operator -= ( const dualquat & q );
  dualquat          operator * ( const dualquat & q ) const; // non-transitive multiplication q1 * q2 = (q1._A*q2._A, q1._A*q2._B + q1._B*q2._A)
  dualquat inline   conjugate() const{ quat a (_a.conjugate()); quat b (_b.conjugate()); dualquat q ( a, b); return q; } // calculates q^-1
  void inline       set_conjugate(){ _a = _a.conjugate(); _b = _b.conjugate(); } //sets conjugate for current dual quaternion

  quat              get_real() const;
  quat              get_dual() const;
  quat &            get_real_ref();
  quat &            get_dual_ref();
  void              set_real( const quat & q );
  void              set_dual( const quat & q );
  float             get_val( int index ) const;

  dualscalar        magnitude_squared() const; // ||q||^2 =  q * q^-1
  dualscalar        magnitude() const; // ||q|| = sqrt(q * q^-1)

  dualquat          normalize() const; // normalize to ||q|| = 1+ e*0
  void              normalize_current(); // normalize to ||q|| = 1+ e*0

  dualquat          invert() const; //gets inverse where q^-1 * q = q * q^-1 = 1 + e0

  dualquat          pow_float(double e) const; ///Raises the supplied quaternion to the power e according to euler's formula, this applies to unit quaternions. q = [ cos(theta/2), sin(theta/2)Screwaxis ] + eps [ -alpha/2*sin(theta/2), sin(theta/2)Moment + alpha/2*cos(theta/2)Screwaxis ]

  float             get_screw_parameters( vec & screwaxix, vec & moment, vec & angles ); // returns norm of _a._quat's x,y,z 
  void              set_screw_parameters( vec & screwaxis, vec & moment, float theta, float alpha );

  void              get_rigid_transform( float a [] ) const; //convert to rigid transform in a column major array representing 4x4 mat
  void              set_translation( const float a [] ); //set translation quaternion by provided array (x,y,z)
  void              axis_angle_degree( const float axis[], float angle ); // sets rotation quaternion by provided axis and angle
  void              axis_angle_degree_vector( const vec & v, float angle );  

  class Exception : public std::runtime_error {
  public:
    Exception(const std::string &msg): std::runtime_error(msg) { }
  };
};

dualquat          scale_add_dualscalar( const dualscalar & s, const dualquat & q1, const dualquat & q2 ); // returns ( s.a*q1.a + q2.a, s.a*q1.b + q2.b + s.b * q1.a )
dualquat          scale_add_float( float s, const dualquat & q1, const dualquat & q2 ); // returns s*q1 + q2
dualquat          scale_dual_scalar( const dualscalar & s, const dualquat & q ); //( s.a*q1.a, s.a*q.b + s.b * q.a )
dualquat          scale_float( float s, const dualquat & q ); //s*q
dualquat          interpolate_sclerp( const dualquat & q1, const dualquat & q2, float t ); ///Screw Linear Interpolate. q = q1*(q1^-1 q2)^t. For the shortest distance, q1 and q2 should have the same orientation.

} }

#endif
