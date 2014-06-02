#include "Vec.h"
#include <string.h>
#include <math.h>
#include <stdexcept>

Vec::Vec(){
  _dim = 3;
  _vec = new float [_dim];
  memset( _vec, 0, sizeof(float)*_dim );
}

Vec::Vec(int dim){
  _dim = dim;
  _vec = new float [_dim];
  memset( _vec, 0, sizeof(float)*_dim );
}

Vec::Vec(const Vec & v){
  _vec = new float[v._dim];
  _dim = v._dim;

  //copy data
  memcpy( _vec, v._vec, sizeof(float)*_dim );
}

Vec & Vec::operator=(const Vec & v)
{
  _vec = new float[v._dim];
  _dim = v._dim;

  //copy data
  memcpy( _vec, v._vec, sizeof(float)*_dim );

  return *this;
}

Vec::~Vec(){
  if( _dim == 1 || _dim == 0 )
    delete _vec;
  else if( _dim > 1 )
    delete [] _vec;
    
  _vec = NULL;
}

void Vec::SetDim(int dim){
  if( _dim != dim ){
    float * newVec = new float[dim];

    memset( newVec, 0, sizeof(float)*dim );
    memcpy( newVec, _vec, sizeof(float)*_dim );
 
    //delete old vector
    if( _dim == 1 || _dim == 0 )
      delete _vec;
    else if(_dim > 1)
      delete [] _vec;
    
    _vec = newVec;
    newVec = NULL;
    _dim = dim;
  }
}

Vec Vec::operator + (const Vec & v) const{
  if( _dim != v._dim )
    throw Exception( "Vec::operator+(): dimension not match" );

  Vec newVec( _dim );

  float * a = _vec;
  float * b = v._vec;
  float * c = newVec._vec;  

  for( int i = 0; i < _dim; i++ ){
    *c = *a + *b;
    a++;
    b++;
    c++;
  }

  return newVec;
}

Vec Vec::operator - (const Vec & v) const{
  if( _dim != v._dim )
    throw Exception( "Vec::operator-(): dimension not match" );

  Vec newVec( _dim );

  float * a = _vec;
  float * b = v._vec;
  float * c = newVec._vec;

  for( int i = 0; i < _dim; i++ ){
    *c = *a - *b;
    a++;
    b++;
    c++;
  }

  return newVec;
}

float Vec::Dot(const Vec & v) const{
  if( _dim != v._dim )
    throw Exception( "Vec::Dot(): dimension not match" );

  float * a = _vec;
  float * b = v._vec;
  float out = 0;

  for( int i = 0; i < _dim; i++ ){
    out += (*a) * (*b);
    a++;
    b++;
  }
  
  return out;
}

Vec Vec::Cross(const Vec & v) const{
  if( _dim != v._dim )
    throw Exception( "Vec::Cross(): dimension not match" );

  if( _dim != 3 )
    throw Exception( "Vec::Cross(): dimension should be 3" );

  Vec newVec(_dim);

  newVec._vec[0] = _vec[1]*v._vec[2] - _vec[2]*v._vec[1];
  newVec._vec[1] = -1.0f * (_vec[0]*v._vec[2] - _vec[2]*v._vec[0]);
  newVec._vec[2] = _vec[0]*v._vec[1] - _vec[1]*v._vec[0];

  return newVec;
}

float Vec::Magnitude() const{
  float out = 0;
  const float * a = (const float *) _vec; 
  
  for( int i = 0; i < _dim; i++ ){
    out += (*a)*(*a);
    a++;
  }
  
  return sqrt(out);
}

void Vec::NormalizeThis(){
  Vec v = Normalize();
  *this = v;
}

Vec Vec::Normalize() const {

  Vec v( *this );

  float mag = this->Magnitude();

  if( mag == 0 ){
    throw Exception( "Vec::Normalize(): magnitude is 0" );
    return v;
  }

  float * a = v._vec;

  for( int i = 0; i < v._dim; i++ ){
    *a = *a/mag;
    a++;
  }
  
  return v;
}


Vec ScaleVec( float s, const Vec v ){
  Vec a;
  a = v;
  for( int i = 0; i < v._dim; i++ ){
    a[i] = s * a[i];
  }
  return a;
}
Vec ScaleVecAdd( float s, const Vec v1, const Vec v2 ){
  Vec a = ScaleVec( s, v1 );
  return a + v2;
}
