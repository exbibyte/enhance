// adapted from MD5 Loader Demo By A.J. Tavakoli

#ifndef QUAT_H
#define QUAT_H

#include "Vec.h"

#define QUAT_X 0
#define QUAT_Y 1
#define QUAT_Z 2
#define QUAT_W 3

class Quat{
 public:
  float             _quat[4];
                    Quat();
                    Quat( float x, float y, float z, float w );
		    Quat( Vec a, float w ); //a = {x,y,z}
		    Quat( const Quat & q );

  Quat &            operator = ( const Quat & q );
  Quat              operator - ( const Quat & q ) const;
  Quat &            operator -= ( const Quat & q );
  Quat              operator + ( const Quat & q ) const;
  Quat &            operator += ( const Quat & q );
  Quat              operator * ( const Quat & q ) const;
  Quat &            operator *= ( const Quat & q );
  inline float &    operator [] ( int i ){ return _quat[i]; };
  inline float      operator [] ( int i ) const{ return _quat[i]; };

  void              AxisAngleDegree( const float axis[], float angle );
  void              AxisAngleDegreeVector( const Vec & v, float angle );  
  float             Length() const;
  void              Normalize();
  Quat              Pow( float t );
  void              ToMatrix( float mat[] ) const;
  inline Quat       Conjugate() const { return Quat(-_quat[0], -_quat[1], -_quat[2], _quat[3]); }
  Quat              Negate() const; //negative version
};

Quat                Scale( float s, const Quat q); // s*q
Quat                ScaleAdd( float s, const Quat q1, const Quat q2 ); //s*q1 + q2
Quat                Slerp(const Quat & q1, const Quat & q2, float t); //spherical linear interpolation
Quat                Interpolate( const Quat q1, const Quat q2, float r ); // (1-r) * q1 + r * q2              


#endif
