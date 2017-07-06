#include "dualscalar.hpp"
#include <cmath>

using namespace std;
using namespace e2::math;

dualscalar::dualscalar(){
  _a = 0;
  _b = 0;
}

dualscalar::dualscalar( float a, float b ){
  _a = a;
  _b = b;
}

dualscalar::dualscalar( dualscalar & d ){
  _a = d._a;
  _b = d._b;
}

void dualscalar::set_dual_number( float a, float b ){
  _a = a;
  _b = b;
}

void dualscalar::set_dual_scalar( const dualscalar & d ){
  _a = d._a;
  _b = d._b;
}

void dualscalar::set_array( float v [] ){
  _a = v[0];
  _b = v[1];
}

void dualscalar::get_array( float v [] ) const {
  v[0] = _a;
  v[1] = _b;
}

dualscalar & dualscalar::operator = (const dualscalar & d ){
  _a = d._a;
  _b = d._b;

  return (*this);
}

dualscalar dualscalar::operator + ( const dualscalar & d ) const {
  dualscalar s;
  s._a = _a + d._a;
  s._b = _b + d._b;
  return s;
}

dualscalar & dualscalar::operator += (const dualscalar & d ){
  _a += d._a;
  _b += d._b;
  return (*this);
}

dualscalar dualscalar::operator - ( const dualscalar & d ) const {
  dualscalar s;
  s._a = _a - d._a;
  s._b = _b - d._b;
  return s;
}

dualscalar & dualscalar::operator -= ( const dualscalar & d ){
  _a -= d._a;
  _b -= d._b;
  return (*this);
}

dualscalar dualscalar::operator * ( const dualscalar & d ) const {
  //a = a1*a2
  //b = a1*b2 +b1*a2
  dualscalar s;
  s._a = _a * d._a;
  s._b = _a * d._b + _b * d._a;
  return s;
}

dualscalar & dualscalar::operator *= ( const dualscalar & d ){
  //a = a1*a2
  //b = a1*b2 +b1*a2
  dualscalar s;
  s._a = _a * d._a;
  s._b = _a * d._b + _b * d._a;
  _a = s._a;
  _b = s._b;
  return (*this);
}

dualscalar dualscalar::operator / ( const dualscalar & d ) const {
  //a = a1/a2
  //b = (b1*a2 - a1*b2)/(a1 * a2)
  dualscalar s;
  s._a = _a / d._a;
  s._b = (d._a * _b - _a * d._b)/(_a * d._a);
  return s;
}

dualscalar & dualscalar::operator /= (const dualscalar & d ){
  float a = _a / d._a;
  float b = (d._a * _b - _a * d._b)/(_a * d._a);
  _a = a;
  _b = b;
  return (*this);
}

dualscalar dualscalar::conjugate() const {
  dualscalar s;
  s._a = _a;
  s._b = -_b;
  return s;
}

float dualscalar::norm() const {
  return fabs(_a);
}

dualscalar dualscalar::sqrt() const {
  dualscalar s;
  s._a = std::sqrt(_a);
  s._b = _b/(2 * s._a); //not sure about this equation
  return s;
}

dualscalar dualscalar::pow( dualscalar e ) const {
  dualscalar s;
  s._a = std::pow( _a, e._a );
  s._b = _b/_a * e._a * s._a + e._b * s._a * log( _a );
  return s;
}

dualscalar dualscalar::invert() const {
  dualscalar s;
  s._a = 1/ _a;
  s._b = - _b * s._a * s._a;
  return s;
}

float dualscalar::get_real() const {
  return _a;
}

float dualscalar::get_dual() const {
  return _b;
}
