#include <math.h>
#include <stdexcept>
#include <iostream>

using namespace std;

#include "Quat.h"
#include "Vec.h"

Quat::Quat(){
  _quat[0] = 0.0f;
  _quat[1] = 0.0f;
  _quat[2] = 0.0f;
  _quat[3] = 1.0f;
}

Quat::Quat( float x, float y, float z, float w ){
  _quat[0] = x;
  _quat[1] = y;
  _quat[2] = z;
  _quat[3] = w;
}

Quat::Quat( Vec v, float w ){
  _quat[0] = v._vec[0];
  _quat[1] = v._vec[1];
  _quat[2] = v._vec[2];
  _quat[3] = w;
}

Quat::Quat( const Quat & q ){
  _quat[0] = q[0];
  _quat[1] = q[1];
  _quat[2] = q[2];
  _quat[3] = q[3];
}

Quat & Quat::operator = ( const Quat & q ) {
  for( int i = 0; i < 4; i++ ){
    _quat[i] = q._quat[i];
  }
  return *this;
}

Quat Quat::operator - ( const Quat & q ) const{
  Quat r = *this;

  for( int i = 0; i < 4; i++ ){
    r._quat[i] -= q._quat[i];
  }
  
  return r;
}

Quat & Quat::operator -= ( const Quat & q ) {

  for( int i = 0; i < 4; i++ ){
    _quat[i] -= q._quat[i];
  }
  
  return *this;
}

Quat Quat::operator + ( const Quat & q ) const{
  Quat r = *this;

  for( int i = 0; i < 4; i++ ){
    r._quat[i] += q._quat[i];
  }
  
  return r;
}

Quat & Quat::operator += ( const Quat & q ) {

  for( int i = 0; i < 4; i++ ){
    _quat[i] += q._quat[i];
  }
  
  return *this;
}

Quat Quat::operator * ( const Quat & q ) const{
  Quat r;
  
  r._quat[3] = _quat[3] * q._quat[3] - _quat[0] * q._quat[0] - _quat[1] * q._quat[1] - _quat[2] * q._quat[2];
  r._quat[0] = _quat[3] * q._quat[0] + _quat[0] * q._quat[3] + _quat[1] * q._quat[2] - _quat[2] * q._quat[1];
  r._quat[1] = _quat[3] * q._quat[1] - _quat[0] * q._quat[2] + _quat[1] * q._quat[3] + _quat[2] * q._quat[0];
  r._quat[2] = _quat[3] * q._quat[2] + _quat[0] * q._quat[1] - _quat[1] * q._quat[0] + _quat[2] * q._quat[3];

  return r;
}

Quat & Quat::operator *= ( const Quat & q ) {
  
  float w = _quat[3] * q._quat[3] - _quat[0] * q._quat[0] - _quat[1] * q._quat[1] - _quat[2] * q._quat[2];
  float x = _quat[3] * q._quat[0] + _quat[0] * q._quat[3] + _quat[1] * q._quat[2] - _quat[2] * q._quat[1];
  float y = _quat[3] * q._quat[1] - _quat[0] * q._quat[2] + _quat[1] * q._quat[3] + _quat[2] * q._quat[0];
  float z = _quat[3] * q._quat[2] + _quat[0] * q._quat[1] - _quat[1] * q._quat[0] + _quat[2] * q._quat[3];
  
  _quat[0] = x;
  _quat[1] = y;
  _quat[2] = z;
  _quat[3] = w;

  return *this;
}

void Quat::AxisAngleDegree( const float axis[], float angle ){
  
  Vec v;
  v._vec[0] = axis[0];
  v._vec[1] = axis[1];
  v._vec[2] = axis[2];

  v.NormalizeCurrent();

  double radians = (angle/180.0f)*3.14159265f;
  
  double sinehalf = sin( double(radians/2.0f) );
  
  _quat[0] = v._vec[0] * sinehalf;
  _quat[1] = v._vec[1] * sinehalf;
  _quat[2] = v._vec[2] * sinehalf;
  
  _quat[3] = (float)cos( double(radians/2.0f) );
}

void Quat::AxisAngleDegreeVector( const Vec & v, float angle ){
  if(v._dim < 3)
    throw("Quat::AxisAngleVector(): vector dimension < 3");

  float axis[3];
  axis[0] = v._vec[0];
  axis[1] = v._vec[1];
  axis[2] = v._vec[2];
  
  this->AxisAngleDegree( axis, angle );
}

void Quat::SetTranslation( const float a [] ){
  _quat[0] = a[0]/2; //dx
  _quat[1] = a[1]/2; //dy
  _quat[2] = a[2]/2; //dz
  _quat[3] = 0;
}

float Quat::Length() const{
  return (float)sqrt( LengthSquared() );
}

float Quat::LengthSquared() const{
  return (float) double(_quat[0]*_quat[0] + _quat[1]*_quat[1] + _quat[2]*_quat[2] + _quat[3]*_quat[3]);
}

void Quat::NormalizeQuatCurrent(){
   float len = Length();

   if( len == 0 )
     throw("Quat::Normalize(): can't divide by length 0");

   _quat[0] /= len;
   _quat[1] /= len;
   _quat[2] /= len;
   _quat[3] /= len;
}

Quat Quat::NormalizeQuat() const{
  Quat q = *this;
  q.NormalizeQuatCurrent();
  return q;
}

Quat Quat::Log() const{
  Quat q;
  //set a of quat
  float q_len = this->Length();
  q._quat[3] = log( q_len );

  //copy x,y,z of quat into vec
  Vec v;
  v.SetDim(3);
  for( int i = 0; i < 3; i++ ){
    v[i] = _quat[i];
  }

  //get v/||v||
  float v_len = v.Magnitude();
  v.NormalizeCurrent();

  //get arccos(a/||v||)
  float m = acos( _quat[3] / q_len );
  v = ScaleVec( m, v );

  //set x,y,z of quat
  for( int i = 0; i < 3; i++ ){
    q._quat[i] = v[i];
  }

  return q;
}

Quat Quat::Pow( float t ){
  Quat result = (*this); //copy

  //i,j,k components
  Vec v;
  v._vec[0] = _quat[0];
  v._vec[1] = _quat[1];
  v._vec[2] = _quat[2];
  v.NormalizeCurrent();
  
  float len = this->Length();

  double alpha = acos( double(_quat[3]) / len ); // original angle
  double beta = t * alpha; // new angle
  
  double coeff = pow(len, t);
  result._quat[3] = coeff * cos( beta );

  for( int i = 0; i < 3; i++ ){
    result._quat[i] = coeff * v._vec[i] * sin( beta );
  }

  // if ( fabs(double(_quat[3])) < 0.9999 ) {
  //   float alpha = (float)acos(double(_quat[3]));
  //   float newAlpha = alpha*t;

  //   result._quat[3] = (float)cos( double(newAlpha) );
  //   float fact = float( sin(double(newAlpha))/sin(double(alpha)) );
  //   result._quat[0] *= fact;
  //   result._quat[1] *= fact;
  //   result._quat[2] *= fact;
  // }

  return result;
}

void Quat::ToMatrixRot( float mat[] ) const{

  // column 1
  mat[ 0] = 1.0f - 2.0f * ( _quat[1] * _quat[1] + _quat[2] * _quat[2] ); 
  mat[ 1] = 2.0f * (_quat[0] * _quat[1] + _quat[2] * _quat[3]);
  mat[ 2] = 2.0f * (_quat[0] * _quat[2] - _quat[1] * _quat[3]);
  mat[ 3] = 0.0f;  
  
  // column 2
  mat[ 4] = 2.0f * ( _quat[0] * _quat[1] - _quat[2] * _quat[3] );  
  mat[ 5] = 1.0f - 2.0f * ( _quat[0] * _quat[0] + _quat[2] * _quat[2] ); 
  mat[ 6] = 2.0f * (_quat[2] * _quat[1] + _quat[0] * _quat[3] );  
  mat[ 7] = 0.0f;  
  
  // column 3
  mat[ 8] = 2.0f * ( _quat[0] * _quat[2] + _quat[1] * _quat[3] );
  mat[ 9] = 2.0f * ( _quat[1] * _quat[2] - _quat[0] * _quat[3] );
  mat[10] = 1.0f - 2.0f * ( _quat[0] * _quat[0] + _quat[1] * _quat[1] );  
  mat[11] = 0.0f;  
  
  // column 4
  mat[12] = 0;  
  mat[13] = 0;  
  mat[14] = 0;  
  mat[15] = 1.0f;
}

void Quat::ToMatrixTrans( float mat[] ) const{
  for( int i = 0; i < 12; i++ ){
    mat[i] = 0;
  }
  mat[12] = 2 * _quat[0];
  mat[13] = 2 * _quat[1]; 
  mat[14] = 2 * _quat[2];
  mat[15] = 1;
}

Quat Quat::Negate() const {
  return Quat( -_quat[0], -_quat[1], -_quat[2], -_quat[3] );
}

Quat InterpolateBasic( const Quat q1, const Quat q2, float r ){
  Quat q;

  for( int i = 0 ; i < 4; i++) {
    q._quat[i] = (1-r) * q1._quat[i] + r * q2._quat[i];
  }

  return q;
}
 
Quat Scale( float s, const Quat q ){
  return Quat( s * q._quat[0], s * q._quat[1], s * q._quat[2], s * q._quat[3] );
}

Quat ScaleAdd( float s, const Quat q1, const Quat q2 ){
  return Quat( s * q1._quat[0] + q2._quat[0], s * q1._quat[1] + q2._quat[1], s * q1._quat[2] + q2._quat[2], s * q1._quat[3] + q2._quat[3] );
}

Quat InterpolateSlerp( const Quat & q1, const Quat & q2, float t ){
  Quat result, p2 = q2;

  //q2 = q1 * result
  //result = conj(q1)*q2
  //omega = half angle between the 2 quaternions 
  float cosOmega = q1._quat[3]*q2._quat[3] + q1._quat[0]*q2._quat[0] + q1._quat[1]*q2._quat[1] + q1._quat[2]*q2._quat[2];

  //inverted case
  if ( cosOmega < 0.0f ) {
    p2._quat[0] = -p2._quat[0];
    p2._quat[1] = -p2._quat[1];
    p2._quat[2] = -p2._quat[2];
    p2._quat[3] = -p2._quat[3];
    cosOmega = -cosOmega;
  }

  float k0, k1;
  if ( cosOmega > 0.99999f ) {
    k0 = 1.0f - t;
    k1 = t;
  }
  else {
    float sinOmega = (float)sqrt( double(1.0f - cosOmega*cosOmega) );
    float omega = (float)atan2( double(sinOmega), double(cosOmega) );

    float invSinOmega = 1.0f/sinOmega;

    k0 = float( sin(double((1.0f - t)*omega)) )*invSinOmega;
    k1 = float( sin(double(t*omega)) )*invSinOmega;
  }

  for ( int i=0; i < 4; i++ )
    result[i] = q1[i]*k0 + p2[i]*k1;

  return result;
}

