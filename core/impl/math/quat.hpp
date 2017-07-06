// adapted from MD5 Loader Demo By A.J. Tavakoli

#ifndef E2_QUAT_HPP
#define E2_QUAT_HPP

#include "vec.hpp"

namespace e2 { namespace math {
	
#define QUAT_X 0
#define QUAT_Y 1
#define QUAT_Z 2
#define QUAT_W 3

class quat{
 public:
  float             _quat[4];
                    quat();
                    quat( float x, float y, float z, float w );
                    quat( float x, float y, float z ); //w computed from x, y, z
		    quat( vec a, float w ); //a = {x,y,z}
		    quat( const quat & q );

  quat &            operator = ( const quat & q );
  quat              operator - ( const quat & q ) const;
  quat &            operator -= ( const quat & q );
  quat              operator + ( const quat & q ) const;
  quat &            operator += ( const quat & q );
  quat              operator * ( const quat & q ) const;
  quat &            operator *= ( const quat & q );
  inline float &    operator [] ( int i ){ return _quat[i]; };
  inline float      operator [] ( int i ) const{ return _quat[i]; };

  void              axis_angle_degree( const float axis[], float angle );
  void              axis_angle_degree_vector( const vec & v, float angle );  
  void              set_translation( const float a [] );
  float             length() const;
  float             length_squared() const;
  void              normalize_quat_current();
  quat              normalize_quat() const;
  quat              log() const; //log(q) = log ||q|| + v/||v|| * arccos(a/||v||)
  quat              Pow( float t );
  void              ToMatrixRot( float mat[] ) const; //gets rotation 4x4 matrix
  void              ToMatrixTrans( float mat[] ) const; //gets translation 4x4 matrix
  void              ToAxisAngle( vec & axis, float & angle );
  inline quat       conjugate() const { return quat(-_quat[0], -_quat[1], -_quat[2], _quat[3]); }
  quat              Negate() const; //negative version
  void              RotatePoint( float in [], float out[] ) const;
  quat              MultVec( float in [] ) const;
  quat              inverse() const;

  static quat       scale( float s, const quat q); // s*q
  static quat       scale_add( float s, const quat q1, const quat q2 ); //s*q1 + q2
  static quat       interpolate_slerp(const quat & q1, const quat & q2, float t); //spherical linear interpolation
  static quat       interpolate_linear( const quat q1, const quat q2, float r ); // (1-r) * q1 + r * q2              
};

} }

#endif
