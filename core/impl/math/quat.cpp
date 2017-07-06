#include <cmath>
#include <stdexcept>
#include <iostream>
#include <cassert>

using namespace std;

#include "quat.hpp"
#include "vec.hpp"

using namespace e2::math;

quat::quat(){
    _quat[0] = 0.0f;
    _quat[1] = 0.0f;
    _quat[2] = 0.0f;
    _quat[3] = 1.0f;
}

quat::quat( float x, float y, float z, float w ){
    _quat[0] = x;
    _quat[1] = y;
    _quat[2] = z;
    _quat[3] = w;
}

quat::quat( float x, float y, float z ){
    _quat[0] = x;
    _quat[1] = y;
    _quat[2] = z;
    float t = 1.0 - x*x - y*y - z*z;
    if( t < 0 )
	_quat[3] = 0;	
    else
	_quat[3] = (float)-sqrt((double)t);
}

quat::quat( vec v, float w ){
    _quat[0] = v._vec[0];
    _quat[1] = v._vec[1];
    _quat[2] = v._vec[2];
    _quat[3] = w;
}

quat::quat( const quat & q ){
    _quat[0] = q[0];
    _quat[1] = q[1];
    _quat[2] = q[2];
    _quat[3] = q[3];
}

quat & quat::operator = ( const quat & q ) {
    for( int i = 0; i < 4; i++ ){
	_quat[i] = q._quat[i];
    }
    return *this;
}

quat quat::operator - ( const quat & q ) const{
    quat r = *this;

    for( int i = 0; i < 4; i++ ){
	r._quat[i] -= q._quat[i];
    }
  
    return r;
}

quat & quat::operator -= ( const quat & q ) {

    for( int i = 0; i < 4; i++ ){
	_quat[i] -= q._quat[i];
    }
  
    return *this;
}

quat quat::operator + ( const quat & q ) const{
    quat r = *this;

    for( int i = 0; i < 4; i++ ){
	r._quat[i] += q._quat[i];
    }
  
    return r;
}

quat & quat::operator += ( const quat & q ) {

    for( int i = 0; i < 4; i++ ){
	_quat[i] += q._quat[i];
    }
  
    return *this;
}

quat quat::operator * ( const quat & q ) const{
    quat r;
  
    r._quat[3] = _quat[3] * q._quat[3] - _quat[0] * q._quat[0] - _quat[1] * q._quat[1] - _quat[2] * q._quat[2];
    r._quat[0] = _quat[3] * q._quat[0] + _quat[0] * q._quat[3] + _quat[1] * q._quat[2] - _quat[2] * q._quat[1];
    r._quat[1] = _quat[3] * q._quat[1] - _quat[0] * q._quat[2] + _quat[1] * q._quat[3] + _quat[2] * q._quat[0];
    r._quat[2] = _quat[3] * q._quat[2] + _quat[0] * q._quat[1] - _quat[1] * q._quat[0] + _quat[2] * q._quat[3];

    return r;
}

quat & quat::operator *= ( const quat & q ) {
  
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

void quat::axis_angle_degree( const float axis[], float angle ){
  
    vec v;
    v._vec[0] = axis[0];
    v._vec[1] = axis[1];
    v._vec[2] = axis[2];

    v.normalize_current();

    double radians = (angle/180.0f)*3.14159265f;
  
    double sinehalf = sin( double(radians/2.0f) );
  
    _quat[0] = v._vec[0] * sinehalf;
    _quat[1] = v._vec[1] * sinehalf;
    _quat[2] = v._vec[2] * sinehalf;
  
    _quat[3] = (float)cos( double(radians/2.0f) );
}

void quat::axis_angle_degree_vector( const vec & v, float angle ){
    if(v._dim < 3)
	throw("quat::axisanglevector(): vector dimension < 3");

    float axis[3];
    axis[0] = v._vec[0];
    axis[1] = v._vec[1];
    axis[2] = v._vec[2];
  
    this->axis_angle_degree( axis, angle );
}

void quat::set_translation( const float a [] ){
    _quat[0] = a[0]/2; //dx
    _quat[1] = a[1]/2; //dy
    _quat[2] = a[2]/2; //dz
    _quat[3] = 0;
}

float quat::length() const{
    return (float)sqrt( length_squared() );
}

float quat::length_squared() const{
    return (float) double(_quat[0]*_quat[0] + _quat[1]*_quat[1] + _quat[2]*_quat[2] + _quat[3]*_quat[3]);
}

void quat::normalize_quat_current(){
    float len = length();

    if( len == 0 )
	throw("quat::normalize(): can't divide by length 0");

    if( len > 0.0f ){
	_quat[0] /= len;
	_quat[1] /= len;
	_quat[2] /= len;
	_quat[3] /= len;
    }
}

quat quat::normalize_quat() const{
    quat q = *this;
    q.normalize_quat_current();
    return q;
}

quat quat::log() const{
    quat q;
    //set a of quat
    float q_len = this->length();
    q._quat[3] = std::log( q_len );

    //copy x,y,z of quat into vec
    vec v;
    v.set_dim(3);
    for( int i = 0; i < 3; i++ ){
	v[i] = _quat[i];
    }

    //get v/||v||
    float v_len = v.magnitude();
    v.normalize_current();

    //get arccos(a/||v||)
    float m = acos( _quat[3] / q_len );
    v = vec::scale_vec( m, v );

    //set x,y,z of quat
    for( int i = 0; i < 3; i++ ){
	q._quat[i] = v[i];
    }

    return q;
}

quat quat::Pow( float t ){
    quat result = (*this); //copy

    //i,j,k components
    vec v;
    v._vec[0] = _quat[0];
    v._vec[1] = _quat[1];
    v._vec[2] = _quat[2];
    v.normalize_current();
  
    float len = this->length();

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

void quat::ToMatrixRot( float mat[] ) const{

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

void quat::ToMatrixTrans( float mat[] ) const{
    for( int i = 0; i < 12; i++ ){
	mat[i] = 0;
    }
    mat[12] = 2 * _quat[0];
    mat[13] = 2 * _quat[1]; 
    mat[14] = 2 * _quat[2];
    mat[15] = 1;
}

void quat::ToAxisAngle( vec & axis, float & angle ){

    double k = sqrt( 1 - _quat[3] * _quat[3] );

    if( k < 0.000001 ){
	axis._vec[0] = 1;
	axis._vec[1] = 0;
	axis._vec[2] = 0;
	angle = 0;
    }else{
	axis._vec[0] = _quat[0] / k;
	axis._vec[1] = _quat[1] / k;
	axis._vec[2] = _quat[2] / k;
	axis.normalize_current();
	angle = 2.0 * acos( _quat[3] );
    }
    return;
}

quat quat::Negate() const {
    return quat( -_quat[0], -_quat[1], -_quat[2], -_quat[3] );
}

void quat::RotatePoint( float in [], float out[] ) const {
    quat temp, final;
    quat conj = this->conjugate();
    temp = this->MultVec( in );
    final = temp * conj;
    out[0] = final._quat[0];
    out[1] = final._quat[1];
    out[2] = final._quat[2];
}
quat quat::MultVec( float in [] ) const {
    quat q;
    q._quat[3] = - (_quat[0]*in[0]) - (_quat[1]*in[1]) - (_quat[2]*in[2]);
    q._quat[0] = (_quat[3]*in[0]) + (_quat[1]*in[2]) + (_quat[2]*in[1]);
    q._quat[1] = (_quat[3]*in[1]) + (_quat[2]*in[0]) + (_quat[0]*in[2]);
    q._quat[2] = (_quat[3]*in[2]) + (_quat[0]*in[1]) + (_quat[1]*in[0]);
    return q;
}

quat quat::inverse() const {
    quat conj = this->conjugate();
    float norm = conj.length_squared();
    if( norm == 0.0f )
	throw("quat::inverse(): can't divide by length 0");

    quat inv = Scale( 1.0/norm, conj );
    return inv;
}

quat InterpolateBasic( const quat q1, const quat q2, float r ){
    quat q;

    for( int i = 0 ; i < 4; i++) {
	q._quat[i] = (1-r) * q1._quat[i] + r * q2._quat[i];
    }

    return q;
}
 
quat Scale( float s, const quat q ){
    return quat( s * q._quat[0], s * q._quat[1], s * q._quat[2], s * q._quat[3] );
}

quat ScaleAdd( float s, const quat q1, const quat q2 ){
    return quat( s * q1._quat[0] + q2._quat[0], s * q1._quat[1] + q2._quat[1], s * q1._quat[2] + q2._quat[2], s * q1._quat[3] + q2._quat[3] );
}

quat InterpolateSlerp( const quat & q1, const quat & q2, float t ){
    quat result, p2 = q2;

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

    assert( t >= 0 && t <=1 );
    
    if( t < 0 ){
	t = 0;
    }
    else if( t > 1 ){
	t = 1;
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

