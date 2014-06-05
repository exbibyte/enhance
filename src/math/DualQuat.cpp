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

DualQuat::DualQuat( DualQuat & a ){
  SetQuats( a._A, a._B );
}

void DualQuat::SetIdentity(){
  _A._quat[0] = 0;
  _A._quat[1] = 0;
  _A._quat[2] = 0; 
  _A._quat[3] = 1;

  _B._quat[0] = 0;
  _B._quat[1] = 0;
  _B._quat[2] = 0; 
  _B._quat[3] = 0;
}

void DualQuat::SetZero(){
  _A._quat[0] = 0;
  _A._quat[1] = 0;
  _A._quat[2] = 0; 
  _A._quat[3] = 0;

  _B._quat[0] = 0;
  _B._quat[1] = 0;
  _B._quat[2] = 0; 
  _B._quat[3] = 0;
}

void DualQuat::SetQuats( Quat & a, Quat & b ){
  _A = a;
  _B = b;
}

void DualQuat::SetQuatsVecs( Vec A, float a, Vec B, float b ){
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

void DualQuat::SetArray( float a [] ){
  _A = Quat( a[0], a[1], a[2], a[3] ); 
  _B = Quat( a[4], a[5], a[6], a[7] );
}

void DualQuat::GetArray( float a [] ) const{
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

void DualQuat::SetReal( const Quat & q ){
  _A = q;
}

void DualQuat::SetDual( const Quat & q ){
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

DualScalar DualQuat::MagnitudeSquared() const {
  float a = 0;
  float b = 0;

  DualQuat q;
  q = *this;
  q = q.Conjugate();
  
  DualQuat p;
  p = (*this) * q;
  
  a = p._A.LengthSquared();
  b = p._B.LengthSquared();

  DualScalar d( a, b );
  return d;
}

DualScalar DualQuat::Magnitude() const {
  DualScalar s;
  s = MagnitudeSquared();
  s._a = sqrt(s._a);
  s._b = sqrt(s._b);
  return s;
}

DualQuat DualQuat::Normalize() const {
  DualQuat q;
  q.NormalizeCurrent();
  // DualScalar n;
  // n = Magnitude();
  // n = n.Invert();
  // q = ScaleDualScalar( n, q );
  return q;
}

void DualQuat::NormalizeCurrent() {
  DualScalar n;
  n = Magnitude();
  n = n.Invert();
  (*this) = ScaleDualScalar( n, (*this) );
}

DualQuat DualQuat::Invert() const {
  DualScalar s;
  s = MagnitudeSquared();
  s = s.Invert();
  DualQuat q;
  q = Conjugate();
  q = ScaleDualScalar( s, q );
  return q;
}

DualQuat InterpolateSclerp( const DualQuat & q1, const DualQuat & q2, float t) { 

  DualQuat q;

  //q1^-1 * q2
  q = q1.Conjugate() * q2;
  // double d = q1.a * q2.a + q1.A.dot(q2.A);
  // if (d < 0) {
  // scale(-1);
  // }
  q = q.PowFloat(t);
  q = q1 * q;  

  return q;
}

DualQuat DualQuat::PowFloat(double e) const {

  DualQuat d;
  d = *this;

  Vec screwaxis;
  Vec moment;
  Vec angles;

  double normA = d.GetScrewParameters( screwaxis, moment, angles );

  // pure translation
  if ( normA < 1e-15 ) {
    for( int i = 0; i < 3; i++ ){
      d._B._quat[i] *= e;
    }
    d.NormalizeCurrent();
    return d;
  }else{
    // exponentiate
    double theta = angles[0] * e;
    double alpha = angles[1] * e;
    // convert back
    d.SetScrewParameters( screwaxis, moment, theta, alpha );
    return d;
  }
}

float DualQuat::GetScrewParameters(Vec & screwaxis, Vec & moment, Vec & angles ) {

  angles.SetDim(2);
  moment.SetDim(3);
  screwaxis.SetDim(3);

  //get quat A.x, A.y, A.z
  Vec q_A;
  q_A.SetDim(3);
  for( int i = 0; i < 3; i++ ){
    q_A[i] = _A._quat[i];
  }

  //get quat B.x, B.y, B.z
  Vec q_B;
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
    angles[0] = 2 * atan2( normA, _A._quat[3] );
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

void DualQuat::SetScrewParameters(Vec & screwaxis, Vec & moment, float theta, float alpha) {
  float cosa = cos( theta / 2 );
  float sina = sin( theta / 2 );
  
  _A._quat[3] = cosa;
  for( int i = 0; i < 3; i++ ){
    _A._quat[i] = sina * screwaxis[i];
  }
  
  _B._quat[3] = -alpha / 2 * sina;
  for( int i = 0; i < 3; i++ ){
    _B._quat[i] = sina * moment[i] + alpha / 2 * cosa * screwaxis[i];
  }

  NormalizeCurrent();
}

void DualQuat::GetRigidTransform( float trans [] ) const{
      
  trans[0] = _A._quat[3] * _A._quat[3] + _A._quat[0] * _A._quat[0] - _A._quat[1] * _A._quat[1] - _A._quat[2] * _A._quat[2];
  trans[4] = 2*(_A._quat[0]*_A._quat[1]- _A._quat[3]*_A._quat[2]);
  trans[8] = 2*(_A._quat[0]*_A._quat[2]+ _A._quat[3]*_A._quat[1]);
  trans[1] =  2*(_A._quat[0]*_A._quat[1]+ _A._quat[3]*_A._quat[2]);
  trans[5] =  _A._quat[3]*_A._quat[3] - _A._quat[0]*_A._quat[0] + _A._quat[1]*_A._quat[1] -_A._quat[2]*_A._quat[2];;
  trans[9] = 2*(_A._quat[1]*_A._quat[2]- _A._quat[3]*_A._quat[0]);
  trans[2] = 2*(_A._quat[0]*_A._quat[2]- _A._quat[3]*_A._quat[1]);
  trans[6] = 2*(_A._quat[1]*_A._quat[2]+ _A._quat[3]*_A._quat[0]);
  trans[10] =  _A._quat[3]*_A._quat[3] - _A._quat[0]*_A._quat[0] - _A._quat[1]*_A._quat[1] + _A._quat[2]*_A._quat[2];
      
  // trans[12] = 2*(_A._quat[3]*_B._quat[0] - _A._quat[0]*_B._quat[3] + _A._quat[1]*_B._quat[2] - _A._quat[2]*_B._quat[1]);
  // trans[13] = 2*(_A._quat[3]*_B._quat[1] - _A._quat[0]*_B._quat[2] - _A._quat[1]*_B._quat[3] +_A._quat[2]*_B._quat[0]);
  // trans[14] = 2*(_A._quat[3]*_B._quat[2] + _A._quat[0]*_B._quat[1] - _A._quat[1]*_B._quat[0] - _A._quat[2]*_B._quat[3]);

  trans[12] = 2 * _B._quat[0];
  trans[13] = 2 * _B._quat[1];
  trans[14] = 2 * _A._quat[2];

  trans[15] = 1;
  trans[3] = 0;
  trans[7] = 0;
  trans[11] = 0;
}   

void DualQuat::SetTranslation( const float trans [] ){  
  _B.SetTranslation( trans );
}

// void DualQuat::SetTranslation(const float axis [], float angle, const float trans [] ){
  
//   AxisAngleDegree( axis, angle );
//   Mat m_rot[16];
//   ToMatrixRot( m_rot );
  
//   //get diplacement
  
//   int dim_trans[2];
//   dim_trans[0] = 4;
//   dim_trans[1] = 1;
//   Mat m_trans;
//   m_trans.ResizeInt( 2, dim_trans );
//   m_trans( 0, 0 ) = trans[0];
//   m_trans( 1, 0 ) = trans[1];
//   m_trans( 2, 0 ) = trans[2];
//   m_trans( 3, 0 ) = 1;
  
//   Mat m_displace;
//   m_displace = m_rot * m_trans;
//   m_displace.TransposeCurrent();

//   Mat m_trans_dif;
//   m_trans_dif = m_trans - m_displace;

//   _B.SetTranslation( a );
// }

void DualQuat::AxisAngleDegree( const float axis[], float angle ){
  _A.AxisAngleDegree( axis, angle );
}

void DualQuat::AxisAngleDegreeVector( const Vec & v, float angle ){
  _A.AxisAngleDegreeVector( v, angle );
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

