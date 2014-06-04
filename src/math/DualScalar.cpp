#include "DualScalar.h"
#include <math.h>

DualScalar::DualScalar(){
  _a = 0;
  _b = 0;
}

DualScalar::DualScalar( float a, float b ){
  _a = a;
  _b = b;
}

DualScalar::DualScalar( DualScalar & d ){
  _a = d._a;
  _b = d._b;
}

void DualScalar::SetDualNumber( float a, float b ){
  _a = a;
  _b = b;
}

void DualScalar::SetDualScalar( const DualScalar & d ){
  _a = d._a;
  _b = d._b;
}

void DualScalar::SetArray( float v [] ){
  _a = v[0];
  _b = v[1];
}

void DualScalar::GetArray( float v [] ) const {
  v[0] = _a;
  v[1] = _b;
}

DualScalar & DualScalar::operator = (const DualScalar & d ){
  _a = d._a;
  _b = d._b;

  return (*this);
}

DualScalar DualScalar::operator + ( const DualScalar & d ) const {
  DualScalar s;
  s._a = _a + d._a;
  s._b = _b + d._b;
  return s;
}

DualScalar & DualScalar::operator += (const DualScalar & d ){
  _a += d._a;
  _b += d._b;
  return (*this);
}

DualScalar DualScalar::operator - ( const DualScalar & d ) const {
  DualScalar s;
  s._a = _a - d._a;
  s._b = _b - d._b;
  return s;
}

DualScalar & DualScalar::operator -= ( const DualScalar & d ){
  _a -= d._a;
  _b -= d._b;
  return (*this);
}

DualScalar DualScalar::operator * ( const DualScalar & d ) const {
  //a = a1*a2
  //b = a1*b2 +b1*a2
  DualScalar s;
  s._a = _a * d._a;
  s._b = _a * d._b + _b * d._a;
  return s;
}

DualScalar & DualScalar::operator *= ( const DualScalar & d ){
  //a = a1*a2
  //b = a1*b2 +b1*a2
  DualScalar s;
  s._a = _a * d._a;
  s._b = _a * d._b + _b * d._a;
  _a = s._a;
  _b = s._b;
  return (*this);
}

DualScalar DualScalar::operator / ( const DualScalar & d ) const {
  //a = a1/a2
  //b = (b1*a2 - a1*b2)/(a1 * a2)
  DualScalar s;
  s._a = _a / d._a;
  s._b = (d._a * _b - _a * d._b)/(_a * d._a);
  return s;
}

DualScalar & DualScalar::operator /= (const DualScalar & d ){
  float a = _a / d._a;
  float b = (d._a * _b - _a * d._b)/(_a * d._a);
  _a = a;
  _b = b;
  return (*this);
}

DualScalar DualScalar::Conjugate() const {
  DualScalar s;
  s._a = _a;
  s._b = -_b;
  return s;
}

float DualScalar::Norm() const {
  return fabs(_a);
}

DualScalar DualScalar::Sqrt() const {
  DualScalar s;
  s._a = sqrt(_a);
  s._b = _b/(2 * s._a); //not sure about this equation
  return s;
}

DualScalar DualScalar::Pow( DualScalar e ) const {
  DualScalar s;
  s._a = pow( _a, e._a );
  s._b = _b/_a * e._a * s._a + e._b * s._a * log( _a );
  return s;
}

DualScalar DualScalar::Invert() const {
  DualScalar s;
  s._a = 1/ _a;
  s._b = - _b * s._a * s._a;
  return s;
}

float DualScalar::GetReal() const {
  return _a;
}

float DualScalar::GetDual() const {
  return _b;
}
