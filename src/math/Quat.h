#ifndef QUAT_H
#define QUAT_H

#include "Vec.h"

class Quat{
 public:
  float             _quat[4];
                    Quat();
                    Quat( float x, float y, float z, float w );

  Quat              operator - ( const Quat & q ) const;
  Quat              operator + ( const Quat & q ) const;
  Quat              operator * ( const Quat & q ) const;
  const Quat &      operator *= ( const Quat & q );
  inline float &    operator [] ( int i ){ return _quat[i]; };
  inline float      operator [] ( int i ) const{ return _quat[i]; };

  void              AxisAngleDegree( const float axis[], float angle );
  void              AxisAngleDegreeVector( const Vec & v, float angle );  
  float             Length() const;
  void              Normalize();
  Quat              Pow( float t );
  void              ToMatrix( float mat[] ) const;
};

Quat                Slerp(const Quat & q1, const Quat & q2, float t);


#endif
