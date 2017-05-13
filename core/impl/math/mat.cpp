#include <cmath>
#include <cassert>

#include "mat.hpp"

namespace e2 { namespace math {

mat::mat(){
    // sets up default 4x4 dimension
    int dim[2] = { 4, 4 };
    resize_int( 2, dim );
}

void mat::resize_int( int count, int dim [] ){
    if( count == 0 ){
	_size = 0;
	delete [] _mat;
	_mat = new float [ 0 ];
	return;
    }else{
	//set mat dimension
	_dim.set_dim(count);
	_size = 1;
	for( int i = 0; i < count; i++ ){
	    _size *= dim[i];
	    _dim[i] = dim[i];
	}
	_mat = new float [ _size ];
	for( int i = 0; i < _size; i++ ){
	    _mat[i] = 0;
	}
    }
}

void mat::resize_vec( const vec & v ){
    float * dim;
    int count;
    v.get_array( count, dim );
    int * dim_int = new int[count];
    for(int i = 0; i < count; i++ ){
	dim_int[i] = floor(dim[i] + 0.5);
    }

    resize_int( count, dim_int );
}

mat & mat::operator = ( const mat & m ){
    _size = m._size;
    _dim = m._dim;
    _mat = new float [ _size ];
    memcpy( _mat, m._mat, sizeof(float)*_size );
    return *this;
}

mat mat::operator * ( const mat & m ) const{

    mat a;
  
    if( (int)_dim[1] != (int)m._dim[0] ){
	//size mismatch
	assert( false && "mat size mismatch" );
	return a;
    }else{
	//new mat size after multiplication
	int dim[2];
	dim[0] = _dim[0];
	dim[1] = m._dim[1];
	a.resize_int( 2, dim );
   
	//row index
	for( int i = 0; i < (int)a._dim[0]; i++){
	    //column index
	    for( int j = 0; j < (int)a._dim[1]; j++){
		// for( int x = 0; x < (int)_dim[1]; x++){
		//   a( i, j ) += (*this)( i, x ) * m( x, j );
		// }
		for( int x = 0; x < (int)_dim[1]; x++){
		    a._mat[ j * (int)a._dim[0] + i ] += _mat[ i + x * (int)_dim[0] ] * m._mat[ x + j * (int)m._dim[0] ];
		}
	    }
	}
	return a;
    }
}

mat mat::operator + ( const mat & a ) const{
    mat m;
    int dim[2];
    if( _dim[0] != a._dim[0] || _dim[1] != a._dim[1] ){
	m.resize_int( 0, dim );
	return m;
    }else{
	dim[0] = _dim[0];
	dim[1] = _dim[1];
	m.resize_int( 2, dim );
	for( int j = 0; j < _dim[1]; j++ ){
	    for( int i = 0; i < _dim[0]; i++ ){
		m( i, j ) = (*this)( i, j ) + a( i, j );
	    }
	}
	return m;
    }
}

mat mat::operator - ( const mat & a ) const{
    mat m;
    int dim[2];
    if( _dim[0] != a._dim[0] || _dim[1] != a._dim[1] ){
	m.resize_int( 0, dim );
	return m;
    }else{
	dim[0] = _dim[0];
	dim[1] = _dim[1];
	m.resize_int( 2, dim );
	for( int j = 0; j < _dim[1]; j++ ){
	    for( int i = 0; i < _dim[0]; i++ ){
		m( i, j ) = (*this)( i, j ) - a( i, j );
	    }
	}
	return m;
    }
}

float & mat::operator ()( int m, int n ){
    if( m >= (int)_dim[0] || n >= (int)_dim[1] ){
	return _mat[0];
    }else{
	return _mat[ m + n * (int)_dim[0] ];
    }
}

float mat::operator ()( int m, int n ) const{
    if( m >= (int)_dim[0] || n >= (int)_dim[1] ){
	return NAN;
    }else{
	return _mat[ m + n * (int)_dim[0] ];
    }
}

void mat::set_from_vec( const vec & v, bool column ){
    int dim[2];
    int len = v._dim;
    if( column == true ){
	dim[0] = len;
	dim[1] = 1;
	resize_int( 2, dim );
    }else{
	dim[0] = 1;
	dim[1] = len;
	resize_int( 2, dim );
    }
    for( int i = 0; i < len; i++ ){
	_mat[i] = v[i];
    }
}

bool mat::set_from_array( float * src, size_t col, size_t row ){
    int dim[2] { (int)col, (int)row };
    resize_int( 2, dim );
    for( int i = 0; i < col*row; ++i ){
	_mat[i] = *src++;
    }
    return true;
}

bool mat::get_array( float * dest, size_t dest_num, size_t & actual_size ) const {
    if( dest_num < _size )
	return false;
    actual_size = _size;
    float * current = _mat;
    for( int i = 0; i < _size; ++i ){
	*dest++ = *current++;
    }
    return true;
}

bool mat::get_vec( vec & v, int index, bool column ) const{
    if( column == true ){
	if( index >= _dim[1] ){
	    return false;
	}
	v.set_dim( _dim[0] );
	for( int i = 0; i < _dim[0]; i++ ){
	    v[i] = (*this)( i, index );
	}
    }else{
	if( index >= _dim[0] ){
	    return false;
	}
	v.set_dim( _dim[1] );
	for( int i = 0; i < _dim[1]; i++ ){
	    v[i] = (*this)( index, i );
	}
    }
    return true;
}

bool mat::get_sub_mat( mat & m, int row, int col, int sizerow, int sizecol ) const{
    if( row >= _dim[0] || col >= _dim[1] || row + sizerow - 1 >= _dim[0] || col + sizecol - 1 >= _dim[1] ){
	return false;
    }

    int dim[2];
    dim[0] = sizerow;
    dim[1] = sizecol;
    m.resize_int( 2, dim );

    for(int j = 0; j < sizecol; j++){
	for(int i = 0; i < sizerow; i++){
	    m( i, j ) = (*this)( row + i, col + j );
	}
    }

    return true;
}

void mat::transpose_current(){
    (*this) = transpose();
}

mat mat::transpose() const{
    mat m;
    int dim[2];
    dim[0] = _dim[1];
    dim[1] = _dim[0];
    m.resize_int( 2, dim );
  
    for( int j = 0; j < dim[1]; j++ ){
	for( int i = 0; i < dim[0]; i++ ){
	    m( i, j ) = (*this)( j, i );
	}
    }
    return m;
}

} }
