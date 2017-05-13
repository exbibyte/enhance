#include <math.h>
#include <cassert>

#include "vec.hpp"

namespace e2 { namespace math {

vec::vec(){
    _dim = 3;
    _vec = new float [_dim];
    memset( _vec, 0, sizeof(float)*_dim );
}

vec::vec(int dim){
    _dim = dim;
    _vec = new float [_dim];
    memset( _vec, 0, sizeof(float)*_dim );
}

vec::vec(const vec & v){
    _vec = new float[v._dim];
    _dim = v._dim;

    //copy data
    memcpy( _vec, v._vec, sizeof(float)*_dim );
}

vec & vec::operator=(const vec & v)
{
    _vec = new float[v._dim];
    _dim = v._dim;

    //copy data
    memcpy( _vec, v._vec, sizeof(float)*_dim );

    return *this;
}

vec::~vec(){
    if( _dim == 1 || _dim == 0 )
	delete _vec;
    else if( _dim > 1 )
	delete [] _vec;
    
    _vec = nullptr;
}

void vec::set_dim(int dim){
    if( _dim != dim ){
	float * newvec = new float[dim];

	memset( newvec, 0, sizeof(float)*dim );
	memcpy( newvec, _vec, sizeof(float)*_dim );
 
	//delete old vector
	if( _dim == 1 || _dim == 0 )
	    delete _vec;
	else if(_dim > 1)
	    delete [] _vec;
    
	_vec = newvec;
	newvec = nullptr;
	_dim = dim;
    }
}

vec vec::operator + (const vec & v) const{
    if( _dim != v._dim )
	assert( false && "vec::operator+(): dimension not match" );

    vec newvec( _dim );

    float * a = _vec;
    float * b = v._vec;
    float * c = newvec._vec;  

    for( int i = 0; i < _dim; i++ ){
	*c = *a + *b;
	a++;
	b++;
	c++;
    }

    return newvec;
}

vec vec::operator - (const vec & v) const{
    if( _dim != v._dim )
	assert( false && "vec::operator-(): dimension not match" );

    vec newvec( _dim );

    float * a = _vec;
    float * b = v._vec;
    float * c = newvec._vec;

    for( int i = 0; i < _dim; i++ ){
	*c = *a - *b;
	a++;
	b++;
	c++;
    }

    return newvec;
}

vec vec::operator / (const vec & v) const{
    if( _dim != v._dim )
	assert( false && "vec::operator/(): dimension not match" );

    vec newvec( _dim );

    for( int i = 0; i < _dim; i++ ){
	if( v[i] == 0 ){
	    assert( false && "vec::operator/(): zero division" );
	}
	newvec._vec[i] = _vec[i] / v._vec[i];
    }

    return newvec;
}
bool vec::is_equal(const vec & v, float error) const{
    if( _dim != v._dim ){
	return false;
    }
    for( int i = 0; i < _dim; ++i ){
	if( ( _vec[i] < v._vec[i] - error ) || ( _vec[i] > v._vec[i] + error ) ){
	    return false;
	}
    }
    return true;
}

float vec::dot(const vec & v) const{
    if( _dim != v._dim )
	assert( false && "vec::dot(): dimension not match" );

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

vec vec::cross(const vec & v) const{
    if( _dim != v._dim )
	assert( false && "vec::cross(): dimension not match" );

    if( _dim < 3 ){
	assert( false && "vec::cross(): dimension invalid" );
    }

    //ignore higher dimensions than 3
    vec newvec(_dim);

    newvec._vec[0] = _vec[1]*v._vec[2] - _vec[2]*v._vec[1];
    newvec._vec[1] = -1.0f * (_vec[0]*v._vec[2] - _vec[2]*v._vec[0]);
    newvec._vec[2] = _vec[0]*v._vec[1] - _vec[1]*v._vec[0];

    return newvec;
}

float vec::magnitude() const{
    float out = 0;
    const float * a = (const float *) _vec; 
  
    for( int i = 0; i < _dim; i++ ){
	out += (*a)*(*a);
	a++;
    }
  
    return sqrt(out);
}

void vec::normalize_current(){
    vec v = normalize();
    *this = v;
}

vec vec::normalize() const {

    vec v( *this );

    float mag = this->magnitude();

    if( mag == 0 ){
	assert( false && "vec::normalize(): magnitude is 0" );
	return v;
    }

    float * a = v._vec;

    for( int i = 0; i < v._dim; i++ ){
	*a = *a/mag;
	a++;
    }
  
    return v;
}

void vec::set_from_array(int dim, float array [] ) {
    set_dim( dim );
    for( int i = 0; i < dim; i++ ){
	_vec[i] = array[i];
    }
}

void vec::set_from_array(int dim, double * array ){
    set_dim( dim );
    for( int i = 0; i < dim; i++ ){
	_vec[i] = array[i];
    }
}

void vec::get_array(int & dim, float * & array ) const {
    dim = _dim;
    array = new float[dim];
    for( int i = 0; i < dim; i++ ){
	array[i] = _vec[i];
    }
}

bool vec::get_array( float * dest, size_t dest_num, size_t & actual_num ) const {
    if( dest_num < _dim )
	return false;
    actual_num = _dim;
    for( int i = 0; i < _dim; i++ ){
	*dest++ = _vec[i];
    }
    return true;
}

vec vec::scale_vec( float s, const vec v ){
    vec a;
    a = v;
    for( int i = 0; i < v._dim; i++ ){
	a[i] = s * a[i];
    }
    return a;
}
vec vec::scale_vec_add( float s, const vec v1, const vec v2 ){
    vec a = scale_vec( s, v1 );
    return a + v2;
}

vec vec::get_sub_vector( int index_start, int index_end ) const{
    if( index_start < 0 || index_start >= _dim ){
	assert( false && "vec::getsubvector(): invalid index" );
    }
    if( index_end < 0 || index_end >= _dim ){
	assert( false && "vec::getsubvector(): invalid index" );
    }
    int n = index_end - index_start;
    if( n > _dim ){
	assert( false && "vec::getsubvector(): invalid size" );
    }
    vec v(n);
    for( int i = 0; i < n; ++i ){
	v._vec[i] = _vec[index_start + i];
    }
    return v;
}

} }
