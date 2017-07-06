#include "dualquat.hpp"
#include "vec.hpp"

#include <iostream>
#include <math.h>

using namespace std;
using namespace e2::math;

dualquat::dualquat(){
  set_identity();
}

dualquat::dualquat( quat & a, quat & b ){
  set_quats( a, b );
}

dualquat::dualquat( vec va, float a, vec vb, float b ){
  set_quats_vecs( va, a, vb, b );
}

dualquat::dualquat( dualquat & a ){
  set_quats( a._a, a._b );
}

void dualquat::set_identity(){
  _a._quat[0] = 0;
  _a._quat[1] = 0;
  _a._quat[2] = 0; 
  _a._quat[3] = 1;

  _b._quat[0] = 0;
  _b._quat[1] = 0;
  _b._quat[2] = 0; 
  _b._quat[3] = 0;
}

void dualquat::set_zero(){
  _a._quat[0] = 0;
  _a._quat[1] = 0;
  _a._quat[2] = 0; 
  _a._quat[3] = 0;

  _b._quat[0] = 0;
  _b._quat[1] = 0;
  _b._quat[2] = 0; 
  _b._quat[3] = 0;
}

void dualquat::set_quats( quat & a, quat & b ){
  _a = a;
  _b = b;
}

void dualquat::set_quats_vecs( vec va, float a, vec vb, float b ){
  if(va._dim < 3 || vb._dim < 3){
    set_identity();
    cout<<"vector dimension less than 3"<<endl;
  }else{
    _a._quat[0] = va._vec[0];
    _a._quat[1] = va._vec[1];
    _a._quat[2] = va._vec[2]; 
    _a._quat[3] = a;
    
    _b._quat[0] = vb._vec[0];
    _b._quat[1] = vb._vec[1];
    _b._quat[2] = vb._vec[2]; 
    _b._quat[3] = b;
  }
}

void dualquat::set_array( float a [] ){
  _a = quat( a[0], a[1], a[2], a[3] ); 
  _b = quat( a[4], a[5], a[6], a[7] );
}

void dualquat::get_array( float a [] ) const{
  for( int i = 0; i < 4; i++){
    a[i] = _a._quat[i];
  }
  for( int i = 0; i < 4; i++){
    a[i+4] = _b._quat[i];
  }
}

dualquat & dualquat::operator = ( const dualquat & q )
{
  _a = q._a;
  _b = q._b;
  return *this;
}

dualquat dualquat::operator + ( const dualquat & q ) const{
  dualquat d;

  //add component-wise
  for( int i = 0; i < 4; i++ ){
    d._a._quat[i] = _a._quat[i] + q._a._quat[i];
    d._b._quat[i] = _b._quat[i] + q._b._quat[i];
  }

  return d;
}

dualquat & dualquat::operator += ( const dualquat & q ){

  //add component-wise
  for( int i = 0; i < 4; i++ ){
    _a._quat[i] = _a._quat[i] + q._a._quat[i];
    _b._quat[i] = _b._quat[i] + q._b._quat[i];
  }

  return *this;
}

dualquat dualquat::operator - ( const dualquat & q ) const{
  dualquat d;

  //add component-wise
  for( int i = 0; i < 4; i++ ){
    d._a._quat[i] = _a._quat[i] - q._a._quat[i];
    d._b._quat[i] = _b._quat[i] - q._b._quat[i];
  }

  return d;
}

dualquat & dualquat::operator -= ( const dualquat & q ){

  //add component-wise
  for( int i = 0; i < 4; i++ ){
    _a._quat[i] = _a._quat[i] - q._a._quat[i];
    _b._quat[i] = _b._quat[i] - q._b._quat[i];
  }

  return *this;
}

dualquat dualquat::operator * ( const dualquat & q ) const{

  quat a;
  quat b;

  a = _a * q._a;
  b = ( _a * q._b ) + ( _b * q._a );

  dualquat d(a, b);

  return d;
}

quat dualquat::get_real() const{
  return quat( _a._quat[0], _a._quat[1], _a._quat[2], _a._quat[3] );
}

quat dualquat::get_dual() const{
  return quat( _b._quat[0], _b._quat[1], _b._quat[2], _b._quat[3] );
}
quat & dualquat::get_real_ref(){
    return _a;
}
quat & dualquat::get_dual_ref(){
    return _b;
}
void dualquat::set_real( const quat & q ){
  _a = q;
}

void dualquat::set_dual( const quat & q ){
  _b = q;
}

float dualquat::get_val( int index ) const{
  if( index < 0 || index > 7 ){
    return NAN;
  }else{
    if( index < 4 ){
      return _a._quat[ index ];
    }else{
      return _b._quat[ index - 4 ];
    }
  }
}

dualscalar dualquat::magnitude_squared() const {
  float a = 0;
  float b = 0;

  dualquat q;
  q = *this;
  q = q.conjugate();
  
  dualquat p;
  p = (*this) * q;
  
  a = p._a.length_squared();
  b = p._b.length_squared();

  dualscalar d( a, b );
  return d;
}

dualscalar dualquat::magnitude() const {
  dualscalar s;
  s = magnitude_squared();
  s._a = sqrt(s._a);
  s._b = sqrt(s._b);
  return s;
}

dualquat dualquat::normalize() const {
  dualquat q;
  q.normalize_current();
  // dualscalar n;
  // n = magnitude();
  // n = n.invert();
  // q = scale_dual_scalar( n, q );
  return q;
}

void dualquat::normalize_current() {
  dualscalar n;
  n = magnitude();
  n = n.invert();
  (*this) = scale_dual_scalar( n, (*this) );
}

dualquat dualquat::invert() const {
  dualscalar s;
  s = magnitude_squared();
  s = s.invert();
  dualquat q;
  q = conjugate();
  q = scale_dual_scalar( s, q );
  return q;
}

dualquat interpolate_sclerp( const dualquat & q1, const dualquat & q2, float t) { 

  dualquat q;

  //q1^-1 * q2
  q = q1.conjugate() * q2;
  // double d = q1.a * q2.a + q1.a.dot(q2.a);
  // if (d < 0) {
  // scale(-1);
  // }
  q = q.pow_float(t);
  q = q1 * q;  

  return q;
}

dualquat dualquat::pow_float(double e) const {

  dualquat d;
  d = *this;

  vec screwaxis;
  vec moment;
  vec angles;

  double norma = d.get_screw_parameters( screwaxis, moment, angles );

  // pure translation
  if ( norma < 1e-15 ) {
    for( int i = 0; i < 3; i++ ){
      d._b._quat[i] *= e;
    }
    d.normalize_current();
    return d;
  }else{
    // exponentiate
    double theta = angles[0] * e;
    double alpha = angles[1] * e;
    // convert back
    d.set_screw_parameters( screwaxis, moment, theta, alpha );
    return d;
  }
}

float dualquat::get_screw_parameters(vec & screwaxis, vec & moment, vec & angles ) {

  angles.set_dim(2);
  moment.set_dim(3);
  screwaxis.set_dim(3);

  //get quat a.x, a.y, a.z
  vec q_a;
  q_a.set_dim(3);
  for( int i = 0; i < 3; i++ ){
    q_a[i] = _a._quat[i];
  }

  //get quat b.x, b.y, b.z
  vec q_b;
  q_b.set_dim(3);
  for( int i = 0; i < 3; i++ ){
    q_b[i] = _b._quat[i];
  } 

  float norma = q_a.magnitude();

  // pure translation
  if (norma < 1e-15) {
    screwaxis = q_b.normalize();
    for( int i = 0; i < 3; i++ ){
      moment[i] = 0;
    }    
    angles[0] = 0;
    angles[1] = 2 * q_b.magnitude();
    return norma;
  } else {
    screwaxis = q_a.normalize();
    angles[0] = 2 * atan2( norma, _a._quat[3] );
    //      if (angles[0] > math.pi / 2) {
    //         angles[0] -= math.pi;
    //      }
    angles[1] = -2 * _b._quat[3] / norma;
    vec m1 = vec::scale_vec( 1.0 / norma, q_b );
    vec m2 = vec::scale_vec(  _a._quat[3] * _b._quat[3] / (norma * norma), screwaxis );
    moment = m1 + m2;
    return norma;
  }
}

void dualquat::set_screw_parameters(vec & screwaxis, vec & moment, float theta, float alpha) {
  float cosa = cos( theta / 2 );
  float sina = sin( theta / 2 );
  
  _a._quat[3] = cosa;
  for( int i = 0; i < 3; i++ ){
    _a._quat[i] = sina * screwaxis[i];
  }
  
  _b._quat[3] = -alpha / 2 * sina;
  for( int i = 0; i < 3; i++ ){
    _b._quat[i] = sina * moment[i] + alpha / 2 * cosa * screwaxis[i];
  }

  normalize_current();
}

void dualquat::get_rigid_transform( float trans [] ) const{
      
  trans[0] = _a._quat[3] * _a._quat[3] + _a._quat[0] * _a._quat[0] - _a._quat[1] * _a._quat[1] - _a._quat[2] * _a._quat[2];
  trans[4] = 2*(_a._quat[0]*_a._quat[1]- _a._quat[3]*_a._quat[2]);
  trans[8] = 2*(_a._quat[0]*_a._quat[2]+ _a._quat[3]*_a._quat[1]);
  trans[1] =  2*(_a._quat[0]*_a._quat[1]+ _a._quat[3]*_a._quat[2]);
  trans[5] =  _a._quat[3]*_a._quat[3] - _a._quat[0]*_a._quat[0] + _a._quat[1]*_a._quat[1] -_a._quat[2]*_a._quat[2];;
  trans[9] = 2*(_a._quat[1]*_a._quat[2]- _a._quat[3]*_a._quat[0]);
  trans[2] = 2*(_a._quat[0]*_a._quat[2]- _a._quat[3]*_a._quat[1]);
  trans[6] = 2*(_a._quat[1]*_a._quat[2]+ _a._quat[3]*_a._quat[0]);
  trans[10] =  _a._quat[3]*_a._quat[3] - _a._quat[0]*_a._quat[0] - _a._quat[1]*_a._quat[1] + _a._quat[2]*_a._quat[2];

  //below is not used
  // trans[12] = 2*(_a._quat[3]*_b._quat[0] - _a._quat[0]*_b._quat[3] + _a._quat[1]*_b._quat[2] - _a._quat[2]*_b._quat[1]);
  // trans[13] = 2*(_a._quat[3]*_b._quat[1] - _a._quat[0]*_b._quat[2] - _a._quat[1]*_b._quat[3] +_a._quat[2]*_b._quat[0]);
  // trans[14] = 2*(_a._quat[3]*_b._quat[2] + _a._quat[0]*_b._quat[1] - _a._quat[1]*_b._quat[0] - _a._quat[2]*_b._quat[3]);

  trans[12] = 2 * _b._quat[0];
  trans[13] = 2 * _b._quat[1];
  trans[14] = 2 * _b._quat[2];

  trans[15] = 1;
  trans[3] = 0;
  trans[7] = 0;
  trans[11] = 0;
}   

void dualquat::set_translation( const float trans [] ){  
  _b.set_translation( trans );
}

// void dualquat::SetTranslation(const float axis [], float angle, const float trans [] ){
  
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

//   _b.SetTranslation( a );
// }

void dualquat::axis_angle_degree( const float axis[], float angle ){
  _a.axis_angle_degree( axis, angle );
}

void dualquat::axis_angle_degree_vector( const vec & v, float angle ){
  _a.axis_angle_degree_vector( v, angle );
}

dualquat scale_add_dualscalar( const dualscalar & s, const dualquat & q1, const dualquat & q2 ) {
  float a = s.get_real();
  float b = s.get_dual();

  quat A = ScaleAdd( a, q1._a, q2._a );
  quat B = ScaleAdd( a, q1._b, q2._b );

  for( int i = 0; i < 4; i++ ){
    B._quat[i] += b * q1._a._quat[i];
  }

  dualquat d( A, B );
  return d;
}

dualquat scale_add_float( float s, const dualquat & q1, const dualquat & q2 ) {
  quat A = ScaleAdd( s, q1._a, q2._a );
  quat B = ScaleAdd( s, q1._b, q2._b );
  dualquat d( A, B );
  return d;
}

dualquat scale_dual_scalar(const dualscalar & s, const dualquat & q ) {
  float a = s.get_real();
  float b = s.get_dual();
  
  quat A = Scale( a, q._a );
  quat B = Scale( a, q._b );
  quat B2 = Scale( b, q._a );
  B = B + B2;
  dualquat d( A, B );
  return d;
}

dualquat scale_float( float s, const dualquat & q ) {
  quat A = Scale( s, q._a );
  quat B = Scale( s, q._b );
  dualquat d( A, B );
  return d;
}

