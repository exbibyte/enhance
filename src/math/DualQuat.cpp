#include "DualQuat.h"
#include "Vec.h"

#include <iostream>
#include <math.h>

using namespace std;

DualQuat::DualQuat(){
  SetIdentity();
}

DualQuat::DualQuat( Quat & a, Quat & b ){
  SetQuats( a, b );
}

DualQuat::DualQuat( Vec A, float a, Vec B, float b ){
  SetQuatsVecs( A, a, B, b );
}

DualQuat( DualQuat & a ){
  SetQuats( a._A, a._B );
}

DualQuat::SetIdentity(){
  _A._quat[0] = 0;
  _A._quat[1] = 0;
  _A._quat[2] = 0; 
  _A._quat[3] = 1;

  _B._quat[0] = 0;
  _B._quat[1] = 0;
  _B._quat[2] = 0; 
  _B._quat[3] = 0;
}

DualQuat::SetZero(){
  _A._quat[0] = 0;
  _A._quat[1] = 0;
  _A._quat[2] = 0; 
  _A._quat[3] = 0;

  _B._quat[0] = 0;
  _B._quat[1] = 0;
  _B._quat[2] = 0; 
  _B._quat[3] = 0;
}

DualQuat::SetQuats( Quat & a, Quat & b ){
  _A = a;
  _B = b;
}

DualQuat::SetQuatsVecs( Vec A, float a, Vec B, float b ){
  if(A._dim < 3 || B._dim < 3){
    SetIdentity();
    cout<<"vector dimension less than 3"<<endl;
  }else{
    _A._quat[0] = A._vec[0];
    _A._quat[1] = A._vec[1];
    _A._quat[2] = A._vec[2]; 
    _A._quat[3] = a;
    
    _B._quat[0] = B._vec[0];
    _B._quat[1] = B._vec[1];
    _B._quat[2] = B._vec[2]; 
    _B._quat[3] = b;
  }
}

void DualQuat::SetArray( float [] a ){
  _A = Quat( a[0], a[1], a[2], a[3] ); 
  _B = Quat( a[4], a[5], a[6], a[7] );
}

void DualQuat::GetArray( float [] a ){
  for( int i = 0; i < 4; i++){
    a[i] = _A._quat[i];
  }
  for( int i = 0; i < 4; i++){
    a[i+4] = _B._quat[i];
  }
}

DualQuat & DualQuat::operator = ( const DualQuat & q )
{
  _A = q._A;
  _B = q._B;
  return *this;
}

DualQuat DualQuat::operator + ( const DualQuat & q ) const{
  DualQuat d;

  //add component-wise
  for( int i = 0; i < 4; i++ ){
    d._A._quat[i] = _A._quat[i] + q._A._quat[i];
    d._B._quat[i] = _B._quat[i] + q._B._quat[i];
  }

  return d;
}

DualQuat & DualQuat::operator += ( const DualQuat & q ){

  //add component-wise
  for( int i = 0; i < 4; i++ ){
    _A._quat[i] = _A._quat[i] + q._A._quat[i];
    _B._quat[i] = _B._quat[i] + q._B._quat[i];
  }

  return *this;
}

DualQuat DualQuat::operator - ( const DualQuat & q ) const{
  DualQuat d;

  //add component-wise
  for( int i = 0; i < 4; i++ ){
    d._A._quat[i] = _A._quat[i] - q._A._quat[i];
    d._B._quat[i] = _B._quat[i] - q._B._quat[i];
  }

  return d;
}

DualQuat & DualQuat::operator -= ( const DualQuat & q ){

  //add component-wise
  for( int i = 0; i < 4; i++ ){
    _A._quat[i] = _A._quat[i] - q._A._quat[i];
    _B._quat[i] = _B._quat[i] - q._B._quat[i];
  }

  return *this;
}

DualQuat DualQuat::operator * ( const DualQuat & q ) const{

  Quat a;
  Quat b;

  a = _A * q._A;
  b = ( _A * q._B ) + ( _B * q._A );

  DualQuat d(a, b);

  return d;
}

Quat DualQuat::GetReal() const{
  return Quat( _A._quat[0], _A._quat[1], _A._quat[2], _A._quat[3] );
}

Quat DualQuat::GetDual() const{
  return Quat( _B._quat[0], _B._quat[1], _B._quat[2], _B._quat[3] );
}

Quat DualQuat::SetReal( const Quat & q ){
  _A = q;
}

Quat DualQuat::SetDual( const Quat & q ){
  _B = q;
}

float DualQuat::GetVal( int index ) const{
  if( index < 0 || index > 7 ){
    return NAN;
  }else{
    if( index < 4 ){
      return _A._quat[ index ];
    }else{
      return _B._quat[ index - 4 ];
    }
  }
}

DualScalar NormSquared() const {
  float a = 0;
  float b = 0;
  for( int i = 0; i < 4; i++ ){
    a += _A._quat[i] * _A._quat[i];
    b += _A._quat[i] * _B._quat[i];
  }
  b *= 2;
  return DualScalar( a, b );
}

DualScalar Norm() const {
  DualScalar s = NormSquared();
  s.Sqrt();
  return s;
}

DualQuat Normalize() const {
  DualScalar n = Norm();
  n = n.Invert();
  
}

/**
 * Screw Linear Interpolation ScLERP between two dual quaternions t represent
 * the normalized distance between the two (t=0.5 is half-way), q =
 * q1*(q1^-1q2)^t. For the shortest distance, q1 and q2 should have the same
 * orientation.
 * 
 */
// void DualQuat::ScLerp( DualQuat & q1, DualQuat & q2, float t) {
  
//   // ScLERP = q1(q1^-1 q2)^t
//   conjugate(q1);
  
//   // correct for shortest distance
//   *this = *this * q2;
//   // double d = q1.a * q2.a + q1.A.dot(q2.A);
//   // if (d < 0) {
//   // scale(-1);
//   // }
//   *this = this->pow(t);
//   q1 = q1 * *this;  
// }


// /**
//  * Raises the supplied quaternion to the power e according to euler's formula
//  * Note: only applies to unit quaternions
//  */
DualQuat DualQuat::PowFloat(double e) const {

  DualQuat d = *this;

  double normA = getScrewParameters(ltmp, mtmp, tmp, d);

  // pure translation
  if (normA < 1e-15) {
    d._B.scale(e);
    d.normalize();
    return d;
  }

  // exponentiate
  double theta = tmp[0] * e;
  double alpha = tmp[1] * e;

  // convert back
  d.setScrewParameters(ltmp, mtmp, theta, alpha);

  return d;
}

float DualQuat::GetScrewParameters(Vec & screwaxis, Vec & moment, Vec & angles ) {

  angles.SetDim(2);
  moment.setDim(3);

  //get quat A.x, A.y, A.z
  vec q_A;
  q_A.SetDim(3);
  for( int i = 0; i < 3; i++ ){
    q_A[i] = _A._quat[i];
  }

  //get quat B.x, B.y, B.z
  vec q_B;
  q_B.SetDim(3);
  for( int i = 0; i < 3; i++ ){
    q_B[i] = _B._quat[i];
  } 

  float normA = q_A.Magnitude();

  // pure translation
  if (normA < 1e-15) {
 
    screwaxis = q_B.Normalize();

    for( int i = 0; i < 3; i++ ){
      moment[i] = 0;
    }    

    angles[0] = 0;
    angles[1] = 2 * q_B.Magnitude();
    return normA;
  } else {

    screwaxis = q_A.Normalize();
    angles[0] = 2 * Math.atan2( normA, _A._quat[3] );
    //      if (angles[0] > Math.PI / 2) {
    //         angles[0] -= Math.PI;
    //      }
    angles[1] = -2 * _B._quat[3] / normA;
    Vec m1 = ScaleVec( 1.0 / normA, q_B );
    Vec m2 = ScaleVec(  _A._quat[3] * _B._quat[3] / (normA * normA), screwaxis );
    moment = m1 + m2;

    return normA;
  }
}

DualQuat ScaleAddDualScalar( const DualScalar & s, const DualQuat & q1, const DualQuat & q2 ) {
  float a = s.GetReal();
  float b = s.GetDual();

  Quat A = ScaleAdd( a, q1._A, q2._A );
  Quat B = ScaleAdd( a, q1._B, q2._B );

  for( int i = 0; i < 4; i++ ){
    B._quat[i] += b * q1._A._quat[i];
  }

  DualQuat d( A, B );
  return d;
}

DualQuat ScaleAddFloat( float s, const DualQuat & q1, const DualQuat & q2 ) {
  Quat A = ScaleAdd( s, q1._A, q2._A );
  Quat B = ScaleAdd( s, q1._B, q2._B );
  DualQuat d( A, B );
  return d;
}

DualQuat ScaleDualScalar(const DualScalar & s, const DualQuat & q ) {
  float a = s.GetReal();
  float b = s.GetDual();
  
  Quat A = Scale( a, q._A );
  Quat B = Scale( a, q._B );
  Quat B2 = Scale( b, q._A );
  B = B + B2;
  DualQuat d( A, B );
  return d;
}

DualQuat ScaleFloat( float s, const DualQuat & q ) {
  Quat A = Scale( s, q._A );
  Quat B = Scale( s, q._B );
  DualQuat d( A, B );
  return d;
}

